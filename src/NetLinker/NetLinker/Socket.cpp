#include "pch.h"
#include "Socket.h"
#include "Protocol.h"

CSocket::CSocket()
{
}

CSocket::~CSocket()
{
}

bool CSocket::Send(SOCKET socket_, const char* pBytes_, int nLen_)
{
	auto _pMem = new char[PACKHEAD + nLen_];
	auto _pPack = new(_pMem) Pack(nLen_);
	memcpy(_pPack->Data, pBytes_, nLen_);

	bool _bRet = Send(socket_, _pPack);
	delete[] _pMem;

	return _bRet;
}

bool CSocket::Send(SOCKET socket_, Pack* pPack_)
{
	if (SOCKET_ERROR == send(socket_, (char*)pPack_, PACKHEAD + pPack_->nDataLen, 0))
		return false;

	return true;
}

bool CSocket::Recv(SOCKET socket_, char* pBuf_, int nLen_)
{
	int _sum = nLen_;

	while (_sum > 0)
	{
		int _count = recv(socket_, pBuf_, _sum, 0);
		if (SOCKET_ERROR == _count)
			return false;

		pBuf_ += _count;
		_sum -= _count;
	}

	return _sum == 0;
}

bool CSocket::Recv(SOCKET socket_, Pack* pPack_)
{
	if (!Recv(socket_, (char*)pPack_, PACKHEAD))
		return false;

	if (!Recv(socket_, pPack_->Data, pPack_->nDataLen))
		return false;

	return true;
}