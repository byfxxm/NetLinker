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
	void* _pServer = Server_Create(3000);

	thread _thds[1];
	for (auto& _th : _thds)
	{
		thread _t([]()
		{
			void* _pClient = Client_Create();
			Client_Connect(_pClient, "127.0.0.1", 3000);

			while (true)
			{
				//string _s;
				//cin >> _s;
				//if (_s == "quit")
				//	break;
				//Client_SendFile(_pClient, "D:\\depends22_x86.zip");
				Client_SendFile(_pClient, "D:\\depends22_x86.zip");
				getchar();
				//Client_SendMsg(_pClient, _s.c_str());
			}
			Client_Delete(_pClient);
		});

		_th.swap(_t);
	}

	for (auto& _th : _thds)
	{
		_th.join();
	}

	Server_Delete(_pServer);

	return 0;
}