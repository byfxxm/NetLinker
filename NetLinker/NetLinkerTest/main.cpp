// NetLinkerTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <winsock.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	DWORD _nVersionRequested = MAKEWORD(2, 1);
	WSADATA _WsaData;
	int _startup = WSAStartup(_nVersionRequested, &_WsaData);

	SOCKET _socket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in _addr;
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	_addr.sin_port = htons(1234);

	bind(_socket, (sockaddr*)&_addr, sizeof(_addr));

	return 0;
}
