#pragma once

using std::thread;
using std::list;

class CServer
{
public:
	CServer();
	~CServer();

	void Listen(int);

private:
	SOCKET m_Socket;
	list<SOCKET> m_listClient;
	thread m_thdListener;
	std::ofstream m_FileOut;
	char m_szRecFilePath[MAX_PATH];
};