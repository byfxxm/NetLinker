#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main()
{
	WSADATA _wsaData;
	WORD _version = MAKEWORD(2, 2);

	if (WSAStartup(_version, &_wsaData))
	{
		cout << "server is not opened!" << endl;
		return 1;
	}
	cout << "server is opened!" << endl;

	SOCKET _serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN _addr;
	_addr.sin_family = AF_INET;
	_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	_addr.sin_port = htons(3000);
	
	bind(_serverSocket, (SOCKADDR*)&_addr, sizeof(SOCKADDR));

	while (true)
	{
		cout << "connecting..." << endl;
		listen(_serverSocket, 10);	//第二个参数代表能够接收的最多的连接数

		SOCKADDR_IN _clientSocket;
		int _nAddrLen = sizeof(SOCKADDR);
		SOCKET _serConn = accept(_serverSocket, (SOCKADDR*)&_clientSocket, &_nAddrLen);

		char _buff[100] = { 0 };
		inet_ntop(AF_INET, &_clientSocket.sin_addr, _buff, sizeof(_buff));
		cout << _buff << " is connected" << endl;

		while (true)
		{
			char _receiveBuf[100] = { 0 };
			if (SOCKET_ERROR == recv(_serConn, _receiveBuf, sizeof(_receiveBuf), 0))
			{
				cout << "disconnet" << endl;
				break;
			}

			if (!strcmp(_receiveBuf, "quit"))
				break;

			cout << _receiveBuf << endl;
		}

		closesocket(_serConn);
	}

	WSACleanup();

	return 0;
}