#pragma once
#include "Socket.h"

class CServer : public CSocket
{
public:
	CServer();
	~CServer();
	void Listen(int);

private:
	SOCKET m_Socket;
	std::list<SOCKET> m_listClient;
	std::thread m_thdListener;
	std::ofstream m_FileOut;
	char m_szRecFilePath[MAX_PATH];
};