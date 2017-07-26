#include "TestExport.h"

#include"../Debug/dbgonly.hpp"

IBaseObject::~IBaseObject(){
	DbgPrtFileLineFunction;
}

#include "TestExport_p.h"

//extern "C" 
STAPI DWORD __stdcall STGetObject(__out IBaseObject ** ppObj, __in const GUID& InferfaceID)
{
	if (!ppObj)
		return ERROR_INVALID_PARAMETER;

	if (InferfaceID == __uuidof(ITestObject)){
		auto ins = new(std::nothrow)TestObject;
		if (!ins)return ERROR_OUTOFMEMORY;
		*ppObj = ins;
		return ERROR_SUCCESS;
	}

	return ERROR_MOD_NOT_FOUND;
}
//extern "C" 
STAPI void __stdcall STReleaseObject(__in IBaseObject * ppObj)
{
	if (ppObj){
		delete ppObj;
	}
}

/////////////////////////////////////////////////////////////////////////////////
int STStartUp(){
	DbgPrtFileLineFunction;
	//DbgOut((DbgTag L"\n"));

	//OutputDebugStringA("\n");
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
class TestClassImp
{
public:
};
TestClass::TestClass() :_p(new TestClassImp)
{
}
TestClass::~TestClass()
{
	if (_p)delete _p;
}


extern volatile int gValue;
int TestClass::setValue(int v)
{
	auto o = gValue;
	gValue = v;
	return o;
}

int TestClass::getValue()
{
	return gValue;
}