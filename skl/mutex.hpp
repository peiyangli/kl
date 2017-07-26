
/*////////////////////////////////////////////////////////
(c)22016
filename: mutex.hpp
author: pei
version: 0.0.1
date:
description: mutex and spinlock
function/class list:
history:
////////////////////////////////////////////////////////*/

#pragma once

#include "kernel.h"

namespace skl{

	//will sleep for waiting
	class mutex //no copyable
	{
		KMUTEX _m;

		mutex(const mutex&) = delete;
		mutex& operator=(const mutex&) = delete;
		mutex(mutex&&) = delete;
		mutex& operator=(mutex&&) = delete;
	public:
		mutex(){
			KeInitializeMutex(&_m, 0);
		}
		~mutex(){
		}

		inline void lock(){
			KeWaitForSingleObject(&_m, Executive, KernelMode, FALSE, NULL);
		}
		inline void unlock(){
			KeReleaseMutex(&_m, FALSE);
		}
	};

	class mutex_fast //no copyable, nonpaged memory
	{
		mutex_fast(const mutex_fast&) = delete;
		mutex_fast& operator=(const mutex_fast&) = delete;
		mutex_fast(mutex_fast&& r) = delete;
		mutex_fast& operator=(mutex_fast&& r) = delete;

		void swap(mutex_fast& r){
			if (this == &r)return;
			auto temp = r._m;
			r._m = _m;
			_m = temp;
		}

		FAST_MUTEX _m;
	public:
		mutex_fast(){
			ExInitializeFastMutex(&_m);
		}
		~mutex_fast(){}
		inline void lock(){
			ExAcquireFastMutex(&_m);
		}
		inline void unlock(){
			ExReleaseFastMutex(&_m);
		}
	};


	template<class _Mutex>
	class auto_locker{
		auto_locker(const auto_locker&) = delete;
		auto_locker& operator=(const auto_locker&) = delete;
		auto_locker(auto_locker&&) = delete;
		auto_locker& operator=(auto_locker&&) = delete;

		_Mutex& _m;
	public:
		explicit auto_locker(_Mutex& m) :_m(m){
			_m.lock();
		}
		~auto_locker(){
			_m.unlock();
		}
	};

	//raising IRQL, the memory protected must be none-paged
	class spin_lock
	{
		friend class auto_spinlocker;
		spin_lock(const spin_lock&) = delete;
		spin_lock& operator=(const spin_lock&) = delete;
		spin_lock(spin_lock&&) = delete;
		spin_lock& operator=(spin_lock&&) = delete;

		KSPIN_LOCK _lock;
	public:
		spin_lock(){
			KeInitializeSpinLock(&_lock);
		}
// 		inline void lock(){
// 			//KfAcquireSpinLock
// 		}
// 		inline void unlock(){
// 			//KfReleaseSpinLock
// 		}
	};
	class auto_spinlocker
	{
		spin_lock& _slk;
		KLOCK_QUEUE_HANDLE _lh;

		auto_spinlocker(auto_spinlocker const &);
		auto_spinlocker & operator=(auto_spinlocker const &);
	public:
		explicit auto_spinlocker(spin_lock& lock) :_slk(lock), _lh({}){
			KeAcquireInStackQueuedSpinLock(&_slk._lock, &_lh);
		}
		~auto_spinlocker(){
			KeReleaseInStackQueuedSpinLock(&_lh);
		}
	};
}