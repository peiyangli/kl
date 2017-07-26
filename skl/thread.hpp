/*////////////////////////////////////////////////////////
(c)22016
filename: thread.hpp
author: pei
version: 0.0.1
date:
description: thread
function/class list:
history:
////////////////////////////////////////////////////////*/

#pragma once


#include "shared_ptr.hpp"
#include "refholder.hpp"
namespace skl{
#pragma warning(push)
#pragma warning(disable: 4127) // while (false);
	template<class _KeyT>
	class shared_ref_item : public ref_item{
		friend class thread;
	public:
		shared_ref_item(){}
		shared_ref_item(const _KeyT& k) : first(k), ref_item(1){
		}
		_KeyT first;
	};

	//PASSIVE_LEVEL
	class thread{
		
		PKTHREAD _thread;
		template<class _Fn>
		static VOID ThreadProc(
			_In_ PVOID StartContext
			){
			if (StartContext){
				auto holder = (shared_ref_item<shared_ptr<_Fn>>*)StartContext;
				(*(holder->first))();
				holder->release();
			}
			PsTerminateSystemThread(STATUS_SUCCESS);
		}

		template<class _Fn, class _Param>
		static VOID ThreadProc(
			_In_ PVOID StartContext
			){
			if (StartContext){
				auto holder = (shared_ref_item<shared_ptr<skl::pair<_Fn, _Param>>>*)StartContext;
				((holder->first->first))(holder->first->second);
				holder->release();
			}
			PsTerminateSystemThread(STATUS_SUCCESS);
		}

		thread(const thread&) = delete;
		thread& operator=(const thread&) = delete;
	public:
		thread() : _thread(NULL){}
		template<class _Fn>
		explicit thread(_Fn&& _Fx) : _thread(NULL)
		{
			do 
			{
				HANDLE  hThread;
				auto ptr = helper::make_shared<_Fn>(_Fx);
				if (!ptr)
					break;
				auto holder = new shared_ref_item<shared_ptr<_Fn>>(ptr);
				if (!holder)
					break;
				auto status = PsCreateSystemThread(&hThread, (ACCESS_MASK)THREAD_ALL_ACCESS, NULL, NULL, NULL, ThreadProc<_Fn>, holder);
				if (!NT_SUCCESS(status)) {
					holder->release();
					break;
				}
				//ok
				status = ObReferenceObjectByHandle(hThread, THREAD_ALL_ACCESS, NULL,
					KernelMode, (PVOID*)&_thread, NULL);
				if (!NT_SUCCESS(status)) {
					_thread = NULL;
				}
				ZwClose(hThread);
			} while (false);
		}

		template<class _Fn, class _Param>
		explicit thread(_Fn&& _Fx, _Param&& param) : _thread(NULL)
		{
			do
			{
				HANDLE  hThread;
				auto ptr = helper::make_shared<skl::pair<_Fn, _Param>>(_Fx, param);
				if (!ptr)
					break;
				auto holder = new shared_ref_item<shared_ptr<skl::pair<_Fn, _Param>>>(ptr);
				if (!holder)
					break;
				auto status = PsCreateSystemThread(&hThread, (ACCESS_MASK)THREAD_ALL_ACCESS, NULL, NULL, NULL, ThreadProc<_Fn, _Param>, holder);
				if (!NT_SUCCESS(status)) {
					holder->release();
					break;
				}
				//ok
				status = ObReferenceObjectByHandle(hThread, THREAD_ALL_ACCESS, NULL,
					KernelMode, (PVOID*)&_thread, NULL);
				if (!NT_SUCCESS(status)) {
					_thread = NULL;
				}
				ZwClose(hThread);
			} while (false);
		}

// 		template<class _Fn,
// 		class... _Args>
// 			explicit thread(_Fn&& _Fx, _Args&&... _Ax) : _thread(NULL)
// 		{
// 			skl::forward(_Ax);
// 		}

		//without wait for thread, not safe if you now unload driver, call detach
		~thread(){
			if (_thread){
				//KeWaitForSingleObject(_thread, Executive, KernelMode, FALSE, NULL);
				ObDereferenceObject(_thread);
				_thread = NULL;
			}
		}

		//wait for thread to exit
		void detach(){
			if (_thread){
				KeWaitForSingleObject(_thread, Executive, KernelMode, FALSE, NULL);
				ObDereferenceObject(_thread);
				_thread = NULL;
			}
		}


		template<class _Func,
		class... _Types>
			bool then(_Func&& func,  _Types&&... _Args)
		{	
			if (!_thread)
				return false;
			KeWaitForSingleObject(_thread, Executive, KernelMode, FALSE, NULL);
			ObDereferenceObject(_thread);
			_thread = NULL;

			func(forward<_Types>(_Args)...);

			return true;
		}

		operator bool()const{
			return _thread != NULL;
		}

		void swap(thread& r){
			if (this == &r)return;
			auto temp = _thread;
			_thread = r._thread;
			r._thread = temp;
		}

		thread(thread&& r):_thread(r._thread){
			r._thread = NULL;
		}
		thread& operator=(thread&& r){
			r.swap(*this);
			return *this;
		}
	public:
		static inline HANDLE get_id(){
			return PsGetCurrentThreadId();
		}
		//<= APC_LEVEL
		static VOID sleep_for_100ns(LONGLONG ns100)
		{
			if (ns100 <= 0)return;
			LARGE_INTEGER my_interval;
			//my_interval.QuadPart = -1;
			my_interval.QuadPart = -ns100;
			KeDelayExecutionThread(KernelMode, 0, &my_interval);
		}
	};
#pragma warning(pop)
}//namespace skl


//to do
//KeWaitForMultipleObjects
//KeWaitForSingleObject
//KeInitializeEvent
//ObReferenceObjectByHandle
//ObDereferenceObject
//ObReferenceObjectByHandle
//KeInitializeSemaphore
//KeReadStateSemaphore
//KeReleaseSemaphore
//KeInitializeMutex
//KeReleaseMutex
//KeInitializeSpinLock
//KeReleaseSpinLock