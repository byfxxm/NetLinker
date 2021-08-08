#pragma once
#pragma warning(disable:4200)

struct Pack
{
	Pack(int nLen_ = 0) : nDataLen(nLen_) {}
	int nDataLen;
	char Data[0];
};

#define PACKHEAD sizeof(Pack)