#pragma once

using std::thread;
using std::list;

class CServer
{
public:
	CServer(int);
	~CServer();

	bool SendMsg(const char*);
	bool ReceiveMsg(char*, int);

private:
	void Listen();
	SOCKET m_Socket;
	list<SOCKET> m_listClient;
	thread m_thdListener;
};