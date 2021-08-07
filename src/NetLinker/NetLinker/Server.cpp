#include "pch.h"
#include "Server.h"
#include "Protocol.h"

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
	closesocket(m_Socket);
	m_thdListener.join();
	WSACleanup();
}

Pack* CServer::RecvBytes(SOCKET Connect_)
{
	int _count = 0;

	char _head[PACKHEAD] = { 0 };
	_count = recv(Connect_, _head, PACKHEAD, 0);
	if (SOCKET_ERROR == _count)
		return nullptr;

	int _length = *(int*)_head;
	auto _pack = new(new char[PACKHEAD + _length])Pack(_length);
	_count = recv(Connect_, _pack->Data, _pack->nDataLen, 0);
	if (SOCKET_ERROR == _count)
	{
		delete _pack;
		return nullptr;
	}

	return _pack;
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
			if (_serConn == INVALID_SOCKET)
				return;

			m_listClient.push_back(_serConn);

			thread _connect([this, _serConn]()
			{
				while (true)
				{
					Pack* _pPack = nullptr;
					delete _pPack;
					_pPack = RecvBytes(_serConn);
					if (_pPack == nullptr)
					{
						closesocket(_serConn);
						m_listClient.erase(std::find(m_listClient.begin(), m_listClient.end(), _serConn));
						return;
					}

					if (memcmp(_pPack->Data, MASK, MASK_SIZE) == 0)
					{
						if (m_FileOut.is_open())
						{
							m_FileOut.close();
							cout << "receive file finished" << endl;
							continue;
						}

						cout << "receive file: " << &_pPack->Data[MASK_SIZE] << endl;
						std::string _filePath(m_szRecFilePath);
						_filePath += &_pPack->Data[MASK_SIZE];
						m_FileOut.open(_filePath, std::ios::binary | std::ios::out);
						continue;
					}

					if (m_FileOut.is_open())
					{
						m_FileOut.write(_pPack->Data, _pPack->nDataLen);
						continue;
					}

					cout << "receive msg: " << _pPack->Data << endl;
				}
			});

			_connect.detach();
		}
	});
	
	m_thdListener.swap(_listen);
}