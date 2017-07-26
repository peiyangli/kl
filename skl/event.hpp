#pragma once

#include "kernel.h"
namespace skl{
	class event{
		KEVENT _e;
	public:
		event(){
			KeInitializeEvent(&_e, SynchronizationEvent, TRUE ); //, NotificationEvent, FALSE); //
		}
		void wait(){ //IRQL <= APC_LEVEL
			KeWaitForSingleObject(&_e, Executive, KernelMode, FALSE, NULL);
		}
		NTSTATUS wait_for(LONGLONG ns100){ //IRQL <= DISPATCH_LEVEL
			if (ns100 <= 0)return STATUS_TIMEOUT;
			LARGE_INTEGER t;
			t.QuadPart = -ns100;
			return KeWaitForSingleObject(&_e, Executive, KernelMode, FALSE, &t);
		}
		void notify(){
			KeSetEvent(&_e, 0, FALSE);
		}
	};
}
