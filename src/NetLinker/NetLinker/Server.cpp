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

bool CServer::Recv(SOCKET Connect_, char* pBuf_, int nLen_)
{
	int _sum = nLen_;

	while (_sum > 0)
	{
		int _count = recv(Connect_, pBuf_, _sum, 0);
		if (SOCKET_ERROR == _count)
			return false;

		pBuf_ += _count;
		_sum -= _count;
	}

	return _sum == 0;
}

bool CServer::RecvBytes(SOCKET Connect_, Pack* pPack_)
{
	if (!Recv(Connect_, (char*)pPack_, PACKHEAD))
		return false;

	if (!Recv(Connect_, pPack_->Data, pPack_->nDataLen))
		return false;

	return true;
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
				char _buff[BUFFER_SIZE] = { 0 };
				auto _pPack = new(_buff) Pack();

				while (true)
				{
					if (!RecvBytes(_serConn, _pPack))
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

						cout << _filePath.c_str() << endl;

						while (!m_FileOut.is_open())
							m_FileOut.open(_filePath, std::ios::binary);

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