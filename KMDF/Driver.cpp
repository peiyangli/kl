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

#include <../skl/kernel.hpp>

#include "../skl/shared_ptr.hpp"

#include "../skl/lookaside.hpp"


class Test2;
typedef skl::LookAside<int, bool, Test2> LookAside_t;
#define LOOKASIDE_ALLOC(type) LOOKASIDE_ALLOC_WITHT(type, LookAside_t)//to skl::MatchTypeFinder<type, LookAside_t>::TType::get()
#define LOOKASIDE_FREE(ptr) LOOKASIDE_FREE_WITHT(ptr, LookAside_t)//to skl::MatchTypeFinder<skl::remove_pointer<decltype(ptr)>::type, LookAside_t>::TType::free(ptr)




class Test2{
	int _id;
	long _more;
public:
	Test2(int id = -1)
		:_id(id), _more(9999){
		KdPrint((PRINT_TAG"Test2 ID(%d):", (ULONG)_id));
	}
	~Test2(){
		KdPrint((PRINT_TAG"Test2 ID(%d):", (ULONG)_id));
	}
	int getid(){ return _id; }

	bool test(){
	}

	void* operator new(size_t size){
		UNREFERENCED_PARAMETER(size);
		return LOOKASIDE_ALLOC(Test2);
	}
		void operator delete(void *ptr, size_t size){
			UNREFERENCED_PARAMETER(size);
			LOOKASIDE_FREE((Test2*)ptr);
	}
};


class Test{
	int _id;
	long _more;
public:
	Test(int id = -1) 
		:_id(id), _more(9999){
		KdPrint((PRINT_TAG"Test ID(%d):", (ULONG)_id));
	}
	~Test(){
		KdPrint((PRINT_TAG"Test ID(%d):", (ULONG)_id));
	}
	int getid(){ return _id; }

	bool test(){
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




NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT  DriverObject,
    _In_ PUNICODE_STRING RegistryPath
    )
/*++

Routine Description:
    DriverEntry initializes the driver and is the first routine called by the
    system after the driver is loaded. DriverEntry specifies the other entry
    points in the function driver, such as EvtDevice and DriverUnload.

Parameters Description:

    DriverObject - represents the instance of the function driver that is loaded
    into memory. DriverEntry must initialize members of DriverObject before it
    returns to the caller. DriverObject is allocated by the system before the
    driver is loaded, and it is released by the system after the system unloads
    the function driver from memory.

    RegistryPath - represents the driver specific path in the Registry.
    The function driver can use the path to store driver related data between
    reboots. The path does not store hardware instance specific data.

Return Value:

    STATUS_SUCCESS if successful,
    STATUS_UNSUCCESSFUL otherwise.

--*/
{
	UNREFERENCED_PARAMETER(RegistryPath);
	UNREFERENCED_PARAMETER(DriverObject);

	NTSTATUS status = STATUS_NOT_IMPLEMENTED;

	//LookAside::init_once<int, Test>(false);

// 	LookAside::get<int>();
// 	LookAside::get<Test>();

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
		//skl::function<bool(int)> check = [](int i){return i % 2 == 0; };
		skl::shared_ptr<Test> ptro;
		{
			auto ptr = skl::helper::make_shared<Test, PagedPool>(5);
			if (ptr){
				id = ptr->getid();

				auto ptr2 = ptr;

				id = ptr2.use_count();
				id = ptr2->getid();

				auto ptr3 = skl::helper::make_shared<Test>(6);

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
		id = ptro.getV();
		auto bv = skl::helper::make_shared<bool>(false);
		if (bv){
			id = bv.getV();
			if (!*bv)
				bv = skl::helper::make_shared<bool>(true);
		}

		//auto arraytest = skl::helper::make_shared_array<Test, 6>();
	}
	//LookAside::init_once<int, Test>(true);

	KdPrint((PRINT_TAG"=================LookAside Begin:"));
	LookAside_t::init_once(false);


	{
		auto ptr = new Test2(12);
		if (ptr){
			id = ptr->getid();
			delete ptr;
		}

		auto sptr = skl::helper::make_shared<Test2>(13);
		if (sptr){
			id = sptr->getid();
			id = sptr.use_count();
		}
	}
	
	

	{
		auto ptr = LOOKASIDE_ALLOC(bool);
		if (ptr){
			*ptr = false;
			LOOKASIDE_FREE(ptr);
		}
	}
	
	LookAside_t::init_once(true);

	KdPrint((PRINT_TAG"=================LookAside Begin:"));

	KdPrint((PRINT_TAG"=================LookAsidePool End:"));
	{
		skl::LookAsidePool<int, bool, Test> poolLookAside;
		auto pint = poolLookAside.New<int>();
		poolLookAside.Delete(pint);
// 
		auto pbool = poolLookAside.New<bool>();
		poolLookAside.Delete(pbool);

		auto ptest = poolLookAside.New<Test>();
		ptest->Test::Test(100);
		id = ptest->getid();
		ptest->Test::~Test();
		poolLookAside.Delete(ptest);
	}
	KdPrint((PRINT_TAG"=================LookAsidePool End:"));


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
}
