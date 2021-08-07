#include <WINSOCK2.H>

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <string>
using std::string;
#include <conio.h>

#pragma comment(lib,"ws2_32.lib")

void main()
{
	int err;
	WORD versionRequired;
	WSADATA wsaData;                            //包含系统所支持的WinStock版本信息
	versionRequired = MAKEWORD(1, 1);           //初始化版本1.1
	//注册WinStock，返回状态
	err = WSAStartup(versionRequired, &wsaData);//协议库的版本信息
	if (!err)                                   //返回结果为0表示初始化失败
	{
		cout << LPSTR("客户端套接字已经打开!\n");
	}
	else
	{
		//调用WSAGetLastError()查看错误信息
		cout << ("客户端套接字打开失败：") << WSAGetLastError() << endl;
		return;//结束
	}
	/*
	创建套接字：
	流式套接字：   SOCK_STREAM , IPPROTO_TCP
	数据报套接字: SOCK_DGRAM  , IPPROTO_UDP
	*/
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    //创建流式套接字
	SOCKADDR_IN clientsock_in;                                          //专门针对Internet 通信域的Winsock地址结构
	clientsock_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");        //通过inet_addr结构指定套接字的主机IP地址 
	clientsock_in.sin_family = AF_INET;                                 //指定协议家族:AF_INET
	clientsock_in.sin_port = htons(3000);                               //指定将要分配给套接字的传输层端口号：6000

	int fail = connect(clientSocket, (SOCKADDR*)&clientsock_in, sizeof(SOCKADDR));//开始连接
	if (fail) {
		cout << "与服务端连接失败！程序将退出..." << endl;
		_getch();
		return;
	}
	string s;
	while (cin >> s) {
		char receiveBuf[100];
		//接收数据
		recv(clientSocket, receiveBuf, 101, 0);
		cout << receiveBuf << endl;
		//发送数据
		send(clientSocket, s.c_str(), s.length() + 1, 0);
		if (s == "quit") {
			break;
		}
	}
	closesocket(clientSocket);
	//关闭套接字
	if (WSACleanup() == SOCKET_ERROR) {
		cout << "套接字关闭失败：" << WSAGetLastError() << endl;
	}
	else {
		cout << "套接字成功关闭." << endl;
	}
	_getch();
	return;
}