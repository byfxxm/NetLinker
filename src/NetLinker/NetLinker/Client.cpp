#include "pch.h"
#include "Client.h"
#include "Protocol.h"

CClient::CClient()
{
	WSADATA _wsaData;
	WSAStartup(MAKEWORD(2, 2), &_wsaData);
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

CClient::~CClient()
{
	closesocket(m_Socket);
	WSACleanup();
}

bool CClient::Connect(const char* pAddr_, int nPort_)
{
	SOCKADDR_IN _clientSockAddr;
	inet_pton(AF_INET, pAddr_, &_clientSockAddr.sin_addr);
	_clientSockAddr.sin_family = AF_INET;
	_clientSockAddr.sin_port = htons(nPort_);

	if (SOCKET_ERROR == connect(m_Socket, (SOCKADDR*)&_clientSockAddr, sizeof(SOCKADDR)))
		return false;

	return true;
}

bool CClient::SendMsg(const char* pMsg_)
{
	return SendBytes(pMsg_, strlen(pMsg_) + 1);
}

bool CClient::SendBytes(const char* pBytes_, int nLen_)
{
	auto _pMem = new char[PACKHEAD + nLen_];
	auto _pPack = new(_pMem) Pack(nLen_);
	memcpy(_pPack->Data, pBytes_, nLen_);

	bool _bRet = (SOCKET_ERROR != send(m_Socket, _pMem, PACKHEAD + nLen_, 0));
	delete[] _pMem;

	return _bRet;
}

bool CClient::SendFile(const char* pFile_)
{
	std::ifstream _fin(pFile_, std::ios::binary);

	if (!_fin.is_open())
		return false;

	char _file[MAX_PATH];
	strcpy_s(_file, pFile_);
	char* _name = strrchr(_file, '\\');
	_name++;

	char _str[1024] = { 0 };
	sprintf_s(_str, MASK "%s", _name);
	SendMsg(_str);

	_fin.seekg(0, std::ios::end);
	sprintf_s(_str, MASK FILELEN "%llu", (unsigned long long)_fin.tellg());
	SendMsg(_str);
	_fin.seekg(0, std::ios::beg);

	char _buff[BUFFER_SIZE] = { 0 };
	while (true)
	{
		memcpy(_buff, MASK, MASK_SIZE);
		_fin.read(_buff + MASK_SIZE, sizeof(_buff) - MASK_SIZE - PACKHEAD);
		SendBytes(_buff, (int)_fin.gcount() + MASK_SIZE);

		if (_fin.eof())
		{
			SendBytes(MASK EOFILE, MASK_SIZE + EOFILE_SIZE);
			break;
		}
	}

	return true;
}