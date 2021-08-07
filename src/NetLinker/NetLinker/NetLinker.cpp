#include "pch.h"
#include "NetLinker.h"
#include "Server.h"
#include "Client.h"

void* Server_Create(int nPort_)
{
	return new CServer(nPort_);
}

void Server_Delete(void* p_)
{
	delete p_;
}

bool Server_SendMsg(void* pServer_, const char* pMsg_)
{
	return ((CServer*)pServer_)->SendMsg(pMsg_);
}

bool Server_ReceiveMsg(void* pServer_, char* pMsg_, int nLen_)
{
	return ((CServer*)pServer_)->ReceiveMsg(pMsg_, nLen_);
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

bool Client_ReceiveMsg(void* pClient_, char* pMsg_, int nLen_)
{
	return ((CClient*)pClient_)->ReceiveMsg(pMsg_, nLen_);
}