// dllmain.cpp : 定义 DLL 应用程序的入口点。
//#include "stdafx.h"

#include "../Debug/dbgonly.hpp"

#include <windows.h>


#pragma data_seg("SecRWS")
volatile int gValue = 0;
volatile DWORD gTID = 0;
#pragma data_seg()
#pragma comment(linker,"/section:SecRWS,RWS")


#include "TestExport.h"
int STGetValue()
{
	return gValue;
}

DWORD STSetTID(DWORD v)
{
	auto temp = gTID;
	gTID = v;
	return temp;
}
DWORD STGetTID()
{
	return gTID;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//DbgPrtFileLineFunctionVar();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

