#pragma once


#ifdef NETLINKER_EXPORTS
#define NETLINKER_API extern "C" __declspec(dllexport)
#else
#define NETLINKER_API extern "C" __declspec(dllimport)
#endif

NETLINKER_API void* NetLinker_Create();
NETLINKER_API void NetLinker_Delete(void* pNetLinker_);
