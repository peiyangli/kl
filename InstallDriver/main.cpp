
#include "../sl/config.h"


#include <boost/filesystem.hpp>

#include <iostream>
#include <string>
#include <functional>


#include <windows.h>
namespace sc{


	class ScopeGuard
	{
	public:
		explicit ScopeGuard(std::function<void()> onExitScope)
			: onExitScope_(onExitScope), dismissed_(false)
		{ }

		~ScopeGuard()
		{
			if (!dismissed_)
			{
				onExitScope_();
			}
		}

		void Dismiss()
		{
			dismissed_ = true;
		}

	private:
		std::function<void()> onExitScope_;
		bool dismissed_;

	private: // noncopyable
		ScopeGuard(ScopeGuard const&);
		ScopeGuard& operator=(ScopeGuard const&);
	};



	BOOL LoadNTDriver(const wchar_t* lpszDriverName, const wchar_t* lpszDriverPath)
	{
		BOOL bRet = FALSE;
		SC_HANDLE hServiceMgr = NULL;//SCM管理器的句柄
		SC_HANDLE hServiceDDK = NULL;//NT驱动程序的服务句柄
		//打开服务控制管理器
		do
		{
			hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (hServiceMgr == NULL)   {
				break;
			}
			//创建驱动所对应的服务
			hServiceDDK = CreateService(hServiceMgr,
				lpszDriverName, //驱动程序的在注册表中的名字  
				lpszDriverName, // 注册表驱动程序的 DisplayName 值  
				SERVICE_ALL_ACCESS, // 加载驱动程序的访问权限  
				SERVICE_KERNEL_DRIVER,// 表示加载的服务是驱动程序  
				SERVICE_DEMAND_START, // 注册表驱动程序的 Start 值  
				SERVICE_ERROR_IGNORE, // 注册表驱动程序的 ErrorControl 值  
				lpszDriverPath, // 注册表驱动程序的 ImagePath 值  
				NULL,
				NULL,
				NULL,
				NULL,
				NULL);
			DWORD dwRtn;
			//判断服务是否失败
			if (hServiceDDK == NULL)
			{
				dwRtn = GetLastError();
				if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS) {
					break;
				}
				// 驱动程序已经加载，只需要打开  
				hServiceDDK = OpenService(hServiceMgr, lpszDriverName, SERVICE_ALL_ACCESS);
				if (hServiceDDK == NULL)  {
					//如果打开服务也失败，则意味错误
					dwRtn = GetLastError();
					break;
				}
			}
			//开启此项服务
			bRet = StartService(hServiceDDK, NULL, NULL);
			if (!bRet) {
				DWORD dwRtn = GetLastError();
				if (dwRtn != ERROR_SERVICE_ALREADY_RUNNING)
					break;
				bRet = TRUE;
				break;
			}

			// wait for startup
			DWORD timeOUt = 1000;
			SERVICE_STATUS status;
			auto startTick = GetTickCount();
			while (QueryServiceStatus(hServiceDDK, &status) == TRUE)
			{
				Sleep(status.dwWaitHint > 200 ? 200 : status.dwWaitHint);

				if (status.dwCurrentState == SERVICE_RUNNING)
				{
					bRet = true;
					break;
				}
				if (GetTickCount() - startTick > timeOUt){
					bRet = false; //?
					break;
				}
				if (0 == status.dwWaitHint)
				{
					bRet = false; //?
					break;
				}
			}

			bRet = TRUE;
		} while (false);

		if (hServiceDDK) {
			CloseServiceHandle(hServiceDDK);
		}
		if (hServiceMgr) {
			CloseServiceHandle(hServiceMgr);
		}
		return bRet;
	}

	//
	static BOOL StartSvr(const wchar_t * szSvrName)
	{
		SC_HANDLE m_sManager = NULL;
		SC_HANDLE m_sService = NULL;

		BOOL rt = FALSE;
		do
		{
			m_sManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (!m_sManager) {
				break;
			}

			m_sService = OpenService(m_sManager, szSvrName, SERVICE_ALL_ACCESS);
			if (NULL == m_sService) {
				break;
			}
			//Ready For Start Service
			if (!StartService(m_sService, 0, NULL) &&
				(ERROR_SERVICE_ALREADY_RUNNING != GetLastError())) {
				//ERROR_PATH_NOT_FOUND
				break;
			}

			rt = TRUE;
		} while (false);

		if (m_sService) CloseServiceHandle(m_sService);
		if (m_sManager) CloseServiceHandle(m_sManager);

		return rt;
	}


	static BOOL StopSvr(const wchar_t * szSvrName, DWORD dwTimeOut = 1000)
	{
		SC_HANDLE hServiceMgr = NULL, hServiceTwdm = NULL;
		SERVICE_STATUS_PROCESS SvrSta;

		BOOL rt = FALSE;
		do
		{
			hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (NULL == hServiceMgr) {
				break;
			}
			ScopeGuard guard([hServiceMgr](){
				CloseServiceHandle(hServiceMgr); //make sure to release this handle in case that exception occurred in callback-func and been caught out side of this function
			});
			hServiceTwdm = OpenService(hServiceMgr, szSvrName, SERVICE_ALL_ACCESS);
			if (hServiceTwdm == NULL)  {
				break;
			}
			ScopeGuard guard1([hServiceTwdm](){
				CloseServiceHandle(hServiceTwdm); //make sure to release this handle in case that exception occurred in callback-func and been caught out side of this function
			});


			DWORD dwBytesNeeded = 0;
			if (!QueryServiceStatusEx(
				hServiceTwdm,
				SC_STATUS_PROCESS_INFO,
				(LPBYTE)&SvrSta,
				sizeof(SERVICE_STATUS_PROCESS),
				&dwBytesNeeded)) {
				break;
			}
			if (SvrSta.dwCurrentState == SERVICE_STOPPED) {
				//already stop
				return TRUE;
			}
			//ControlService(hServiceTwdm, SERVICE_CONTROL_STOP, &SvrSta);
			//wait for a while

			auto dwStartTick = GetTickCount();
			//stop self
			if (!ControlService(
				hServiceTwdm,
				SERVICE_CONTROL_STOP,
				(LPSERVICE_STATUS)&SvrSta)) {
				break;
			}

			while (SvrSta.dwCurrentState != SERVICE_STOPPED)
			{
				Sleep(SvrSta.dwWaitHint > 200 ? 200 : SvrSta.dwWaitHint);
				if (!QueryServiceStatusEx(
					hServiceTwdm,
					SC_STATUS_PROCESS_INFO,
					(LPBYTE)&SvrSta,
					sizeof(SERVICE_STATUS_PROCESS),
					&dwBytesNeeded)) {
					return FALSE;
				}

				if (SvrSta.dwCurrentState == SERVICE_STOPPED){
					return TRUE;
				}

				if (GetTickCount() - dwStartTick > dwTimeOut) {
					return FALSE;
				}
			}

			rt = TRUE;
		} while (false);

		// 	if (hServiceTwdm) CloseServiceHandle(hServiceTwdm);
		// 	if (hServiceMgr) CloseServiceHandle(hServiceMgr);

		return rt;
	}
	/*
	* uninstall driver and service
	*/
	static BOOL DelSvr(const wchar_t * szSvrName)
	{
		SC_HANDLE hServiceMgr = NULL, hServiceTwdm = NULL;
		SERVICE_STATUS SvrSta;

		BOOL rt = FALSE;
		do
		{
			hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (NULL == hServiceMgr) {
				break;
			}
			hServiceTwdm = OpenService(hServiceMgr, szSvrName, SERVICE_ALL_ACCESS);
			if (hServiceTwdm == NULL)  {
				break;
			}

			ControlService(hServiceTwdm, SERVICE_CONTROL_STOP, &SvrSta);

			if (!DeleteService(hServiceTwdm))  {
				break;
			}

			rt = TRUE;
		} while (false);

		if (hServiceTwdm) CloseServiceHandle(hServiceTwdm);
		if (hServiceMgr) CloseServiceHandle(hServiceMgr);

		return rt;
	}

}

int main(int argc, char* argv[])
{
	auto& ins = ConfigCml::instance();

	boost::program_options::options_description od("Install NT Driver");
	od.add_options()
		("help,h", "help for command line")
		("install,i", "Install Mode")
		("uninstall,u", "uninstall Mode")
		("name,n", boost::program_options::value<std::string>(), "name of driver")
		("path-image,p", boost::program_options::value<std::string>(), "sys file image path");
	if (!ins.init(argc, argv, od)){
		return -1;
	}
	if (ins.has("help")){
		std::cout << od << std::endl;
		return 0;
	}

	if (ins.has("install")){
		auto name = ins.get<std::wstring>("name", L"");
		auto image = ins.get<std::wstring>("path-image", L"");
		if (name.empty() || image.empty()){
			std::cout << od << std::endl << std::endl;

			std::cout << "Invalid name or image" << std::endl;
			return -1;
		}
		if (!sc::LoadNTDriver(name.c_str(), image.c_str())){
			std::cout << "Failed to load service" << std::endl;
			return -1;
		}

		std::cout << "install ok" << std::endl;
	}
	else if (ins.has("uninstall")){
		auto name = ins.get<std::wstring>("name", L"");
		if (name.empty()){
			std::cout << od << std::endl << std::endl;
			std::cout << "Invalid name" << std::endl;
			return -1;
		}
		if (!sc::StopSvr(name.c_str(), 0xffffffff)){
			std::cout << "Failed to stop service" << std::endl;
			return -1;
		}

		if (!sc::DelSvr(name.c_str())){
			std::cout << "Failed to delete service" << std::endl;
			return -1;
		}

		std::cout << "uninstall ok" << std::endl;
	}
	else{
		std::cout << od << std::endl;
		return -1;
	}

	return 0;
}