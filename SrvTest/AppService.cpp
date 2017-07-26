//this project
#include "AppService.h"

//solution libs
#include"../sl/config.h"
#include"../sl/ServiceOperater.h"

#include"../Debug/dbgonly.hpp"
#include"../sl/Registry.h"
#include"../sl/WinUtl.h"
#include"../dl/TestExport.h"
//third libs


//system


#define SrvTestName L"SrvTest"
#define SrvTestName2 L"SrvTest2"


#define REG_PATH_THREADMSG_TID L"HKEY_LOCAL_MACHINE\\Software\\Anvisoft\\Asd2\\Service"
#define REG_KEYNAME_THREADMSG_TID L"tid"

namespace{

	std::wstring GetCurrentProcessPath(){
		wchar_t buf[MAX_PATH] = {};
		auto len = GetModuleFileName(NULL, buf, MAX_PATH);
		if (len > 0 && len < MAX_PATH)
			return buf;

		return L"";
	}



	BOOL EnablePriv(HANDLE handle)
	{
		HANDLE hToken;
		LUID sedebugnameValue;
		TOKEN_PRIVILEGES tkp;
		if (!OpenProcessToken(handle, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			DbgBfPrt((DbgTag L"%1%(%2%):%3%, Error: %4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % GetLastError());
			return FALSE;
		}
		if (!LookupPrivilegeValue(NULL, SE_TCB_NAME, &sedebugnameValue))
		{
			CloseHandle(hToken);
			DbgBfPrt((DbgTag L"%1%(%2%):%3%, Error: %4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % GetLastError());
			return FALSE;
		}
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = sedebugnameValue;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
		{
			CloseHandle(hToken);
			DbgBfPrt((DbgTag L"%1%(%2%):%3%, Error: %4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % GetLastError());
			return FALSE;
		}
		DbgBfPrt((DbgTag L"%1%(%2%):%3%, Error: %4%-%5%") % __FILENAME__ % __LINE__ % __FUNCTION__ % sedebugnameValue.HighPart % sedebugnameValue.LowPart);
		return TRUE;
	}

	bool PostMessageToWorkingThread(UINT msg, WPARAM wp){

		//Registry reg(REG_PATH_THREADMSG_TID);
		//auto votid = reg.value(REG_KEYNAME_THREADMSG_TID, (unsigned long long)0);
		//bool ok = false;
		//auto otid = votid.To<unsigned long long>(0, &ok);
		//if (!ok || otid < 5)return false;
		auto otid = STGetTID();
		if (otid < 5)return false;

		return TRUE == ::PostThreadMessage(otid, msg, wp, 0);
	}

}




AppService::AppService()
{
	
}

bool AppService::cml(int argc, const char* const* argv)
{
	ConfigCml& cml = ConfigCml::instance();
	boost::program_options::options_description od("for test only!!!");
	od.add_options()
		("console", "console mode")
		("install", "install mode")
		("worker", "worker mode")
		("pri_pid", boost::program_options::value<int64_t>(), "process Pid")
		("msg_id", boost::program_options::value<int64_t>()->default_value(0), "")
		("msg_param", boost::program_options::value<int64_t>()->default_value(0), "")
		("version", boost::program_options::value<std::string>()->default_value("1.0.0"), "version of")
		;
	return cml.init(argc, argv, od);
}

bool PostMessageBySrv(UINT msg, WPARAM wp)
{
	ServiceOperater so;
	std::vector<std::wstring> args;
	args.push_back((boost::wformat(L"--msg_id=%1%") % msg).str());
	args.push_back((boost::wformat(L"--msg_param=%1%") % (int64_t)wp).str());
	auto ok = so.start(SrvTestName, args);
	return ok;
}
int AppService::execute()
{
	ConfigCml& cml = ConfigCml::instance();
	if (runType() == AbstractServiceApp::RT_CONSOLE){
		if (cml.has("install")){
			auto cp = GetCurrentProcessPath();
			if (!cp.empty()){
				ServiceOperater so;
				auto ok = so.create(SrvTestName, cp);
				DbgBfPrt((DbgTag L"[%1%(%2%)-%3%]Create Service %4%(%5%)") % __FILENAME__ % __LINE__ % __FUNCTION__% cp % ok);
				ok = so.create(SrvTestName2, cp);
				DbgBfPrt((DbgTag L"[%1%(%2%)-%3%]Create Service %4%(%5%)") % __FILENAME__ % __LINE__ % __FUNCTION__% cp % ok);
			}
			return 0;
		}

		ServiceOperater so;

		//PostMessageToWorkingThread(8, 2);

		std::vector<std::wstring> args;
		////args.push_back((boost::wformat(L"--pri_pid=%1%") % GetCurrentThreadId()).str());
		//args.push_back((boost::wformat(L"--pri_pid=%1%")%GetCurrentProcessId()).str());
		//auto ok = so.start(SrvTestName, args);

		//Sleep(2000);
		//LUID sedebugnameValue;
		//LookupPrivilegeValue(NULL, SE_TCB_NAME, &sedebugnameValue);
		//DbgBfPrt((DbgTag L"%1%(%2%):%3%, Error: %4%-%5%") % __FILENAME__ % __LINE__ % __FUNCTION__ % sedebugnameValue.HighPart % sedebugnameValue.LowPart);
		//PostMessageToWorkingThread(9, 2);


		//DbgBfPrt((DbgTag L"[%1%(%2%)-%3%]PostMessageToWorkingThread %4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % GetLastError());

		args.clear();
		args.push_back(L"--worker");
		//args.push_back(L"--msg_id=23432");
		auto ok = so.start(SrvTestName2, args);

		auto msg_param = cml.get<int64_t>("msg_param", (int64_t)24);

		TestClass tc;
		auto v = tc.getValue();
		PostMessageBySrv(UM_TSRV_PRINT_PARAM, 0);
		PostMessageBySrv(UM_TSRV_SETVALUE_PARAM, msg_param);
		v = tc.getValue();
		PostMessageBySrv(UM_TSRV_PRINT_PARAM, 0);

		v = STGetValue();

	}
	else{
		//auto cp = GetCurrentProcessPath();
		//auto pri_pid = cml.get<int64_t>("pri_pid", (int64_t)0);
		//
		//DbgPrtFileLineFunction;
		if (cml.has("worker")){
			//SrvTestName2
			DbgPrtFileLineFunctionVar(L"srv worker");
			{
				auto Tid = GetCurrentThreadId();

				STSetTID(Tid);
				//set id
				Registry reg(REG_PATH_THREADMSG_TID);
				reg.setValue(REG_KEYNAME_THREADMSG_TID, (unsigned long long)Tid);
			}

			auto testTimerId = SetTimer(NULL, 0x1111, 20000, NULL);
			MSG msg;
			PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
			while (GetMessage(&msg, NULL, 0, 0))
			{
				switch (msg.message)
				{
				case WM_TIMER:
					if (msg.wParam == testTimerId){
						DbgPrtFileLineFunctionVar(L"Service Test Timer.");
					}
					break;
				case UM_TSRV_PRINT_PARAM:
				{
					DbgPrtFileLineFunctionVar(TestClass().getValue());
				}
					break;
				case UM_TSRV_SETVALUE_PARAM:
				{
					TestClass tc;
					tc.setValue(msg.wParam);
					DbgPrtFileLineFunctionVar(msg.wParam);
				}
					break;
				default:
					break;
				}
				//DbgPrtFileLineFunctionVar(msg.message);
			}
			DbgPrtFileLineFunction;
		}
		else{
			//SrvTestName
		}
	}
	return 0;
}
bool AppService::beforeRunning()
{
	ConfigCml& cml = ConfigCml::instance();
	//auto cmls = cml.getContent();
	//for (auto& c : cmls){
	//	DbgBfPrt((DbgTag L"[%1%(%2%)-%3%]arg-%4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % c.first.c_str());
	//}

	if (cml.has("worker")){
		//SrvTestName2
		DbgPrtFileLineFunction;
	}
	else{
		//SrvTestName
		auto cp = GetCurrentProcessPath();


		auto msg_id = cml.get<int64_t>("msg_id", (int64_t)0);
		if (msg_id > 0){
			auto msg_param = cml.get<int64_t>("msg_param", (int64_t)0);

			PostMessageToWorkingThread(msg_id, msg_param);

			//DbgPrtFileLineFunctionVar(L"PostMessageToWorkingThread");
			DbgPrtFileLineFunctionVar(TestClass().getValue());
		}


		//auto pri_pid = cml.get<int64_t>("pri_pid", (int64_t)0);
		//if (pri_pid > 0){
		//	DbgBfPrt((DbgTag L"[%1%(%2%)-%3%]pri_pid-%4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % pri_pid);
		//	//opt::CHandle hOpen(OpenProcess(PROCESS_ALL_ACCESS|PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pri_pid));
		//	opt::CHandle hOpen(OpenThread(PROCESS_ALL_ACCESS | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pri_pid));
		//	if (NULL != hOpen) {
		//		auto ok = EnablePriv(hOpen);

		//		DbgBfPrt((DbgTag L"[%1%(%2%)-%3%]EnableDebugPrivilege-%4%") % __FILENAME__ % __LINE__ % __FUNCTION__ % ok);
		//	}
		//}
		//else{

		//}
		//PostMessageToWorkingThread(8, 2);
		return false;
	}
	return true;
}