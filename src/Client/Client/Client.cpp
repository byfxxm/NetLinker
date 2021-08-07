#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main()
{
	WSADATA _wsaData;
	WORD _version = MAKEWORD(2, 2);

	if (WSAStartup(_version, &_wsaData))
	{
		cout << "client is not opened" << WSAGetLastError() << endl;
		return 1;
	}
	cout << "client is opened" << endl;

	SOCKET _clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN _clientSockAddr;

	inet_pton(AF_INET, "127.0.0.1", &_clientSockAddr.sin_addr);
	_clientSockAddr.sin_family = AF_INET;
	_clientSockAddr.sin_port = htons(3000);

	if (connect(_clientSocket, (SOCKADDR*)&_clientSockAddr, sizeof(SOCKADDR)))
	{
		cout << "disconnect" << endl;
		return 1;
	}

	string s;
	while (cin >> s)
	{
		send(_clientSocket, s.c_str(), s.length() + 1, 0);

		if (s == "quit")
			break;
	}

	closesocket(_clientSocket);
	WSACleanup();

	return 0;
}