#pragma once

#include <ntifs.h>

// #include <dontuse.h>
// #include <suppress.h>
#ifdef __cplusplus
extern "C" {
//#include <wdm.h>
#include <fltKernel.h>

#endif
	DRIVER_INITIALIZE DriverEntry;
	NTSTATUS
		DriverEntry(
		_In_ PDRIVER_OBJECT DriverObject,
		_In_ PUNICODE_STRING RegistryPath
		);
#ifdef __cplusplus 
}
#endif



class Fs
{
public:
	static NTSTATUS FLTAPI
		InstanceSetup( _In_ PCFLT_RELATED_OBJECTS FltObjects,
		_In_ FLT_INSTANCE_SETUP_FLAGS Flags,
		_In_ DEVICE_TYPE VolumeDeviceType,
		_In_ FLT_FILESYSTEM_TYPE VolumeFilesystemType
		);
	static VOID FLTAPI
		InstanceTeardownStart( _In_ PCFLT_RELATED_OBJECTS FltObjects,
		_In_ FLT_INSTANCE_TEARDOWN_FLAGS Flags
		);
	static VOID FLTAPI
		InstanceTeardownComplete( _In_ PCFLT_RELATED_OBJECTS FltObjects,
		_In_ FLT_INSTANCE_TEARDOWN_FLAGS Flags
		);
	static NTSTATUS FLTAPI
		InstanceQueryTeardown( _In_ PCFLT_RELATED_OBJECTS FltObjects,
		_In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
		);
	static NTSTATUS DriverEntry( _In_ PDRIVER_OBJECT DriverObject,
		_In_ PUNICODE_STRING RegistryPath
		);
	static NTSTATUS FLTAPI
		Unload( _In_ FLT_FILTER_UNLOAD_FLAGS Flags
		);

	PFLT_FILTER _filterHandle;
public:
	PFLT_FILTER filterHandle()const{ return _filterHandle; };
	Fs();
	~Fs();
};
extern Fs* gFsInstance;

