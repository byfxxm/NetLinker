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
	if (send(m_Socket, pMsg_, strlen(pMsg_) + 1, 0))
		return false;

	return true;;
}

void CClient::SendBytes(const char* pBytes_, int nLen_)
{
	while (!send(m_Socket, pBytes_, nLen_, 0))
		std::this_thread::yield();
}

bool CClient::SendFile(const char* pFile_)
{
	std::ifstream _fin(pFile_, std::ios::binary | std::ios::out);

	if (!_fin)
		return false;

	char _name[50];
	char _ext[10];
	char _file[60];
	_splitpath_s(pFile_, nullptr, 0, nullptr, 0, _name, sizeof(_name), _ext, sizeof(_ext));
	sprintf_s(_file, "%%%%%s%s", _name, _ext);
	SendMsg(_file);

	char _buff[1000] = { 0 };
	while (true)
	{
		memset(_buff, 0, sizeof(_buff));
		_fin.read(_buff, sizeof(_buff));
		SendBytes(_buff, sizeof(_buff));
		if (_fin.eof())
		{
			SendBytes("%%", 2);
			break;
		}
	}

	return true;
}