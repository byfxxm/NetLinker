#pragma once

class CClient
{
public:
	CClient();
	~CClient();

	bool Connect(const char*, int);
	bool SendMsg(const char*);
	bool ReceiveMsg(char*, int);

private:
	SOCKET m_Socket;
};