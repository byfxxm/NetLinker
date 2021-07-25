#include "stdafx.h"
#include "NetLinker.h"
#include "NetLinerImp.h"


void* NetLinker_Create()
{
	return new NetLinerImp();
}

void NetLinker_Delete(void* pNetLinker_)
{
	delete (NetLinerImp*)pNetLinker_;
}