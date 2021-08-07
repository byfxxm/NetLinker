#pragma once

class CClient
{
public:
	CClient();
	~CClient();

	bool Connect(const char*, int);
	bool SendMsg(const char*);
	bool SendFile(const char*);

private:
	void SendBytes(const char*, int);
	SOCKET m_Socket;
};