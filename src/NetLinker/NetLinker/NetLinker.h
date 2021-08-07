#pragma once

#ifdef NETLINKER_EXPORTS
#define NETLINKER_API extern "C" __declspec(dllexport)
#else
#define NETLINKER_API extern "C" __declspec(dllimport)
#endif

NETLINKER_API void* Server_Create();
NETLINKER_API void Server_Delete(void* pServer_);
NETLINKER_API void Server_Listen(void* pServer_, int nPort_);

NETLINKER_API void* Client_Create();
NETLINKER_API void Client_Delete(void* pClient_);
NETLINKER_API bool Client_Connect(void* pClient_, const char* pAddr_, int nPort_);
NETLINKER_API bool Client_SendMsg(void* pClient_, const char* pMsg_);
NETLINKER_API bool Client_SendFile(void* pClient_, const char* pFile_);