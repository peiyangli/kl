#include "kernel.h"


#include"globle_runtime.h"

//#pragma warning(push)
#pragma warning(disable: 4127) // while (false);
#pragma warning(disable: 4316)
//#pragma warning(pop)
namespace skl{
	namespace runtime{
		GlobleRuntime* gInstanceGlobleRuntime;

		NTSTATUS RunTimeInit(){
			NTSTATUS status = STATUS_UNSUCCESSFUL;
			do
			{
				gInstanceGlobleRuntime = new GlobleRuntime;
				if (!gInstanceGlobleRuntime){
					status = STATUS_INSUFFICIENT_RESOURCES;
					break;
				}
				status = STATUS_SUCCESS;
			} while (false);
			return status;
		}

		//call when exit/unload driver
		NTSTATUS RunTimeUnInit(){
			NTSTATUS status = STATUS_UNSUCCESSFUL;
			do
			{
				if (gInstanceGlobleRuntime){
					delete gInstanceGlobleRuntime;
				}
				status = STATUS_SUCCESS;
			} while (false);
			return status;
		}
	}
#define DELAY_ONE_MICROSECOND 	(-10)
#define DELAY_ONE_MILLISECOND	(DELAY_ONE_MICROSECOND*1000)
	VOID SleepFor(LONG msec)
	{
		if (msec == 0)return;
		LARGE_INTEGER my_interval;
		my_interval.QuadPart = DELAY_ONE_MILLISECOND;
		my_interval.QuadPart *= msec;
		KeDelayExecutionThread(KernelMode, 0, &my_interval);
	}

	VOID SleepFor100ns(LONG ns100)
	{
		if (ns100 <= 0)return;
		LARGE_INTEGER my_interval;
		//my_interval.QuadPart = -1;
		my_interval.QuadPart = -ns100;
		KeDelayExecutionThread(KernelMode, 0, &my_interval);
	}
}