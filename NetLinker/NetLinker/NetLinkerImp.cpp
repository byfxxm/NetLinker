#include "stdafx.h"
#include "NetLinerImp.h"

NetLinerImp::NetLinerImp()
{
	WSAStartup(1, &m_WsaData);
}

NetLinerImp::~NetLinerImp()
{

}