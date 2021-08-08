#pragma once

struct Pack;
class CSocket
{
public:
	CSocket();
	~CSocket();

	bool Send(SOCKET, const char*, int);
	bool Send(SOCKET, Pack*);
	bool Recv(SOCKET, char*, int);
	bool Recv(SOCKET, Pack*);
};