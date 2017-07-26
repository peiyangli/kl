#pragma once

#include "Variant.hpp"

#include <initializer_list>
#include <vector>
#include <string>
#include <memory>

typedef std::vector<std::wstring> StringList;
//typedef std::string ByteArray;
//typedef std::vector<char> ByteArray;
class ByteArray : public std::vector<char>{
public:
	ByteArray() 
		: vector(){
	}
	ByteArray(const std::initializer_list<char>& il) 
		: vector(std::begin(il), std::end(il)){
	}
	ByteArray(const char* p) 
		: vector(std::strlen(p)){
		if (size() > 0)
			std::memcpy(data(), p, size());
	}
	ByteArray(const char* p, size_t len) 
		: vector(len){
		if (len > 0)
			std::memcpy(data(), p, len);
	}
	ByteArray& operator += (const ByteArray& r){
		auto so = size();
		auto sn = r.size();
		resize(so + sn);
		if (sn > 0)
			std::memcpy(data() + so, r.data(), sn);
		return *this;
	}
};

typedef Variant<std::wstring, int, unsigned long long, StringList, ByteArray> RegVariant;

class RegistryPrivate;

class Registry
{
public:
	enum Status{
		NoError,
		AccessError,
		FormatError
	};
	Registry(std::wstring rPath, bool wow = false);
	~Registry();

	StringList allKeys() const;
	StringList childKeys() const;
	StringList childGroups() const;
	bool isWritable() const;

	//should be deprecated, only when REG_SZ -> REG_EXPAND_SZ, szSz used
	void setValue(const std::wstring &key, const RegVariant &value, bool exSz = false);
	RegVariant value(const std::wstring &key, const RegVariant &defaultValue = RegVariant()) const;

	void remove(const std::wstring &key);
	bool contains(const std::wstring &key) const;

	Status status() const;

	std::wstring group() const;

private:
	std::unique_ptr<RegistryPrivate>	d;
};




// 一下是测试代码
// #include "Variant.hpp"
// #include "Registry.h"
// void Test(){
// 	Registry reg(L"HKEY_CURRENT_USER\\Software\\Anvisoft\\Cloud System Booster");
// 
// 	auto keys = reg.allKeys();
// 	auto cg = reg.childGroups();
// 	auto ck = reg.childKeys();
// 
// 	auto b = reg.contains(L"lang");
// 	b = reg.contains(L"lang1");
// 	b = reg.contains(L"Settings");
// 
// 	reg.setValue(L"test_for_int", RegVariant(1));
// 	auto intv = reg.value(L"test_for_int");
// 	auto intvv = intv.To<int>(0);
// 
// 	reg.setValue(L"test_for_wstring", RegVariant(std::wstring(L"Hello Reg")));
// 	auto wstringv = reg.value(L"test_for_wstring");
// 	auto wstringvv = wstringv.To<std::wstring>(std::wstring(L""));
// 
// 
// 	reg.setValue(L"test_for_ull", RegVariant((unsigned long long)10));
// 	auto ullv = reg.value(L"test_for_ull");
// 	auto ullvv = ullv.To<unsigned long long>(1);
// 
// 	StringList info{ L"OneString", L"TowString" };
// 	reg.setValue(L"test_for_StringList", RegVariant(info));
// 	auto strlistv = reg.value(L"test_for_StringList");
// 	auto strlistvv = strlistv.To<StringList>(StringList());
// 
// 	ByteArray arr{ 'a', 'b', '\0', 'c', 'd' };
// 	arr += ByteArray("\0hello", 6);
// 	reg.setValue(L"test_for_bytes", RegVariant(arr));
// 	auto bytesv = reg.value(L"test_for_bytes");
// 	auto bytesvv = bytesv.To<ByteArray>(ByteArray());
// 
// 	//remove
// 	getchar();
// 
// 	reg.remove(L"test_for_int");
// 	reg.remove(L"test_for_wstring");
// 	reg.remove(L"test_for_ull");
// 	reg.remove(L"test_for_StringList");
// 	reg.remove(L"test_for_bytes");
// }
