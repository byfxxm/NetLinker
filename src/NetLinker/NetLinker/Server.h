#pragma once

using std::thread;
using std::list;
struct Pack;

class CServer
{
public:
	CServer();
	~CServer();

	void Listen(int);
	bool RecvBytes(SOCKET, Pack*);

private:
	bool Recv(SOCKET, char*, int);

	SOCKET m_Socket;
	list<SOCKET> m_listClient;
	thread m_thdListener;
	std::ofstream m_FileOut;
	char m_szRecFilePath[MAX_PATH];
};