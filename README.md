# README #

### 熟悉c++，写windows内核还在用c吗? 看看下面这个skl库吧， 虽然现在还不完善， 但已经足够强大 ###

* 简介
基础的window内核c++库
* Version:
0.0.1
* 环境：vs2013,2015/c++11(注意配置wdk相关项目)
* skl是库， KMDF是例子， 其他请忽略

### 基本内容 ###

* c++全局new/delete操作符
* 原子变量
* 智能指针： shared_ptr, unique_ptr
* lookaside内存模板
* 模板容器： queue, stack, list, map, vector
* hash计算
* string
* thread

### todo ###

* funtional/bind


### Examples, see [DriverEntry](/KMDF/Driver.cpp)###
* lookaside, [detail](/skl/lookaside.hpp)
```c
    //lookaside
    skl::LookAsideSizePool<PagedPool, 1, 2, 4, 8, 16> lookaside;
    //allocate
	auto pchar = lookaside.New<char>();
	auto punsigned = lookaside.New<unsigned>();
	auto pull = lookaside.New<unsigned long long>();
	auto pTest2 = lookaside.New<Test2>();
	//const int d = sizeof(Test2);
    //free
	lookaside.Delete(pchar);
	lookaside.Delete(punsigned);
	lookaside.Delete(pull);
	lookaside.Delete(pTest2);
```

* string, [detail](/skl/string.hpp)
```c
    //string
    skl::string s(L"device\\one\\hello.exe");
	auto pos = s.rfind(L'\\');
	if (pos != skl::string::npos){
		auto ss = s.substr(pos+1);//hello.exe
	}
```


* vector, [detail](/skl/vector.hpp)
```
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
			istartwith(skl::string(L""), itr);
		}
	}
```


* map, [detail](/skl/map.hpp)
```c
    //map, test is some class
    skl::map<int, Test> m;
    for (auto itr : m) {//call m.end() once
        auto mev = itr->second;
        KdPrint(("Hello"));
    }
    for (auto itr = m.begin(); itr != m.end(); ++itr) {
        auto mev = itr->second;
        KdPrint(("Hello"));
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

    for (auto itr : m) {
        auto mev = itr->second;
        KdPrint(("Hello"));
    }
```

* shared_ptr, [detail](/skl/shared_ptr.hpp)
```c
    //shared_ptr
    auto ptr = skl::make_shared<Test, PagedPool>(5);
    auto bv = skl::make_shared<bool>(false);
	skl::shared_ptr<BaseCall> item = skl::make_shared<SubCall>();//class child to parent
	
	//this is a async test
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
	
	//and another
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
	
```

* thread, [detail](/skl/thread.hpp)
```c
    //thread
    auto mid = skl::thread::get_id();
	auto spTest = skl::make_shared<Test>(110);
	skl::thread a([spTest, mid](){
		auto id = spTest->getid();
		id;//110
		auto uc = spTest.use_count();//be 2 is ok

		auto tid = skl::thread::get_id();

		if (mid == tid)
			return 0l;
		return uc;
	});
	a.detach();
	auto uc = spTest.use_count();//be 1 is ok
	uc;
```

### debug ###
* VirtualKD  windbg  虚拟机

### testing ###
* not well tested

### 开源协议 ###
* GPL/LGPL

### 免责声明 ###

### 联系作者 ###
* qq：1124388868
