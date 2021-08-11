#include "pch.h"
#include "Server.h"
#include "Protocol.h"

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

void CServer::Listen(int nPort_)
{
	SOCKADDR_IN _addr;
	_addr.sin_family = AF_INET;
	_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	_addr.sin_port = htons(nPort_);

	::bind(m_Socket, (SOCKADDR*)&_addr, sizeof(SOCKADDR));
	listen(m_Socket, 10);
	cout << "listening..." << endl;

	thread _listen([this]()
	{
		while (true)
		{
			SOCKADDR_IN _clientSocket;
			int _nAddrLen = sizeof(SOCKADDR);
			SOCKET _serConn = accept(m_Socket, (SOCKADDR*)&_clientSocket, &_nAddrLen);
			if (_serConn == INVALID_SOCKET)
				return;

			cout << _serConn << " connected." << endl;
			m_listClient.push_back(_serConn);

			thread _connect([this, _serConn]()
			{
				char _buff[BUFFER_SIZE] = { 0 };
				auto _pPack = new(_buff) Pack();
				unsigned long long _fileSize = 0;
				unsigned long long _filePos = 0;
				ofstream _fout;

				while (true)
				{
					if (!Recv(_serConn, _pPack))
					{
						closesocket(_serConn);
						m_listClient.erase(find(m_listClient.begin(), m_listClient.end(), _serConn));
						return;
					}

					if (memcmp(_pPack->Data, MASK, MASK_SIZE) == 0)
					{
						char* _pData = &_pPack->Data[MASK_SIZE];
						int _nLen = _pPack->nDataLen - MASK_SIZE;

						if (memcmp(_pData, EOFILE, EOFILE_SIZE) == 0)
						{
							_fout.close();
							printf("\n");
							char _finish[] = "finished!";
							Send(_serConn, _finish, strlen(_finish) + 1);
							continue;
						}

						if (memcmp(_pData, FILELEN, FILELEN_SIZE) == 0)
						{
							_fileSize = stoull(&_pData[FILELEN_SIZE]);
							continue;
						}

						if (_fout.is_open())
						{
							_fout.write(_pData, _nLen);
							_filePos += _nLen;
							if (_fileSize > 0)
								printf("\r%llu%%", (unsigned long long)((double)_filePos / _fileSize * 100));

							continue;
						}

						cout << "receive file: " << _pData << endl;
						string _filePath(m_szRecFilePath);
						_filePath += _pData;
						_fileSize = 0;
						_filePos = 0;

						while (!_fout.is_open())
							_fout.open(_filePath, ios::binary);

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