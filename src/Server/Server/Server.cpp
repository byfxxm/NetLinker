#include <WINSOCK2.H>
#include <ws2tcpip.h>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;

#pragma comment(lib,"ws2_32.lib")

int main()
{
	//创建套接字
	WORD myVersionRequest;
	WSADATA wsaData;                    //包含系统所支持的WinStock版本信息
	myVersionRequest = MAKEWORD(1, 1);  //初始化版本1.1
	int err;
	err = WSAStartup(myVersionRequest, &wsaData);
	if (!err) {
		printf("已打开套接字\n");
	}
	else {
		//进一步绑定套接字
		printf("套接字未打开!");
		return 1;
	}
	SOCKET serSocket = socket(AF_INET, SOCK_STREAM, 0);//创建了可识别套接字
	//需要绑定的参数
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//ip地址
	addr.sin_port = htons(3000);//绑定端口
	//将套接字绑定到指定的网络地址
	bind(serSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR));//绑定完成
	listen(serSocket, 10);                              //第二个参数代表能够接收的最多的连接数
	SOCKADDR_IN clientsocket;
	int len = sizeof(SOCKADDR);
	SOCKET serConn;
	//等待客户端的连接
	serConn = accept(serSocket, (SOCKADDR*)&clientsocket, &len);
	cout << "客户端" << inet_ntoa(clientsocket.sin_addr) << "已连接" << endl;             //客户端已连接
	//inet_ntop();
	while (1) {
		char sendBuf[100];
		sprintf_s(sendBuf, "server : welcome %s to server.", inet_ntoa(clientsocket.sin_addr));
		//在对应的IP处并且将这行字打印到那里
		send(serConn, sendBuf, strlen(sendBuf) + 1, 0);
		char receiveBuf[100];
		//接收客户端传来的信息
		recv(serConn, receiveBuf, strlen(receiveBuf) + 1, 0);
		const char* quit = "quit";
		//如果客户端传来了quit信号，则服务端关闭，客户端也关闭
		if (!strcmp(receiveBuf, quit)) {
			break;
		}
		printf("%s\n", receiveBuf);
	}
	closesocket(serConn);   //关闭
	WSACleanup();           //释放资源的操作
}