/*++

Module Name:

    driver.c

Abstract:

    This file contains the driver entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

#include "driver.h"

#pragma warning(disable: 4819)




#ifdef ALLOC_PRAGMA
#pragma ALLOC_TEXT (INIT, DriverEntry)
#pragma ALLOC_TEXT (PAGE, KMDFEvtDeviceAdd)
#pragma ALLOC_TEXT (PAGE, KMDFEvtDriverContextCleanup)
#endif


#define PRINT_TAG "[KMDF]: "

#include <../skl/kernel.h>

#include "../skl/shared_ptr.hpp"

#include "../skl/lookaside.hpp"

#include "../skl/list.hpp"

#include "../skl/work_queue.hpp"
#include "../skl/functional.hpp"

#include "win.h"
//class Test2;
//typedef skl::LookAside<int, bool, Test2> LookAside_t;
//#define LOOKASIDE_ALLOC(type) LOOKASIDE_ALLOC_WITHT(type, LookAside_t)//to skl::MatchTypeFinder<type, LookAside_t>::TType::get()
//#define LOOKASIDE_FREE(ptr) LOOKASIDE_FREE_WITHT(ptr, LookAside_t)//to skl::MatchTypeFinder<skl::remove_pointer<decltype(ptr)>::type, LookAside_t>::TType::free(ptr)

class Test2{
	int _id;
	long _more;
public:
	Test2(int id = -1)
		:_id(id), _more(9999){
		KdPrint((PRINT_TAG"Test2 ID(%d):\n", (ULONG)_id));
	}
	Test2(int id, long m)
		:_id(id), _more(m){
		KdPrint((PRINT_TAG"Test2 ID(%d):\n", (ULONG)_id));
	}
	~Test2(){
		KdPrint((PRINT_TAG"Test2 ID(%d):\n", (ULONG)_id));
	}
	int getid(){ return _id; }

	bool test(){
	}

	unsigned long long hash;
};


class Test{
	int _id;
	long _more;
public:
	Test(int id = -1, long more = 999) 
		:_id(id), _more(more){
		KdPrint((PRINT_TAG"Test ID(%d):\n", (ULONG)_id));
	}
	virtual ~Test(){
		KdPrint((PRINT_TAG"Test ID(%d):\n", (ULONG)_id));
	}
	int getid(){ return _id; }

	bool test(){
	}
	bool operator < (const Test& r){
		return _id < r._id;
	}
};

class TestChild : public Test{
public:
	TestChild(int id = 1, long more = 1) : Test(id, more){}
};

class TestForPlacementNew{
	int i = 0;
public:
	TestForPlacementNew(){
		KdPrint((PRINT_TAG"TestForPlacementNew ID(%d):\n", (ULONG)i));
	}
	~TestForPlacementNew(){
		KdPrint((PRINT_TAG"~TestForPlacementNew ID(%d):\n", (ULONG)i));
	}
};

template<class _T>
class TestMore{
	_T _func;
public:
	TestMore(_T f) :_func(f){}

	void invoke(){
		_func();
	}
};

#include "../skl/utility.h"
#include "../skl/map.hpp"
#include "../skl/set.hpp"
#include "../skl/string.hpp"
#include "../skl/thread.hpp"

#include "../skl/mutex.hpp"

#include "../skl/vector.hpp"
class Context{
public:
	skl::mutex _m;
};

#pragma warning(disable: 4127) // while (false);
NTSTATUS RegGetDword(UNICODE_STRING& keyName, UNICODE_STRING& valueName, ULONG* o)
{
	NTSTATUS status = STATUS_INVALID_PARAMETER;
	HANDLE hKey = NULL;
	do 
	{
		if (!o)
			break;
		OBJECT_ATTRIBUTES  ObjectAttributes;
		InitializeObjectAttributes(&ObjectAttributes, &keyName,
			OBJ_CASE_INSENSITIVE, NULL, NULL);

		status = ZwOpenKey(&hKey, GENERIC_ALL, &ObjectAttributes);
		if (!NT_SUCCESS(status)) {
			break;
		}
		unsigned char buffer[sizeof(KEY_VALUE_PARTIAL_INFORMATION) + sizeof(ULONG)];
		PKEY_VALUE_PARTIAL_INFORMATION pkvpi = (PKEY_VALUE_PARTIAL_INFORMATION)buffer;
		ULONG ulSize = sizeof(KEY_VALUE_PARTIAL_INFORMATION) + sizeof(ULONG);
		status = ZwQueryValueKey(hKey, &valueName, KeyValuePartialInformation, pkvpi, ulSize, &ulSize);
		if (!NT_SUCCESS(status)) {
			break;
		}
		if (pkvpi->Type != REG_DWORD){
			status = STATUS_VHD_INVALID_TYPE;
			break;
		}
		if (pkvpi->DataLength != sizeof(ULONG)) {
			status = STATUS_INTERNAL_ERROR;
			break;
		}
		*o = *((PULONG)pkvpi->Data);
	} while (false);
	if (hKey){
		ZwClose(hKey);
	}
	return status;
}

#define REG_SOFTWARE_KEY_NAME  L"\\Registry\\Machine\\Software\\KMDF"
#define REG_SOFTWARE_VALUE_NAME  L"TestValue"


VOID SleepFor100ns(LONG ns100)
{
	if (ns100 <= 0)return;
	LARGE_INTEGER my_interval;
	//my_interval.QuadPart = -1;
	my_interval.QuadPart = -ns100;
	KeDelayExecutionThread(KernelMode, 0, &my_interval);
}

ULONGLONG GetTickCount100ns()
{
	LARGE_INTEGER tick_count;
	static ULONG myinc = KeQueryTimeIncrement();
	KeQueryTickCount(&tick_count);
	tick_count.QuadPart *= myinc;
	return tick_count.QuadPart;
}

VOID SleepFor100ns2(LONG ns100)
{
	if (ns100 <= 0)return;
	LARGE_INTEGER my_interval;
	//my_interval.QuadPart = -1;
	my_interval.QuadPart = GetTickCount100ns() + ns100;
	KeDelayExecutionThread(KernelMode, 0, &my_interval);
}

void MakeATest()
{
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING keyName;
	UNICODE_STRING valueName;
	RtlInitUnicodeString(&keyName, REG_SOFTWARE_KEY_NAME);
	RtlInitUnicodeString(&valueName, REG_SOFTWARE_VALUE_NAME);
	ULONG value = 10000;
	status = RegGetDword(keyName, valueName, &value);



	auto preTime = GetTickCount100ns();
	__int64 dtime = 0;
	KdPrint(("[KMDF]%l\n", dtime));

	for (ULONG i = 0; i < value; ++i) //1ms if value 10000
	{
		SleepFor100ns(1); //100ns, 10-4ms
	}

	auto curTime = GetTickCount100ns();
	dtime = curTime - preTime;
	KdPrint(("[KMDF]%l\n", dtime));
	preTime = curTime;

	for (ULONG i = 0; i < value; ++i) //10ms
	{
		SleepFor100ns(10); 
	}

	curTime = GetTickCount100ns();
	dtime = curTime - preTime;
	KdPrint(("[KMDF]%l\n", dtime));
	preTime = curTime;

	for (ULONG i = 0; i < value; ++i) //100ms
	{
		SleepFor100ns(100);
	}

	curTime = GetTickCount100ns();
	dtime = curTime - preTime;
	KdPrint(("[KMDF]%l\n", dtime));
	preTime = curTime;

	for (ULONG i = 0; i < value; ++i) //1s
	{
		SleepFor100ns(100000);
	}

	curTime = GetTickCount100ns();
	dtime = curTime - preTime;
	KdPrint(("[KMDF]%l\n", dtime));
	preTime = curTime;
}

#include "../skl/atomic_bits.hpp"


int IamInThread(int c){
	c = c + 1;
	auto tid = skl::thread::get_id();
	tid;
	return c;
}



struct BaseCall{
	virtual ~BaseCall(){}
	virtual void run() = 0;
	void operator()(){
		run();
	}
};

class SubCall : public BaseCall{
	int i = 0;
public:
	SubCall(){}
	virtual void run() override{
		i++;
	}
};

class SubCall2 : public BaseCall{
	long i = 100;
public:
	virtual void run() override{
		i--;
	}
};

#include "../skl/array.hpp"
#include "../skl/scope_guard.hpp"
#include "../skl/kfifo.hpp"

ULONG rand(ULONG min, ULONG max)
{
	static ULONG seed = (ULONG)GetTickCount100ns();
	return min + RtlRandomEx(&seed) % (max - min);
}


#define MAX_PATH_LEN 260
NTSTATUS
DriverEntryImp(
    _In_ PDRIVER_OBJECT  DriverObject,
    _In_ PUNICODE_STRING RegistryPath
    )
{
	UNREFERENCED_PARAMETER(RegistryPath);
	UNREFERENCED_PARAMETER(DriverObject);

	skl::Fifo<int> fifo;
	for (int i = 1; i < 20; ++i){
		fifo.push(i);
	}

	int o = 0;
	int ic = 0;
	while (fifo.pop(o)) {
		o;
		ic++;
	}

	auto rn = rand(0, 10000);
	for (int i = 0; i < 100; ++i)
	{
		rn = rand(0, 10000);
	}

	//skl::atomic_bits<1008, 10> bits;
	NTSTATUS status = STATUS_NOT_IMPLEMENTED;
	BOOTDISK_INFORMATION_EX bi;
	IoGetBootDiskInformation((PBOOTDISK_INFORMATION)&bi, sizeof(bi));

	char tmpbuffer[sizeof(UNICODE_STRING) + MAX_PATH_LEN*sizeof(WCHAR)];
	//status = RetrieveNameByEPROCESS(IoGetCurrentProcess(), tmpbuffer, sizeof(tmpbuffer), NULL);
	ULONG rtl = 0;
	status = RetrieveNameByPID((HANDLE)252, tmpbuffer, sizeof(tmpbuffer), &rtl);
	if (!NT_SUCCESS(status)) {
		return status;
	}
	status = STATUS_NOT_IMPLEMENTED;

	PUNICODE_STRING strSystem4 = (UNICODE_STRING*)tmpbuffer;
	strSystem4;

	int nnnnn = 100;
	{
		SCOPE_GUARD_LAMBDA(hell, [&nnnnn](){
			nnnnn-=2;
		});
		nnnnn *= 2;

	}
	nnnnn;//nnnnn*2-2 is ok, not (nnnnn-2)*2
	{//array
		//skl::array<int, 3> arr = { 1 };

	}

	{
		//test vector
		skl::vector<int, NonPagedPool> vInt;
		skl::vector<int, PagedPool> vInt2({101, 102, 103, 104, 105, 106, 107, 108, 109});
		vInt.push_back(1);
		vInt.push_back(2);
		vInt2.push_back(110);
		vInt2.push_back(111);
		vInt2.push_back(112);
		vInt.append(vInt2);
		auto vvv = 0;
		for (unsigned i = 0; i < vInt.size(); ++i){
			vvv = vInt.at(i);
		}
		vInt.insert(0, 3);
		vInt.insert(2, 4);
		vInt.insert(1, {11,12,13,14});
		for (auto& itr : vInt)
		{
			vvv = itr;
		}

		vInt.erase(1);
		for (auto& itr : vInt)
		{
			vvv = itr;
		}

		vInt.clear();

		skl::vector<skl::shared_ptr<Test>> vTest;
		auto pt1 = skl::make_shared<Test>(1);
		auto pt2 = skl::make_shared<Test>(2);
		for (unsigned i = 0; i < 10; ++i){
			vTest.push_back(pt1);
		}
		vTest.push_back(pt2);
		for (unsigned i = 0; i < vTest.size(); ++i){
			auto vvvt = vTest.at(i);
			vvvt;
		}

		int id = 0;
		for (auto itr : vTest)
		{
			id = itr->getid();
		}

		vTest.clear();

		auto uc = pt1.use_count();
		uc = pt2.use_count();
		uc;

		skl::vector<skl::string> vWhiteList{L"c:\\windows\\", L"c:\\hello\\windows\\"};
		for (auto& itr : vWhiteList)
		{
			//itr.istartwith();
			istartwith(L"c:\\hello\\windows\\teswt.exe", itr);
		}
	}

	{
		auto ptest0 = skl::make_shared<Test>(0);
		auto ptest1 = skl::make_shared<Test>(1);
		auto ptest2 = skl::make_shared<Test>(2);
		auto ptest3 = skl::make_shared<Test>(3);
		auto ptest4 = skl::make_shared<Test>(4);
		auto ptest5 = skl::make_shared<Test>(5);
		auto ptest6 = skl::make_shared<Test>(6);
		auto ptest7 = skl::make_shared<Test>(7);
		auto ptest8 = skl::make_shared<Test>(8);
		auto ptest9 = skl::make_shared<Test>(9);
		auto ptest10 = skl::make_shared<Test>(10);
		{
			skl::vector<skl::shared_ptr<Test>> vTest1(2);
			skl::vector<skl::shared_ptr<Test>> vTest2({ ptest0, ptest2, ptest3 });
			vTest1 = vTest2;

			vTest1.insert(1, ptest1);

			vTest1.insert(1, { ptest4, ptest5, ptest6, ptest7, ptest8, ptest9, ptest10 });

			vTest1.erase(0);

			vTest1.clear();
		}
		auto ppppuc = ptest0.use_count();
		ppppuc;
	}


	//MakeATest();

	//LookAside::init_once<int, Test>(false);
	skl::LookAsideSizePool<PagedPool, 1, 2, 4, 8, 16> lookaside;

	auto pchar = lookaside.New<char>();
	auto punsigned = lookaside.New<unsigned>();
	auto punsigned2 = lookaside.New<unsigned>(11);
	auto pull = lookaside.New<unsigned long long>();
	auto pTest2 = lookaside.New<Test2>();
	auto pTest22 = lookaside.New<Test2>(101, 888);
	//const int d = sizeof(Test2);
	//pTest2 = new(pTest2)Test2;

	lookaside.Delete(pchar);
	lookaside.Delete(punsigned);
	lookaside.Delete(punsigned2);
	lookaside.Delete(pull);

	//pTest2->~Test2();
	lookaside.Delete(pTest2);
	lookaside.Delete(pTest22);

	auto buffer = new char[sizeof(TestForPlacementNew)];

	auto pt = new(buffer)TestForPlacementNew;
	pt->~TestForPlacementNew();//if this is a struct, no need to call this

	delete[]buffer;

	// 	LookAside::get<int>();
	// 	LookAside::get<Test>();

	{
		skl::map<skl::string, bool> msb;
		msb.insert(skl::string(L"c:\\one.exe"), true);
		msb.insert(skl::string(L"c:\\tow.EXE"), true);
		msb.insert(skl::string(L"c:\\three.ex"), true);
		msb.insert(skl::string(L"c:\\one.exxe"), true);
		msb.count();

		for (auto itr : msb)
		{
			auto mev = itr->first;
			auto bs = mev.startwith(L"c:\\");
			bs = mev.startwith(L"C:\\");
			bs = mev.istartwith(L"c:\\");
			bs = mev.istartwith(L"C:\\");


			bs = mev.endwith(L".exe");
			bs = mev.endwith(L".exe");
			bs = mev.iendwith(L".exe");
			bs = mev.iendwith(L".exe");


			KdPrint(("Hello\n"));
		}
	}
	


	{
		skl::string s(L"device\\one\\device\\hello.exe");
		auto pos = s.find(L"one");
		if (pos != skl::string::npos){
			auto ss = s.substr(pos);
			ss;
		}
		pos = s.ifind(L"One");
		if (pos != skl::string::npos){
			auto ss = s.substr(pos);
			ss;
		}

		auto rs = s.repalce(L"device", L"Dos");
		rs;

	}

	{
		skl::string s(L"device\\one\\hello.exe");
		auto pos = s.rfind(L'\\');
		if (pos != skl::string::npos){
			auto ss = s.substr(pos+1);
			ss;
		}

		pos = s.rifind(L"onE\\");
		if (pos != skl::string::npos){
			auto ss = s.substr(pos + 1);
			ss;
		}
	}

	{
		skl::list<int> v;

		v.swap(skl::list<int>());//empty

		v.push_back(110);

		v.swap(skl::list<int>());//empty

		skl::list<int> vv;
		vv.push_back(120);
		v.swap(vv);

		int tmpi = 0;
		for (auto li : v){
			tmpi = li->_v;
		}
		auto itr = v.push_back(1);
		itr = v.push_back(2);
		//itr = 
			v.push_back(3);

		for (auto li: v){
			tmpi = li->_v;
		}
		v.remove(itr);
		v.remove(itr);

		for (auto li : v){
			tmpi = li->_v;
		}
		itr = v.pop_front();

		for (auto li : v){
			tmpi = li->_v;
		}

		skl::list<int> v2;
		v2.push_back(10);
		v2.push_back(11);
		v2.push_back(12);

		v.swap(v2);
		for (auto li : v){
			tmpi = li->_v;
		}
		for (auto li : v2){
			tmpi = li->_v;
		}


		skl::list<int> av;
		av.push_back(50);
		skl::list<int> ap;
		ap.push_back(10);
		ap.push_back(20);
		ap.push_back(30);
		ap.push_back(40);
		av.append(skl::move(ap));

		for (auto li : av){
			tmpi = li->_v;
		}

		for (auto li : ap){//none
			tmpi = li->_v;
		}

	}
	{
		skl::stack<int> s;
		s.push(10);
		s.push(12);
		s.push(14);

		int svvvvv = 0;
		for (auto li : s){
			svvvvv = li;
		}
	}

	{
		skl::map<Test, skl::shared_ptr<Test>> m2;

		skl::map<int, Test> m;

		for (auto itr : m)
		{
			auto mev = itr->second;
			KdPrint((PRINT_TAG"Hello\n"));
		}
		for (auto itr = m.begin(); itr != m.end(); ++itr)
		{
			auto mev = itr->second;
			KdPrint((PRINT_TAG"Hello\n"));
		}

		
		auto count = m.count();
		auto rt = m.insert(3, Test(3));
		rt = m.insert(4, Test(4));
		rt = m.insert(3, Test(5));
		count = m.count();
		rt = m.remove(3);
		count = m.count();
		rt = m.remove(3);
		count = m.count();
		rt = m.insert(3, Test(6));
		count = m.count();

		for (auto itr : m)
		{
			auto mev = itr->second;
			KdPrint((PRINT_TAG"Hello\n"));
		}
		for (auto itr = m.begin(); itr != m.end(); ++itr)
		{
			auto mev = itr->second;
			KdPrint((PRINT_TAG"Hello\n"));
		}
	}
	
	{
		skl::set<int> si;
		si.insert(9);
		si.insert(10);
		for (auto itr : si)
		{
			auto siv = itr->first;
			siv;
		}
		si.find(9);
	}

	skl::pair<Test, int> pv(1, 4);

	bool btest = true;
	int id = 0;
	if (btest){
		auto tttt = new Test(1);
		if (tttt){
			id = tttt->getid();
			delete tttt;
		}
		
		tttt = new(PagedPool)Test(2);
		if (tttt){
			id = tttt->getid();
			delete tttt;
		}
		tttt = new(PagedPool, 'OOOO')Test(3);
		if (tttt){
			id = tttt->getid();
			delete tttt;
		}

		//skl::shared_list<Test> slist;
		//skl::function<bool(int)> check = [](int i){return i % 2 == 0; };
		skl::shared_ptr<Test> ptro;
		{
			//auto 
			skl::shared_ptr<Test> ptr;
			ptr = skl::make_shared<TestChild>(TestChild());
			ptr = skl::make_shared<Test, PagedPool>(5);
			if (ptr){
				id = ptr->getid();

				auto ptr2 = ptr;

				id = ptr2.use_count();
				id = ptr2->getid();

				auto ptr3 = skl::make_shared<Test>(6);

				if (ptr3){
					id = ptr.use_count();

					ptr = ptr3;
				}

				id = ptr->getid();
				id = ptr.use_count();
			}
			
			ptro = ptr;
		}
		if (ptro){
			id = ptro->getid();
			id = ptro.use_count();
		}
		//id = ptro.getV();
		auto bv = skl::make_shared<bool>(false);
		if (bv){
			//id = bv.getV();
			if (!*bv)
				bv = skl::make_shared<bool>(true);
		}
		//slist.push_back(ptro);
		//auto arraytest = skl::make_shared_array<Test, 6>();
	}
	//LookAside::init_once<int, Test>(true);

	KdPrint((PRINT_TAG"=================LookAside Begin:\n"));

	{
		auto ptr = new Test2(12);
		if (ptr){
			id = ptr->getid();
			delete ptr;
		}

		auto sptr = skl::make_shared<Test2>(13);
		if (sptr){
			id = sptr->getid();
			id = sptr.use_count();
		}
	}


	
	KdPrint((PRINT_TAG"=================LookAside Begin:\n"));

	KdPrint((PRINT_TAG"=================LookAsidePool End:\n"));
	{
		skl::LookAsidePool<NonPagedPool, int, bool, Test> poolLookAside;
		auto pint = poolLookAside.New<int>();
		poolLookAside.Delete(pint);
// 
		auto pbool = poolLookAside.New<bool>();
		poolLookAside.Delete(pbool);

		auto ptest = poolLookAside.Malloc<Test>();
		ptest->Test::Test(100);
		id = ptest->getid();
		ptest->Test::~Test();
		poolLookAside.Free(ptest);

		ptest = poolLookAside.New<Test>(100);
		id = ptest->getid();
		poolLookAside.Delete(ptest);
	}
	KdPrint((PRINT_TAG"=================LookAsidePool End:\n"));

	

	auto labda = [](int){
	};
	auto ptrlll = skl::make_shared<decltype(labda)>(labda);
	(*ptrlll)(12);


	auto mid = skl::thread::get_id();
	auto spTest = skl::make_shared<Test>(110);
	auto spContext = skl::make_shared<Context>();

#define OneSecondFor100ns 1000000

	skl::thread a([spTest, mid, spContext](){
		auto id = spTest->getid();
		id;//110
		auto uc = spTest.use_count();//be 2 is right

		auto tid = skl::thread::get_id();

		if (mid == tid)
			return 0l;

		if (spContext){

			for (int k = 0; k < 5; ++k)
			{
				skl::thread::sleep_for_100ns(OneSecondFor100ns);//100ms
				skl::auto_locker<skl::mutex> locker(spContext->_m);
				for (int i = 0; i < 5; ++i)
				{
					skl::thread::sleep_for_100ns(OneSecondFor100ns);//100ms
				}
			}
			
		}
		return uc;
	});

	if (spContext){//test mutex
		for (int k = 0; k < 5; ++k)
		{
			skl::thread::sleep_for_100ns(OneSecondFor100ns);//100ms
			skl::auto_locker<skl::mutex> locker(spContext->_m);
			for (int i = 0; i < 5; ++i)
			{
				skl::thread::sleep_for_100ns(OneSecondFor100ns);//100ms
			}
		}
	}

	a.detach();
	auto uc = spTest.use_count();
	uc;



	{
		auto spTest2 = skl::make_shared<Test>(110);
		auto spt2 = spTest2;
		skl::thread([](skl::shared_ptr<Test> n){
			auto count = n.use_count();
			n;
			count;
			auto id = n->getid();

			auto tid = skl::thread::get_id();
			tid;
			return id;
		}, spTest2).then([](){
		});
		//b.detach();

		uc = spTest2.use_count();

		skl::thread c(IamInThread, 3);
		c.detach();

		
	}

	auto spt = skl::make_shared<Test>(11111);
	{
		auto worker = skl::simple_worker::create(); //use this thread to make a productive/consumption
		if (worker){
			for (int i = 0; i < 100; ++i)
			{
				worker->push_a_async_call([i, spt, mid](){
					i;
					auto k = spt.use_count(); //101 - i
					k;
					auto ctid = skl::thread::get_id();
					if (ctid == mid)
						return -1;
					return 0;
				});
			}
			skl::thread::sleep_for_100ns(10000000);
			worker->clear();
			worker->detach();//stop and wait for thread to exit
		}
	}
	auto k = spt.use_count();//be one is ok
	k;

	//////////////////////////////////////////////////////////////////////////
	skl::shared_ptr<SubCall> item;
	skl::shared_ptr<SubCall2> item2;
	{
		auto worker = skl::interface_worker<BaseCall>::create();
		if (worker){
			item = skl::make_shared<SubCall>();
			item2 = skl::make_shared<SubCall2>();
			for (int i = 0; i < 100; ++i)
			{
				if (i % 3 == 0){
					worker->push(item2);
				}
				worker->push(item);
			}
			worker->detach();
		}
	}

	return status;


// 	WDF_DRIVER_CONFIG config;
// 	WDF_OBJECT_ATTRIBUTES attributes;
//     //
//     // Initialize WPP Tracing
//     //
// 
//     //
//     // Register a cleanup callback so that we can call WPP_CLEANUP when
//     // the framework driver object is deleted during driver unload.
//     //
//     WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
//     attributes.EvtCleanupCallback = KMDFEvtDriverContextCleanup;
// 
//     WDF_DRIVER_CONFIG_INIT(&config,
//                            KMDFEvtDeviceAdd
//                            );
// 
//     status = WdfDriverCreate(DriverObject,
//                              RegistryPath,
//                              &attributes,
//                              &config,
//                              WDF_NO_HANDLE
//                              );
// 
//     if (!NT_SUCCESS(status)) {
//         return status;
//     }
// 
//     return status;
}

NTSTATUS
DriverEntry(
_In_ PDRIVER_OBJECT  DriverObject,
_In_ PUNICODE_STRING RegistryPath
){
	NTSTATUS status = STATUS_NOT_IMPLEMENTED;
	status = skl::runtime::RunTimeInit();
	if (STATUS_SUCCESS != status)return status;

	status = DriverEntryImp(DriverObject, RegistryPath);

	if (!NT_SUCCESS(status))
		skl::runtime::RunTimeUnInit();

	return status;
}


NTSTATUS
KMDFEvtDeviceAdd(
    _In_    WDFDRIVER       Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
/*++
Routine Description:

    EvtDeviceAdd is called by the framework in response to AddDevice
    call from the PnP manager. We create and initialize a device object to
    represent a new instance of the device.

Arguments:

    Driver - Handle to a framework driver object created in DriverEntry

    DeviceInit - Pointer to a framework-allocated WDFDEVICE_INIT structure.

Return Value:

    NTSTATUS

--*/
{
    NTSTATUS status;

    UNREFERENCED_PARAMETER(Driver);

    PAGED_CODE();

    status = KMDFCreateDevice(DeviceInit);
    return status;
}

VOID
KMDFEvtDriverContextCleanup(
    _In_ WDFOBJECT DriverObject
    )
/*++
Routine Description:

    Free all the resources allocated in DriverEntry.

Arguments:

    DriverObject - handle to a WDF Driver object.

Return Value:

    VOID.

--*/
{
    UNREFERENCED_PARAMETER(DriverObject);

    PAGED_CODE ();

    //
    // Stop WPP Tracing
    //

	skl::runtime::RunTimeUnInit();
}
