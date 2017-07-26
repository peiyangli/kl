#include "ServiceOperater.h"



ServiceOperater::ServiceOperater() :
_hscm(0)
{
	open();
}

ServiceOperater::~ServiceOperater()
{
	close();
}

bool ServiceOperater::open()
{
	if (_hscm)
		return true;
	_hscm = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
	return _hscm != 0;
}

void ServiceOperater::close()
{
	if (_hscm)
		CloseServiceHandle(_hscm);
	_hscm = 0;
}

bool ServiceOperater::remove(const std::wstring& name)
{
	if (false == assertOpen() || name.empty())
		return false;

	bool bRet = false;
	SC_HANDLE hSvc = OpenService(_hscm,
		reinterpret_cast<const WCHAR *>(name.c_str()),
		SERVICE_ALL_ACCESS);
	if (!hSvc){
		return false;
	}

	bRet = (DeleteService(hSvc) == TRUE);

	CloseServiceHandle(hSvc);
	return bRet;
}

bool ServiceOperater::create(const std::wstring &name, const std::wstring &binPath, const std::wstring &disName, const std::wstring &description, ServiceStartType t)
{
	if (false == assertOpen())
		return false;

	SC_HANDLE hSvc = CreateService(
		_hscm,
		reinterpret_cast<const wchar_t*>(name.c_str()),
		reinterpret_cast<const wchar_t*>(disName.c_str()),
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		t, SERVICE_ERROR_NORMAL,
		reinterpret_cast<const wchar_t*>(binPath.c_str()),
		NULL, NULL, NULL, NULL, NULL);
	if (hSvc == NULL){
		return ERROR_SERVICE_EXISTS == GetLastError();
	}
	if (description.length() > 0){
		std::wstring des = description;
		SERVICE_DESCRIPTION info = {};
		info.lpDescription = (LPWSTR)des.c_str(); //no err?
		ChangeServiceConfig2(hSvc, SERVICE_CONFIG_DESCRIPTION, &info);
	}

	CloseServiceHandle(hSvc);

	return true;
}

bool ServiceOperater::stop(const std::wstring& name)
{
	if (false == assertOpen() || name.empty())
		return false;

	bool bRet = false;
	SC_HANDLE hSvc = OpenService(_hscm,
		reinterpret_cast<const WCHAR *>(name.c_str()),
		SERVICE_ALL_ACCESS);
	if (!hSvc){
		return false;
	}
	SERVICE_STATUS status;
	if (!ControlService(hSvc, SERVICE_CONTROL_STOP, &status)){
		if (ERROR_SERVICE_NOT_ACTIVE != GetLastError()){
			bRet = false;
		}
	}
	CloseServiceHandle(hSvc);
	return bRet;
}

bool ServiceOperater::control(const std::wstring& name, unsigned code)
{
	if (false == assertOpen() || name.empty())
		return false;

	bool bRet = false;
	SC_HANDLE hSvc = OpenService(_hscm,
		reinterpret_cast<const WCHAR *>(name.c_str()),
		SERVICE_ALL_ACCESS); //SERVICE_USER_DEFINED_CONTROL
	if (!hSvc){
		return false;
	}
	SERVICE_STATUS status;
	if (!ControlService(hSvc, code, &status)){
		if (ERROR_SERVICE_NOT_ACTIVE != GetLastError()){
			bRet = false;
		}
	}
	CloseServiceHandle(hSvc);
	return bRet;
}

bool ServiceOperater::start(const std::wstring& name)
{
	if (false == assertOpen() || name.empty())
		return false;

	bool bRet = false;
	SC_HANDLE hSvc = OpenService(_hscm,
		reinterpret_cast<const WCHAR *>(name.c_str()),
		SERVICE_ALL_ACCESS);
	if (!hSvc){
		return false;
	}


	ServiceConfigInfo conf(name);
	if (!queryConfigInfo(hSvc, conf)){
		CloseServiceHandle(hSvc);
		return false;
	}

	if (conf.startType() == SST_DISABLE){
		//change to demand
		if (!changeStartType(hSvc, SST_DEMAND)){
			CloseServiceHandle(hSvc);
			return false;
		}
	}
	DWORD     dwNumServiceArgs = 0;
	LPCTSTR * lpServiceArgVectors = NULL;
	if (!StartService(hSvc, dwNumServiceArgs, lpServiceArgVectors)){
		if (ERROR_SERVICE_ALREADY_RUNNING != GetLastError()){
			bRet = true;
		}
	}
	else {
		// wait for startup
		SERVICE_STATUS status;
		while (QueryServiceStatus(hSvc, &status) == TRUE)
		{
			Sleep(status.dwWaitHint);

			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				return true;
			}
			if (0 == status.dwWaitHint)
			{
				return false;
			}
		}
	}
	return bRet;
}

bool ServiceOperater::start(const std::wstring& name, const std::vector<std::wstring>& args)
{
	if (false == assertOpen() || name.empty())
		return false;

	bool bRet = false;
	SC_HANDLE hSvc = OpenService(_hscm,
		reinterpret_cast<const WCHAR *>(name.c_str()),
		SERVICE_ALL_ACCESS);
	if (!hSvc){
		return false;
	}


	ServiceConfigInfo conf(name);
	if (!queryConfigInfo(hSvc, conf)){
		CloseServiceHandle(hSvc);
		return false;
	}

	if (conf.startType() == SST_DISABLE){
		//change to demand
		if (!changeStartType(hSvc, SST_DEMAND)){
			CloseServiceHandle(hSvc);
			return false;
		}
	}
	DWORD     dwNumServiceArgs = 0;
	LPCTSTR * lpServiceArgVectors = NULL;
	std::vector<LPCTSTR> tmpArgs;
	if (args.size() > 0){
		for (auto& arg : args){
			tmpArgs.push_back(arg.c_str());
			dwNumServiceArgs++;
		}

		lpServiceArgVectors = tmpArgs.data();
	}
	if (0==StartService(hSvc, dwNumServiceArgs, lpServiceArgVectors)){
		return false;
	}
	return true;
}

bool ServiceOperater::queryStartType(const std::wstring& name, __out ServiceStartType& o)
{
	return false;
}
bool ServiceOperater::queryBinPath(const std::wstring& name, __out std::wstring& o)
{
	return false;
}
bool ServiceOperater::queryDisplayName(const std::wstring& name, __out std::wstring& o)
{
	return false;
}

bool ServiceOperater::queryConfigInfo(SC_HANDLE hSvc, __out ServiceConfigInfo &o)
{
	if (!hSvc)
		return false;
	LPQUERY_SERVICE_CONFIG pConf = 0;
	bool brt = false;
	DWORD dwSvcLen = 0;
	DWORD dwSvcLenR = 0;
	if (!QueryServiceConfig(hSvc, NULL, 0, &dwSvcLen)) {
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {//
			dwSvcLenR = dwSvcLen;
			pConf = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LMEM_FIXED, dwSvcLen);
		}
		else {
			goto F_EXIT;
		}
	}
	if (!QueryServiceConfig(hSvc, pConf, dwSvcLenR, &dwSvcLen)){
		goto F_EXIT;
	}

	if (SERVICE_KERNEL_DRIVER == pConf->dwServiceType){
		goto F_EXIT;
	}
	o.setBinPath(std::wstring(pConf->lpBinaryPathName));
	o.setsStartType(pConf->dwStartType);
	o.setDisplayName(std::wstring(pConf->lpDisplayName));
	brt = true;
F_EXIT:
	if (pConf){
		LocalFree(pConf);
		pConf = NULL;
	}
	return brt;
}

bool ServiceOperater::queryConfigInfo(__in __out ServiceConfigInfo& o)
{
	if (false == assertOpen() || o.name().empty())
		return false;

	SC_HANDLE hSvc = OpenService(_hscm,
		reinterpret_cast<const WCHAR *>(o.name().c_str()),
		SERVICE_ALL_ACCESS);
	if (NULL == hSvc)
		return false;
	bool brt = queryConfigInfo(hSvc, o);
	CloseServiceHandle(hSvc);
	return brt;
}

bool ServiceOperater::queryDescription(SC_HANDLE hSvc, __out std::wstring& o)
{
	if (!hSvc)
		return false;

	bool brt = false;
	wchar_t *pDesData = NULL;
	LPSERVICE_DESCRIPTION pDes = NULL;
	DWORD dwSvcLen = 0;
	DWORD dwSvcLenR = 0;
	if (!QueryServiceConfig2(hSvc, SERVICE_CONFIG_DESCRIPTION, NULL, 0, &dwSvcLen)){
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER){
			dwSvcLenR = dwSvcLen;
			pDesData = (wchar_t *)LocalAlloc(LMEM_FIXED, dwSvcLen);
		}
		else{
			goto F_EXIT;
		}
	}
	if (!QueryServiceConfig2(hSvc, SERVICE_CONFIG_DESCRIPTION, (byte *)pDesData, dwSvcLenR, &dwSvcLen)){
		goto F_EXIT;
	}
	pDes = (LPSERVICE_DESCRIPTION)pDesData;
	o = std::wstring(pDes->lpDescription);
	brt = true;
F_EXIT:
	if (pDesData){
		LocalFree(pDesData);
		pDesData = NULL;
	}
	return brt;
}

bool ServiceOperater::changeStartType(const std::wstring& name, ServiceStartType n)
{
	if (false == assertOpen() || name.empty())
		return false;

	SC_HANDLE hSvc = OpenService(_hscm,
		reinterpret_cast<const WCHAR *>(name.c_str()),
		SERVICE_ALL_ACCESS);
	bool brt = changeStartType(hSvc, n);
	CloseServiceHandle(hSvc);
	return brt;
}

bool ServiceOperater::changeStartType(SC_HANDLE hSvc, ServiceStartType n)
{
	if (!hSvc)
		return false;

	return 0 != ChangeServiceConfig(hSvc, SERVICE_NO_CHANGE,
		n, SERVICE_NO_CHANGE,
		NULL, NULL, NULL, NULL,
		NULL, NULL, NULL);
}
