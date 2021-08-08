#include "pch.h"
#include "Socket.h"
#include "Protocol.h"

CSocket::CSocket()
{
}

CSocket::~CSocket()
{
}

bool CSocket::SendMsg(SOCKET socket_, const char* pMsg_)
{
	return SendBytes(socket_, pMsg_, strlen(pMsg_) + 1);
}

bool CSocket::SendBytes(SOCKET socket_, const char* pBytes_, int nLen_)
{
	auto _pMem = new char[PACKHEAD + nLen_];
	auto _pPack = new(_pMem) Pack(nLen_);
	memcpy(_pPack->Data, pBytes_, nLen_);

	bool _bRet = (SOCKET_ERROR != send(socket_, _pMem, PACKHEAD + nLen_, 0));
	delete[] _pMem;

	return _bRet;
}

bool CSocket::SendFile(SOCKET socket_, const char* pFile_)
{
	ifstream _fin(pFile_, ios::binary);
	if (!_fin.is_open())
		return false;

	char _file[MAX_PATH];
	strcpy_s(_file, pFile_);
	char* _name = strrchr(_file, '\\');
	_name++;

	char _str[1024] = { 0 };
	sprintf_s(_str, MASK "%s", _name);
	SendMsg(socket_, _str);

	_fin.seekg(0, ios::end);
	sprintf_s(_str, MASK FILELEN "%llu", (unsigned long long)_fin.tellg());
	SendMsg(socket_, _str);
	_fin.seekg(0, ios::beg);

	char _buff[BUFFER_SIZE] = { 0 };
	while (true)
	{
		memcpy(_buff, MASK, MASK_SIZE);
		_fin.read(_buff + MASK_SIZE, sizeof(_buff) - MASK_SIZE - PACKHEAD);
		SendBytes(socket_, _buff, (int)_fin.gcount() + MASK_SIZE);

		if (_fin.eof())
		{
			SendBytes(socket_, MASK EOFILE, MASK_SIZE + EOFILE_SIZE);
			break;
		}
	}

	//m_Cond.wait(_lck);
	return true;
}

bool CSocket::RecvBytes(SOCKET socket_, char* pBuf_, int nLen_)
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
	if (!RecvBytes(socket_, (char*)pPack_, PACKHEAD))
		return false;

	if (!RecvBytes(socket_, pPack_->Data, pPack_->nDataLen))
		return false;

	return true;
}

bool CSocket::RecvFile(SOCKET socket_, Pack* pPack_)
{
	return Recv(socket_, pPack_);
}