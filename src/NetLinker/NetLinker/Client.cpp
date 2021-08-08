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
			if (!RecvFile(m_Socket, _pPack))
				return;

			cout << _pPack->Data << endl;
		}
	});

	_thd.detach();
	return true;
}

SOCKET CClient::GetSocket()
{
	return m_Socket;
}