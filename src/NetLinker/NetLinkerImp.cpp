#include "stdafx.h"
#include "NetLinerImp.h"

NetLinerImp::NetLinerImp()
{
	WSADATA _WsaData;
	WSAStartup(1, &m_WsaData);
}

NetLinerImp::~NetLinerImp()
{

}