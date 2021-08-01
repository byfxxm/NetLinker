#include <iostream>
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	//加载Winsock DLL
	WORD _nVersionRequested = MAKEWORD(2, 2);
	WSADATA _WsaData;
	int _startup = WSAStartup(_nVersionRequested, &_WsaData);

	//创建socket
	SOCKET _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	sockaddr_in _addr;
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	_addr.sin_port = htons(1234);

	bind(_socket, (sockaddr*)&_addr, sizeof(_addr));

	//进入监听状态，等待用户发起请求
	listen(_socket, 20);

	//接收客户端请求
	struct sockaddr_in clnt_addr;
	int clnt_addr_size = sizeof(clnt_addr);
	int clnt_sock = accept(_socket, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

	//向客户端发送数据
	char str[] = "http://c.biancheng.net/socket/";
	//write(clnt_sock, str, sizeof(str));

	closesocket(_socket);
	WSACleanup();
	return 0;
}
