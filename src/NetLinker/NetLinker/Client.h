#pragma once
#include "Socket.h"

class CClient : public CSocket
{
public:
	CClient();
	~CClient();

	bool Connect(const char*, int);
	SOCKET GetSocket();

private:
	SOCKET m_Socket;
};