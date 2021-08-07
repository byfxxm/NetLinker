#include "pch.h"
#include "NetLinker.h"
#include "Server.h"
#include "Client.h"

void* Server_Create()
{
	return new CServer();
}

void Server_Delete(void* p_)
{
	delete p_;
}

void Server_Listen(void* pServer_, int nPort_)
{
	return ((CServer*)pServer_)->Listen(nPort_);
}

void* Client_Create()
{
	return new CClient();
}

void Client_Delete(void* p_)
{
	delete p_;
}

bool Client_Connect(void* pClient_, const char* pAddr_, int nPort_)
{
	return ((CClient*)pClient_)->Connect(pAddr_, nPort_);
}

bool Client_SendMsg(void* pClient_, const char* pMsg_)
{
	return ((CClient*)pClient_)->SendMsg(pMsg_);
}

bool Client_SendFile(void* pClient_, const char* pFile_)
{
	return ((CClient*)pClient_)->SendFile(pFile_);
}