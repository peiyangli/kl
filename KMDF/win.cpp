#include "win.h"
#pragma warning(disable: 4127) // while (false);
//////////////////////////////////////////////////////////////////////////
typedef NTSTATUS(*QUERY_INFO_PROCESS) (
	__in HANDLE ProcessHandle,
	__in PROCESSINFOCLASS ProcessInformationClass,
	__out_bcount(ProcessInformationLength) PVOID ProcessInformation,
	__in ULONG ProcessInformationLength,
	__out_opt PULONG ReturnLength
	);

typedef enum _SYSTEM_INFORMATION_CLASS_SOME {
	SystemBasicInformation = 0,
	SystemPerformanceInformation = 2,
	SystemTimeOfDayInformation = 3,
	SystemProcessInformation = 5,
	SystemProcessorPerformanceInformation = 8,
	SystemInterruptInformation = 23,
	SystemExceptionInformation = 33,
	SystemRegistryQuotaInformation = 37,
	SystemLookasideInformation = 45,
	SystemPolicyInformation = 134,
} SYSTEM_INFORMATION_CLASS_SOME;

typedef struct _SYSTEM_PROCESS_INFORMATION {
	ULONG NextEntryOffset;
	unsigned char Reserved1[52];
	PVOID Reserved2[3];
	HANDLE UniqueProcessId;
	PVOID InheritedFromProcessId;
	ULONG HandleCount;
	unsigned char Reserved4[4];
	PVOID Reserved5[11];
	SIZE_T PeakPagefileUsage;
	SIZE_T PrivatePageCount;
	LARGE_INTEGER Reserved6[6];
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;
// 
// typedef struct _SYSTEM_PROCESSES
//{
// 	ULONG          NextEntryDelta;          //���ɽṹ���е�ƫ������  
// 	ULONG          ThreadCount;             //�߳���Ŀ��  
// 	ULONG          Reserved1[6];
// 	LARGE_INTEGER  CreateTime;              //����ʱ�䣻  
// 	LARGE_INTEGER  UserTime;                //�û�ģʽ(Ring 3)��CPUʱ�䣻  
// 	LARGE_INTEGER  KernelTime;              //�ں�ģʽ(Ring 0)��CPUʱ�䣻  
// 	UNICODE_STRING ProcessName;             //�������ƣ�  
// 	KPRIORITY      BasePriority;            //��������Ȩ��  
// 	HANDLE         ProcessId;               //���̱�ʶ����  
// 	HANDLE         InheritedFromProcessId;  //�����̵ı�ʶ����  
// 	ULONG          HandleCount;             //�����Ŀ��  
// 	ULONG          Reserved2[2];
// 	VM_COUNTERS    VmCounters;              //����洢���Ľṹ��  
// 	IO_COUNTERS    IoCounters;              //IO�����ṹ��  
// 	SYSTEM_THREADS Threads[1];              //��������̵߳Ľṹ���飻  
//}SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;
// 
// 

typedef NTSTATUS(*Query_System_Information_Proc)(
	_In_      SYSTEM_INFORMATION_CLASS_SOME SystemInformationClass,
	_Inout_   PVOID                    SystemInformation,
	_In_      ULONG                    SystemInformationLength,
	_Out_opt_ PULONG                   ReturnLength
	);
//////////////////////////////////////////////////////////////////////////
QUERY_INFO_PROCESS ZwQueryInformationProcess = NULL;
Query_System_Information_Proc ZwQuerySystemInformation = NULL;
//ProcessImageName PUNICODE_STRING, 
NTSTATUS GetProcessImageName(HANDLE ProcessHdl, PVOID ProcessImageName, ULONG ProcessImageNameLen, PULONG ProcessImageNameLenRt)
{
	PAGED_CODE(); // this eliminates the possibility of the IDLE Thread/Process
	if (NULL == ZwQueryInformationProcess) {
		UNICODE_STRING routineName;
		RtlInitUnicodeString(&routineName, L"ZwQueryInformationProcess");
		ZwQueryInformationProcess =
			(QUERY_INFO_PROCESS)MmGetSystemRoutineAddress(&routineName);
		if (NULL == ZwQueryInformationProcess) {
			DbgPrint("Cannot resolve ZwQueryInformationProcess/n");
			return STATUS_NOT_SUPPORTED;
		}
	}

	return ZwQueryInformationProcess(ProcessHdl, ProcessImageFileName, ProcessImageName, ProcessImageNameLen, ProcessImageNameLenRt);
}

NTSTATUS RetrieveNameByEPROCESS(PEPROCESS eProcess, PVOID ProcessImageName, ULONG ProcessImageNameLen, PULONG ProcessImageNameLenRt){

	auto status = STATUS_SUCCESS;

	HANDLE hProcessHandle = NULL;
	do
	{
		status = ObOpenObjectByPointer(eProcess, OBJ_KERNEL_HANDLE, NULL, 0, 0, KernelMode, &hProcessHandle);
		if ((!NT_SUCCESS(status)) || (!hProcessHandle)){
			//ObDereferenceObject(eProcess);
			hProcessHandle = NULL;
			status = STATUS_INTERNAL_ERROR;
			break;
		}
		status = GetProcessImageName(hProcessHandle, ProcessImageName, ProcessImageNameLen, ProcessImageNameLenRt);
		if (!NT_SUCCESS(status)){
			break;
		}
	} while (false);

	if (hProcessHandle){
		ZwClose(hProcessHandle);
	}
	return status;
}

NTSTATUS RetrieveNameByPID(HANDLE pid, PVOID ProcessImageName, ULONG ProcessImageNameLen, PULONG ProcessImageNameLenRt){

	auto status = STATUS_SUCCESS;

	HANDLE hProcessHandle = NULL;
	do
	{
		OBJECT_ATTRIBUTES ObjectAttributes;
		InitializeObjectAttributes(&ObjectAttributes, 0, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 0, 0);
		CLIENT_ID clientid = { pid, 0 };

		status = ZwOpenProcess(&hProcessHandle, PROCESS_ALL_ACCESS, &ObjectAttributes, &clientid);
		if ((!NT_SUCCESS(status)) || (!hProcessHandle)){
			//ObDereferenceObject(eProcess);
			hProcessHandle = NULL;
			status = STATUS_INTERNAL_ERROR;
			break;
		}
		status = GetProcessImageName(hProcessHandle, ProcessImageName, ProcessImageNameLen, ProcessImageNameLenRt);
		if (!NT_SUCCESS(status)){
			break;
		}
	} while (false);

	if (hProcessHandle){
		ZwClose(hProcessHandle);
	}
	return status;
}