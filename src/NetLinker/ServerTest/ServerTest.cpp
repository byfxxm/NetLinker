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
	void* _pServer = Server_Create();
	int _nPort = 0;
	cout << "input port: ";
	cin >> _nPort;
	cout << "listening..." << endl;
	Server_Listen(_pServer, _nPort);

	while (true)
		std::this_thread::yield();

	Server_Delete(_pServer);
	return 0;
}