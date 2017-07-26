#include <windows.h>


#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
// 
// #include"../sl/Registry.h"
// #include"../Debug/dbgonly.hpp"
// 
// 
// 
// #define REG_PATH_THREADMSG_TID L"HKEY_LOCAL_MACHINE\\Software\\Anvisoft\\Asd2\\Service"
// #define REG_KEYNAME_THREADMSG_TID L"tid"
// #define MSG_MY WM_USER+1
// 
// 
// UINT g_msg = RegisterWindowMessage(L"ASD_MSG_FOR_THREAD_COM");
// 
// BOOL PostMessageToWorkingThread(UINT msg, WPARAM wp){
// 
// 	Registry reg(REG_PATH_THREADMSG_TID);
// 	auto votid = reg.value(REG_KEYNAME_THREADMSG_TID, (unsigned long long)0);
// 	bool ok = FALSE;
// 	auto otid = votid.To<unsigned long long>(0, &ok);
// 	if (!ok || otid < 5)return FALSE;
// 
// 	return TRUE == ::PostThreadMessage(otid, g_msg, msg, wp);
// }
// 
// BOOL BrMessageToWorkingThread(UINT msg, WPARAM wp){
// 
// 	Registry reg(REG_PATH_THREADMSG_TID);
// 	auto votid = reg.value(REG_KEYNAME_THREADMSG_TID, (unsigned long long)0);
// 	bool ok = FALSE;
// 	auto otid = votid.To<unsigned long long>(0, &ok);
// 	if (!ok || otid < 5)return FALSE;
// 
// 	//BSF_IGNORECURRENTTASK
// 	//BSM_ALLCOMPONENTS
// 	DWORD dwRct = BSM_ALLCOMPONENTS;// BSM_APPLICATIONS;
// 	return ::BroadcastSystemMessage(BSF_POSTMESSAGE, &dwRct, g_msg, msg, wp) > 0;
// }
// 
// BOOL EnableDebugPrivilege()
// {
// 	HANDLE hToken;
// 	LUID sedebugnameValue;
// 	TOKEN_PRIVILEGES tkp;
// 	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
// 	{
// 		return FALSE;
// 	}
// 	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
// 	{
// 		CloseHandle(hToken);
// 		return FALSE;
// 	}
// 	tkp.PrivilegeCount = 1;
// 	tkp.Privileges[0].Luid = sedebugnameValue;
// 	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
// 	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
// 	{
// 		CloseHandle(hToken);
// 		return FALSE;
// 	}
// 	return TRUE;
// }
// 
// BOOL EnablePriv()
// {
// 	HANDLE hToken;
// 	LUID sedebugnameValue;
// 	TOKEN_PRIVILEGES tkp;
// 	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
// 	{
// 		DbgBfPrt((DbgTag L"%1%(%2%):%3%, Error: %4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % GetLastError());
// 		return FALSE;
// 	}
// 	if (!LookupPrivilegeValue(NULL, SE_TCB_NAME, &sedebugnameValue))
// 	{
// 		CloseHandle(hToken);
// 		DbgBfPrt((DbgTag L"%1%(%2%):%3%, Error: %4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % GetLastError());
// 		return FALSE;
// 	}
// 	tkp.PrivilegeCount = 1;
// 	tkp.Privileges[0].Luid = sedebugnameValue;
// 	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
// 	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
// 	{
// 		CloseHandle(hToken);
// 		DbgBfPrt((DbgTag L"%1%(%2%):%3%, Error: %4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % GetLastError());
// 		return FALSE;
// 	}
// 	DbgBfPrt((DbgTag L"%1%(%2%):%3%, Error: %4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % GetLastError());
// 	return TRUE;
// }
//
//
//BOOL EnablePriv()
//{
//	HANDLE hToken;
//	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
//	{
//		DbgPrtFileLineFunction;
//
//		TOKEN_PRIVILEGES tkp;
//
//		LookupPrivilegeValue(NULL, SE_TCB_NAME, &tkp.Privileges[0].Luid);//修改进程权限
//
//		DbgBfPrt((DbgTag L"%1%(%2%):%3%, Error: %4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % GetLastError());
//		tkp.PrivilegeCount = 1;
//		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
//		AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL);//通知系统修改进程权限
//		DbgBfPrt((DbgTag L"%1%(%2%):%3%, Error: %4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % GetLastError());
//		return((GetLastError() == ERROR_SUCCESS));
//	}
//	return TRUE;
//}

extern "C" int sum(int a, int b);
#pragma comment(lib, "b.a")
int main(){

	auto fff = sum(12, 14);

// 	auto ok = PostMessageToWorkingThread(MSG_MY, 1);
// 
// 	DbgBfPrt((DbgTag L"%1%(%2%):%3%, Error: %5%-%4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % GetLastError() % ok);
// 
// 	getchar();
// 	//EnableDebugPrivilege();
// 	//ok = EnablePriv();
// 	//DbgBfPrt((DbgTag L"%1%(%2%):%3%, Error: %5%-%4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % GetLastError() % ok);
// 	system("pause");
// 	ok = BrMessageToWorkingThread(MSG_MY + 1, 2);
// 
// 	DbgBfPrt((DbgTag L"%1%(%2%):%3%, Error: %5%-%4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % GetLastError() % ok);

	return 0;
}