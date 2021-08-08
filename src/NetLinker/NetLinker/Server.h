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
	list<SOCKET> m_listClient;
	thread m_thdListener;
	char m_szRecFilePath[MAX_PATH];
};