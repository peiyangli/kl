#include "AbstractServiceApp.h"

#include"../Debug/dbgonly.hpp"



#include <Windows.h>
#pragma comment(lib, "Advapi32.lib")

#ifndef DEF_SERVICE_NAME
#define DEF_SERVICE_NAME L""
#endif



////////////////////////////////////////////////////////////////
#include "../sl/config.h"
class ServiceHandler
{
public:
	static std::string unicodeToMultibyte(std::wstring utf8)
	{
		using wcmb = std::codecvt_byname < wchar_t, char, std::mbstate_t >;
		static std::wstring_convert<wcmb> conv(new wcmb(""));
		return conv.to_bytes(utf8);
	}


	static void __stdcall ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
	{
		_hSerivceStatusHandle = RegisterServiceCtrlHandler(DEF_SERVICE_NAME, (LPHANDLER_FUNCTION)ServiceCtrlHandler);
		if (!_hSerivceStatusHandle) {
			return;
		}
		//cml args
		std::vector<std::string> args;
		std::vector<LPCSTR> tmpArgs;
		for (int i = 0; i < dwArgc; ++i){
			args.push_back(unicodeToMultibyte(lpszArgv[i]));
		}
		LPCSTR * lpServiceArgVectors = NULL;
		if (dwArgc > 0){
			for (auto& arg : args){
				tmpArgs.push_back(arg.c_str());
			}
			lpServiceArgVectors = tmpArgs.data();
		}
		if (_appServiceHandler){
			if (lpServiceArgVectors){
				_appServiceHandler->cml(dwArgc, lpServiceArgVectors);
			}
			
			if (_appServiceHandler->beforeRunning()){
				UpdateServiceStatus(SERVICE_RUNNING, 0, 0, 0, 0);
				_appServiceHandler->execute();
			}
		}

		UpdateServiceStatus(SERVICE_STOPPED, 0, 0, 0, 0);
		return;
	}

	static void ServiceCtrlHandler(DWORD nControlCode)
	{
		DbgPrtFileLineFunctionVar(nControlCode);
		switch (nControlCode)
		{
		case SERVICE_CONTROL_STOP:
			DbgPrtFileLineFunction;
			UpdateServiceStatus(SERVICE_STOPPED, 0, 0, 0, 0);
			break;
		case SERVICE_CONTROL_PARAMCHANGE:
		{
			//DbgPrtFileLineFunction;
			//LPWSTR *szArgList;
			//int argCount;
			//szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
			//if (szArgList)
			//{
			//	for (int i = 0; i < argCount; i++)
			//	{
			//		DbgBfPrt((DbgTag L"[%1%(%2%)-%3%]main %4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % szArgList[i]);
			//	}
			//	LocalFree(szArgList);
			//}
		}
			break;
		}
	}


	static BOOL UpdateServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwServiceSpecificExitCode, DWORD dwCheckPoint, DWORD dwWaitHint)
	{
		BOOL success = TRUE;
		SERVICE_STATUS nServiceStatus;

		nServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
		nServiceStatus.dwCurrentState = dwCurrentState;

		if (dwCurrentState == SERVICE_START_PENDING) {
			nServiceStatus.dwControlsAccepted = 0;
		}
		else {
			nServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
		}

		if (dwServiceSpecificExitCode == 0) {
			nServiceStatus.dwWin32ExitCode = dwWin32ExitCode;
		}
		else {
			nServiceStatus.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
		}

		nServiceStatus.dwServiceSpecificExitCode = dwServiceSpecificExitCode;
		nServiceStatus.dwCheckPoint = dwCheckPoint;
		nServiceStatus.dwWaitHint = dwWaitHint;

		if (_hSerivceStatusHandle) {
			success = SetServiceStatus(_hSerivceStatusHandle, &nServiceStatus);
			if (!success) {
			}
		}
		return success;
	}

	static SERVICE_STATUS_HANDLE _hSerivceStatusHandle;
	static AbstractServiceApp*   _appServiceHandler;
};

SERVICE_STATUS_HANDLE ServiceHandler::_hSerivceStatusHandle = 0;
AbstractServiceApp* ServiceHandler::_appServiceHandler = 0;

/////////////////////////////////////////////////////////////////
AbstractServiceApp::AbstractServiceApp()
{
}

bool AbstractServiceApp::run(RunType t)
{
	_runType = t;
	switch (t)
	{
	case RT_SERVICE:
	{
		SERVICE_TABLE_ENTRY service_table[] = {
			{ DEF_SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceHandler::ServiceMain }, // L"" only for SERVICE_WIN32_OWN_PROCESS   //  serviceMain
			{ NULL, NULL }
		};

		ServiceHandler::_appServiceHandler = this;

		BOOL success = StartServiceCtrlDispatcher(service_table);
		return success == TRUE;
	}
	break;
	case RT_CONSOLE:
	{
		cml(__argc, __argv);
		return 0 == execute();
	}
	break;
	default:
		break;
	}
	return false;
}
bool AbstractServiceApp::beforeRunning()
{
	return true;
}

int AbstractServiceApp::execute()
{
	return 0;
}
