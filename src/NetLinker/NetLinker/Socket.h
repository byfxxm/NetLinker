#pragma once

struct Pack;
class CSocket
{
public:
	CSocket();
	~CSocket();

	bool SendMsg(SOCKET, const char*);
	bool SendFile(SOCKET, const char*);
	bool SendBytes(SOCKET, const char*, int);

	bool RecvBytes(SOCKET, char*, int);
	bool Recv(SOCKET, Pack*);
	bool RecvFile(SOCKET, Pack*);
};