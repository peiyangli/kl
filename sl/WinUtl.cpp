#include"WinUtl.h"

#include<string>


#include<Shlwapi.h>

#define BUF_LEN 512
#define MAX_BUF_LEN 1024000
bool ModuleFileName(__out std::wstring& o, HMODULE ins){
	wchar_t buf[BUF_LEN] = {};
	DWORD len = BUF_LEN;
	DWORD rtLen = 0;

	bool ok = false;
	wchar_t* pBuf = buf;
	do
	{
		rtLen = GetModuleFileName(ins, pBuf, len);
		if (rtLen == 0)
			break;
		if (rtLen < len){
			o = pBuf;
			ok = true;
			break;
		}

		len = 2 * len;

		if (pBuf != buf){
			delete[] pBuf;
		}
		pBuf = new wchar_t[len];

	} while (len < MAX_BUF_LEN);

	if (ok){
		::PathCanonicalize(pBuf, o.c_str());
		o = pBuf;
	}
	if (pBuf != buf){
		delete[] pBuf;
	}
	return ok;
}