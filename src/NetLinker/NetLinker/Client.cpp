#include "pch.h"
#include "Client.h"

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

	if (connect(m_Socket, (SOCKADDR*)&_clientSockAddr, sizeof(SOCKADDR)))
		return false;

	return true;;
}

bool CClient::SendMsg(const char* pMsg_)
{
	//char _msg[2000] = { 0 };
	//Packet* _pPack = new(_msg) Packet();
	//_pPack->nDataLen = strlen(pMsg_) + 1;
	//memcpy(_pPack->Data, pMsg_, _pPack->nDataLen);

	if (SOCKET_ERROR == send(m_Socket, pMsg_, strlen(pMsg_) + 1, 0))
		return false;

	char _beat;
	recv(m_Socket, &_beat, 1, 0);
	return true;;
}

bool CClient::SendBytes(const char* pBytes_, int nLen_)
{
	if (SOCKET_ERROR == send(m_Socket, pBytes_, nLen_, 0))
		return false;

	char _beat;
	recv(m_Socket, &_beat, 1, 0);
	return true;
}

bool CClient::SendFile(const char* pFile_)
{
	std::ifstream _fin(pFile_, std::ios::binary);

	if (!_fin)
		return false;

	char _file[MAX_PATH];
	strcpy_s(_file, pFile_);
	char* _name = strrchr(_file, '\\');
	_name++;
	
	char _str[MAX_PATH] = MASK;
	strcat_s(_str, _name);
	SendMsg(_str);

	char _buff[BUFFER_SIZE] = { 0 };
	int _sum = 0;
	while (true)
	{
		memset(_buff, 0, sizeof(_buff));
		_fin.read(_buff, sizeof(_buff));
		SendBytes(_buff, (int)_fin.gcount());

		if (_fin.eof())
		{
			SendMsg(MASK);
			break;
		}
	}

	return true;
}