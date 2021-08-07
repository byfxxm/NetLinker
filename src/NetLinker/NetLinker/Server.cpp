#include "pch.h"
#include "Server.h"

using std::cout;
using std::endl;

CServer::CServer(int nPort_)
{
	WSADATA _wsaData;
	WSAStartup(MAKEWORD(2, 2), &_wsaData);

	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN _addr;
	_addr.sin_family = AF_INET;
	_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	_addr.sin_port = htons(nPort_);

	bind(m_Socket, (SOCKADDR*)&_addr, sizeof(SOCKADDR));

	Listen();
}

CServer::~CServer()
{
	m_thdListener.join();
	closesocket(m_Socket);
	WSACleanup();
}

void CServer::Listen()
{
	listen(m_Socket, 10);

	thread _listen([this]()
	{
		while (true)
		{
			SOCKADDR_IN _clientSocket;
			int _nAddrLen = sizeof(SOCKADDR);
			SOCKET _serConn = accept(m_Socket, (SOCKADDR*)&_clientSocket, &_nAddrLen);

			thread _connect([=]()
			{
				while (true)
				{
					//char _buff[100] = { 0 };
					//inet_ntop(AF_INET, &_clientSocket.sin_addr, _buff, sizeof(_buff));

					char _receiveBuf[1000] = { 0 };
					memset(_receiveBuf, 0, sizeof(_receiveBuf));
					if (SOCKET_ERROR == recv(_serConn, _receiveBuf, sizeof(_receiveBuf), 0))
					{
						closesocket(_serConn);
						return;
					}

					if (strncmp(_receiveBuf, "%%", 2) == 0)
					{
						if (m_FileOut.is_open())
						{
							m_FileOut.close();
							continue;
						}

						cout << "receive file " << &_receiveBuf[2] << ": " << endl;

						//GetModuleFileName()
						std::string _filePath("F:\\");
						_filePath += &_receiveBuf[2];
						m_FileOut.open(_filePath);
						continue;
					}

					if (m_FileOut.is_open())
					{
						m_FileOut.write(_receiveBuf, sizeof(_receiveBuf));
						continue;
					}

					cout << "receive msg: " << _receiveBuf << endl;
				}
			});

			_connect.detach();
		}
	});
	
	m_thdListener.swap(_listen);
}

bool CServer::SendMsg(const char* pMsg_)
{
	if (m_listClient.empty())
		return false;

	for (auto& _client : m_listClient)
	{
		send(_client, pMsg_, strlen(pMsg_) + 1, 0);
	}

	return true;;
}

bool CServer::ReceiveMsg(char*, int)
{
	//if (SOCKET_ERROR == recv(_serConn, _receiveBuf, sizeof(_receiveBuf), 0))
	//{
	//	closesocket(_serConn);
	//}

	return true;
}