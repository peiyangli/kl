#pragma once

#include <Windows.h>

#include <string>
#include <vector>


enum ServiceStartType{
	SST_UNKNOWN = -1,
	SST_AUTO = 0x00000002,
	SST_DEMAND = 0x00000003,
	SST_DISABLE = 0x00000004,
};

class ServiceConfigInfo
{
public:
	ServiceConfigInfo(const std::wstring& n) : _name(n), _startType(SST_UNKNOWN){
	}

	std::wstring name()const{
		return _name;
	}
	void setName(const std::wstring &n){
		_name = n;
	}

	std::wstring binPath()const{
		return _binPath;
	}
	void setBinPath(const std::wstring n){
		_binPath = n;
	}

	std::wstring displayName()const{
		return _displayName;
	}
	void setDisplayName(const std::wstring& n){
		_displayName = n;
	}

	ServiceStartType startType()const{
		return _startType;
	}
	void setsStartType(int t){
		switch (t)
		{
		case SST_AUTO:
			_startType = SST_AUTO;
			break;
		case SST_DEMAND:
			_startType = SST_DEMAND;
			break;
		case SST_DISABLE:
			_startType = SST_DISABLE;
			break;
		default:
			_startType = SST_UNKNOWN;
			break;
		}
	}

private:
	std::wstring             _name;

	ServiceStartType    _startType;
	std::wstring             _binPath;
	std::wstring             _displayName;
};

class ServiceOperater
{
	ServiceOperater(const ServiceOperater&);
	ServiceOperater& operator=(const ServiceOperater&);
	ServiceOperater(ServiceOperater&&);
	ServiceOperater& operator=(ServiceOperater&&);
public:

	ServiceOperater();
	~ServiceOperater();

	inline bool isOpen(){
		return _hscm != 0;
	}
	inline bool assertOpen(){
		return _hscm || open();
	}
	inline bool reOpen(){
		close();
		return open();
	}

	bool remove(const std::wstring& name);
	bool create(const std::wstring &name, const std::wstring &binPath, const std::wstring &disName = L"", const std::wstring &description = L"", ServiceStartType t = SST_AUTO);

	bool stop(const std::wstring& name);
	//Range 128 to 255 0x80-0xFF
	bool control(const std::wstring& name, unsigned code);
	bool start(const std::wstring& name);
	bool start(const std::wstring& name, const std::vector<std::wstring>& args);

	bool queryStartType(const std::wstring& name, __out ServiceStartType& o);
	bool queryBinPath(const std::wstring& name, __out std::wstring& o);
	bool queryDisplayName(const std::wstring& name, __out std::wstring& o);
	bool queryConfigInfo(__in __out ServiceConfigInfo &o);
	bool queryDescription(const std::wstring& name, __out std::wstring& o);
	bool changeStartType(const std::wstring& name, ServiceStartType n);
private:
	bool open();
	void close();

	bool queryConfigInfo(SC_HANDLE hSvc, __out ServiceConfigInfo &o);
	bool queryDescription(SC_HANDLE hSvc, __out std::wstring& o);

	bool changeStartType(SC_HANDLE hSvc, ServiceStartType n);
private:
	SC_HANDLE   _hscm; //handle of service manager
};

