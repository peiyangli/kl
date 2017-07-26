#include "TestExport_p.h"

#include"../Debug/dbgonly.hpp"

class TestObjectImp{
public:
	DWORD GetInterface(__out IBaseObject** o, __in const GUID& InterfaceId){
		//if (__uuidof(ISettingsDiskIOCheck) == InterfaceId){
		//	if (!_mpp._DiskIOCheck)return ERROR_MOD_NOT_FOUND;
		//	auto ins = new(std::nothrow)SettingsDiskIOCheckImp(_mpp._DiskIOCheck);
		//	if (!ins)return ERROR_OUTOFMEMORY;
		//	*o = ins;
		//	return ERROR_SUCCESS;
		//}
		DbgPrtFileLineFunction;
		return ERROR_MOD_NOT_FOUND;
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////
DWORD TestObject::GetInterface(__out IBaseObject** ppObj, __in const GUID& InterfaceId)
{
	if (!ppObj)return ERROR_INVALID_PARAMETER;
	return _p->GetInterface(ppObj, InterfaceId);
}


extern volatile int gValue;

int TestObject::setValue(int v)
{
	auto o = gValue;
	gValue = v;
	return o;
}


int TestObject::getValue()
{
	return gValue;
}