#pragma once
#include "Socket.h"

class CClient : public CSocket
{
public:
	CClient();
	~CClient();

	bool Connect(const char*, int);
	bool SendMsg(const char*);
	bool SendFile(const char*);

private:
	SOCKET m_Socket;
};