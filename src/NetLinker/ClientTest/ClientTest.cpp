#include <iostream>
#include <thread>
#include <string>
#include "../NetLinker/NetLinker.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/NetLinker.lib")
#else
#pragma comment(lib, "../Release/NetLinker.lib")
#endif

using namespace std;

int main()
{
	void* _pClient = Client_Create();

	char _socket[64] = { 0 };
	char* _port = nullptr;
	while (true)
	{
		cout << "input addr and port: ";
		cin >> _socket;
		_port = strrchr(_socket, ':');

		if (_port != nullptr)
		{
			*_port++ = '\0';
			break;
		}

		cout << "invalidate addr and port" << endl;
		continue;
	}

	Client_Connect(_pClient, _socket, atoi(_port));
	string _s;
	while (true)
	{
		cout << "input file path: ";
		cin >> _s;
		cout << "copy file..." << endl;
		Client_SendFile(_pClient, _s.c_str());
	}

	Client_Delete(_pClient);
	return 0;
}