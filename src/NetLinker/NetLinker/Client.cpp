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

	thread _thd([this]()
	{
		char _buff[BUFFER_SIZE] = { 0 };
		auto _pPack = new(_buff) Pack();

		while (true)
		{
			if (!Recv(m_Socket, _pPack))
				return;

			cout << _pPack->Data << endl;
		}
	});

	_thd.detach();
	return true;
}

bool CClient::SendMsg(const char* pMsg_)
{
	return Send(m_Socket, pMsg_, strlen(pMsg_) + 1);
}

bool CClient::SendFile(const char* pFile_)
{
	ifstream _fin(pFile_, ios::binary);
	if (!_fin.is_open())
		return false;

	char _file[MAX_PATH];
	strcpy_s(_file, pFile_);
	char* _name = strrchr(_file, '\\');
	_name++;

	char _str[1024] = { 0 };
	sprintf_s(_str, MASK "%s", _name);
	SendMsg(_str);

	_fin.seekg(0, ios::end);
	sprintf_s(_str, MASK FILELEN "%llu", (unsigned long long)_fin.tellg());
	SendMsg(_str);
	_fin.seekg(0, ios::beg);

	char _buff[BUFFER_SIZE] = { 0 };
	while (true)
	{
		memcpy(_buff, MASK, MASK_SIZE);
		_fin.read(_buff + MASK_SIZE, sizeof(_buff) - MASK_SIZE - PACKHEAD);
		Send(m_Socket, _buff, (int)_fin.gcount() + MASK_SIZE);

		if (_fin.eof())
		{
			Send(m_Socket, MASK EOFILE, MASK_SIZE + EOFILE_SIZE);
			break;
		}
	}

	return true;
}