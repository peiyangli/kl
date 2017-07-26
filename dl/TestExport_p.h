#pragma once

#include"TestExport.h"

#include<memory>

class TestObjectImp;
class TestObject : public ITestObject{
	std::shared_ptr<TestObjectImp> _p;
public:
	virtual DWORD GetInterface(__out IBaseObject** ppObj, __in const GUID& InterfaceId) override;

	virtual int setValue(int v) override;
	virtual int getValue() override;
};