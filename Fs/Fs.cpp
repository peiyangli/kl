/*++

Module Name:

    Fs.c

Abstract:

    This is the main module of the Fs miniFilter driver.

Environment:

    Kernel mode

--*/

#include "Fs.h"


//#pragma prefast(disable:__WARNING_ENCODE_MEMBER_FUNCTION_POINTER, "Not valid for kernel mode drivers")
#pragma warning(disable: 4127) // while (FALSE);
#pragma warning(disable: 4510) // default constructor could not be generated, A const data member. A data member that is a reference.


Fs* gFsInstance = nullptr;

/*************************************************************************
    Prototypes
*************************************************************************/
FLT_PREOP_CALLBACK_STATUS
FsPreOperation (
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID *CompletionContext
    );

VOID
FsOperationStatusCallback(
_In_ PCFLT_RELATED_OBJECTS FltObjects,
_In_ PFLT_IO_PARAMETER_BLOCK ParameterSnapshot,
_In_ NTSTATUS OperationStatus,
_In_ PVOID RequesterContext
);

FLT_POSTOP_CALLBACK_STATUS
FsPostOperation (
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_opt_ PVOID CompletionContext,
    _In_ FLT_POST_OPERATION_FLAGS Flags
    );

FLT_PREOP_CALLBACK_STATUS
FsPreOperationNoPostOperation (
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID *CompletionContext
    );


//
//  operation registration
//

CONST FLT_OPERATION_REGISTRATION Callbacks[] = {

#if 0 // TODO - List all of the requests to filter.
    { IRP_MJ_CREATE,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_CREATE_NAMED_PIPE,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_CLOSE,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_READ,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_WRITE,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_QUERY_INFORMATION,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_SET_INFORMATION,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_QUERY_EA,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_SET_EA,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_FLUSH_BUFFERS,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_QUERY_VOLUME_INFORMATION,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_SET_VOLUME_INFORMATION,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_DIRECTORY_CONTROL,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_FILE_SYSTEM_CONTROL,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_DEVICE_CONTROL,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_INTERNAL_DEVICE_CONTROL,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_SHUTDOWN,
      0,
      FsPreOperationNoPostOperation,
      NULL },                               //post operations not supported

    { IRP_MJ_LOCK_CONTROL,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_CLEANUP,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_CREATE_MAILSLOT,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_QUERY_SECURITY,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_SET_SECURITY,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_QUERY_QUOTA,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_SET_QUOTA,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_PNP,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_ACQUIRE_FOR_SECTION_SYNCHRONIZATION,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_RELEASE_FOR_SECTION_SYNCHRONIZATION,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_ACQUIRE_FOR_MOD_WRITE,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_RELEASE_FOR_MOD_WRITE,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_ACQUIRE_FOR_CC_FLUSH,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_RELEASE_FOR_CC_FLUSH,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_FAST_IO_CHECK_IF_POSSIBLE,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_NETWORK_QUERY_OPEN,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_MDL_READ,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_MDL_READ_COMPLETE,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_PREPARE_MDL_WRITE,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_MDL_WRITE_COMPLETE,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_VOLUME_MOUNT,
      0,
      FsPreOperation,
      FsPostOperation },

    { IRP_MJ_VOLUME_DISMOUNT,
      0,
      FsPreOperation,
      FsPostOperation },

#endif // TODO

    { IRP_MJ_OPERATION_END }
};

//
//  This defines what we want to filter with FltMgr
//

CONST FLT_REGISTRATION FilterRegistration = {

    sizeof( FLT_REGISTRATION ),         //  Size
    FLT_REGISTRATION_VERSION,           //  Version
    0,                                  //  Flags

    NULL,                               //  Context
    Callbacks,                          //  Operation callbacks

    Fs::Unload,                           //  MiniFilterUnload

    Fs::InstanceSetup,                    //  InstanceSetup
    Fs::InstanceQueryTeardown,            //  InstanceQueryTeardown
    Fs::InstanceTeardownStart,            //  InstanceTeardownStart
    Fs::InstanceTeardownComplete,         //  InstanceTeardownComplete

    NULL,                               //  GenerateFileName
    NULL,                               //  GenerateDestinationFileName
    NULL                                //  NormalizeNameComponent
};

/*************************************************************************
    MiniFilter initialization and unload routines.
*************************************************************************/

#include <skl/kernel.h>

NTSTATUS
DriverEntry (
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
    )
{
	NTSTATUS status;
	do 
	{
		status = skl::runtime::RunTimeInit();
		if (!NT_SUCCESS(status))
			break;

		status = Fs::DriverEntry(DriverObject, RegistryPath);
		if (!NT_SUCCESS(status))
			break;

	} while (false);

	if (!NT_SUCCESS(status)){
		skl::runtime::RunTimeUnInit();
	}
    return status;
}



/*************************************************************************
    MiniFilter callback routines.
*************************************************************************/
FLT_PREOP_CALLBACK_STATUS
FsPreOperation (
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID *CompletionContext
    )
{
    NTSTATUS status;
	UNREFERENCED_PARAMETER(status);
	UNREFERENCED_PARAMETER(Data);
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( CompletionContext );
    return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}
FLT_POSTOP_CALLBACK_STATUS
FsPostOperation (
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_opt_ PVOID CompletionContext,
    _In_ FLT_POST_OPERATION_FLAGS Flags
    )
{
    UNREFERENCED_PARAMETER( Data );
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( CompletionContext );
    UNREFERENCED_PARAMETER( Flags );


    return FLT_POSTOP_FINISHED_PROCESSING;
}

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

NTSTATUS FLTAPI Fs::InstanceSetup(_In_ PCFLT_RELATED_OBJECTS FltObjects, _In_ FLT_INSTANCE_SETUP_FLAGS Flags, _In_ DEVICE_TYPE VolumeDeviceType, _In_ FLT_FILESYSTEM_TYPE VolumeFilesystemType)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(Flags);
	UNREFERENCED_PARAMETER(VolumeDeviceType);
	UNREFERENCED_PARAMETER(VolumeFilesystemType);

	PAGED_CODE();

	return STATUS_SUCCESS;
}

VOID FLTAPI Fs::InstanceTeardownStart(_In_ PCFLT_RELATED_OBJECTS FltObjects, _In_ FLT_INSTANCE_TEARDOWN_FLAGS Flags)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(Flags);

	PAGED_CODE();
}

VOID FLTAPI Fs::InstanceTeardownComplete(_In_ PCFLT_RELATED_OBJECTS FltObjects, _In_ FLT_INSTANCE_TEARDOWN_FLAGS Flags)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(Flags);

	PAGED_CODE();
}

NTSTATUS FLTAPI Fs::InstanceQueryTeardown(_In_ PCFLT_RELATED_OBJECTS FltObjects, _In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(Flags);

	PAGED_CODE();


	return STATUS_SUCCESS;
}

NTSTATUS Fs::DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	NTSTATUS status;

	UNREFERENCED_PARAMETER(RegistryPath);


	//
	//  Register with FltMgr to tell it our callback routines
	//

	gFsInstance = new Fs();
	if (!gFsInstance){
		status = STATUS_INSUFFICIENT_RESOURCES;
		return status;
	}

	status = FltRegisterFilter(DriverObject,
		&FilterRegistration,
		&gFsInstance->_filterHandle);

	FLT_ASSERT(NT_SUCCESS(status));

	if (NT_SUCCESS(status)) {
		//
		//  Start filtering i/o
		//
		status = FltStartFiltering(gFsInstance->filterHandle());

		if (!NT_SUCCESS(status)) {

			FltUnregisterFilter(gFsInstance->filterHandle());
		}
	}

	return status;
}

NTSTATUS FLTAPI Fs::Unload(_In_ FLT_FILTER_UNLOAD_FLAGS Flags)
{
	UNREFERENCED_PARAMETER(Flags);

	PAGED_CODE();

	if (gFsInstance){
		FltUnregisterFilter(gFsInstance->filterHandle());
		delete gFsInstance;
	}

	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
Fs::Fs() :_filterHandle(nullptr)
{
}
Fs::~Fs()
{
}
