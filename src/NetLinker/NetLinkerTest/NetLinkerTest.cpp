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
	Server_Listen(_pServer, 1234);

	thread _thds[1];
	for (auto& _th : _thds)
	{
		thread _t([]()
		{
			void* _pClient = Client_Create();
			while (!Client_Connect(_pClient, "127.0.0.1", 1234))
			{
				cout << "great" << endl;
				this_thread::yield();
			}
			
			//Client_SendFile(_pClient, "D:\\new.txt");
			//Client_SendFile(_pClient, "G:\\Download\\BigFoot_WOW_5.3.0.1.exe");
			Client_SendFile(_pClient, "G:\\Download\\466.47-notebook-win10-64bit-international-dch-whql.exe");
			getchar();
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