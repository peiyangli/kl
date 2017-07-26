#pragma once

#include "def.h"


#define UM_TSRV_BEGIN			0x0401
#define UM_TSRV_PRINT_PARAM		0x0402
#define UM_TSRV_SETVALUE_PARAM	0x0403

STAPI int STStartUp();
STAPI int STGetValue();

STAPI DWORD STSetTID(DWORD);
STAPI DWORD STGetTID();
struct __declspec(uuid("85E7F286-6417-428A-BD09-E561BBCF8E19")) ITestObject : public IBaseObject
{
	virtual DWORD GetInterface(__out IBaseObject** ppObj, __in const GUID& InterfaceId) = 0;

	virtual int setValue(int v) = 0;
	virtual int getValue() = 0;
};

class TestClassImp;
class STAPI TestClass{
	//no std export
	TestClassImp* _p;
	TestClass(const TestClass&);
	TestClass(TestClass&&);
	TestClass& operator=(const TestClass&);
	TestClass& operator = (TestClass&&);
public:
	TestClass();
	~TestClass();

	int setValue(int v);
	int getValue();
};