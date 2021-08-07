#include "pch.h"
#include "Server.h"

using std::cout;
using std::endl;

CServer::CServer()
{
	GetModuleFileName(NULL, m_szRecFilePath, sizeof(m_szRecFilePath));
	*(strrchr(m_szRecFilePath, '\\') + 1) = '\0';

	WSADATA _wsaData;
	WSAStartup(MAKEWORD(2, 2), &_wsaData);

	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

CServer::~CServer()
{
	m_thdListener.join();
	closesocket(m_Socket);
	WSACleanup();
}

void CServer::Listen(int nPort_)
{
	SOCKADDR_IN _addr;
	_addr.sin_family = AF_INET;
	_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	_addr.sin_port = htons(nPort_);

	bind(m_Socket, (SOCKADDR*)&_addr, sizeof(SOCKADDR));
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
					char _buff[BUFFER_SIZE] = { 0 };
					int _count = 0;
					_count = recv(_serConn, _buff, sizeof(_buff), 0);
					send(_serConn, "%", 1, 0);

					if (SOCKET_ERROR == _count)
					{
						closesocket(_serConn);
						return;
					}

					if (strncmp(_buff, MASK, MASK_SIZE) == 0)
					{
						if (m_FileOut.is_open())
						{
							m_FileOut.close();
							cout << "receive file finished" << endl;
							continue;
						}

						cout << "receive file: " << &_buff[MASK_SIZE] << endl;
						std::string _filePath(m_szRecFilePath);
						_filePath += &_buff[MASK_SIZE];
						m_FileOut.open(_filePath, std::ios::binary);
						continue;
					}

					if (m_FileOut.is_open())
					{
						m_FileOut.write(_buff, _count);
						continue;
					}

					cout << "receive msg: " << _buff << endl;
				}
			});

			_connect.detach();
		}
	});
	
	m_thdListener.swap(_listen);
}