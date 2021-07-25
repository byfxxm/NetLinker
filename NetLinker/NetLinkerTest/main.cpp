// NetLinkerTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSADATA _WsaData;
	WSAStartup(1, &_WsaData);

	return 0;
}

