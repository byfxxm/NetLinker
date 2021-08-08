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
	delete (CServer*)p_;
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
	delete (CClient*)p_;
}

bool Client_Connect(void* pClient_, const char* pAddr_, int nPort_)
{
	return ((CClient*)pClient_)->Connect(pAddr_, nPort_);
}

bool Client_SendMsg(void* pClient_, const char* pMsg_)
{
	auto _p = (CClient*)pClient_;
	return _p->SendMsg(_p->GetSocket(), pMsg_);
}

bool Client_SendFile(void* pClient_, const char* pFile_)
{
	auto _p = (CClient*)pClient_;
	return _p->SendFile(_p->GetSocket(), pFile_);
}