/*////////////////////////////////////////////////////////
(c)22016
filename: lookaside_atomic.hpp
author: pei
version: 0.0.1
date:
description: allocate atomic in global runtime lookaside, typically use: shared_ptr
function/class list:
history:
////////////////////////////////////////////////////////*/

#pragma once

#include"kernel.h"
#include"globle_runtime.h"

//#include<wdm.h>
#include<ntddk.h>
namespace skl{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	namespace runtime{
		namespace paged{
			//////////////////////////////////////////////////////////////////////////
			template<class _BaseType> class LookasideAtomic32
			{
			private:
				mutable long m_Value;

			public:
				typedef _BaseType _UnderlyingType;
				typedef typename LookasideAtomic32<_BaseType> _ThisType;

				LookasideAtomic32(_BaseType val) { m_Value = val; C_ASSERT(sizeof(m_Value) == sizeof(_BaseType)); }
				LookasideAtomic32(){}
				operator _BaseType() { return m_Value; }

				_BaseType load()const{ return _InterlockedOr(&m_Value, 0); }
				_BaseType store(_BaseType v){ return _InterlockedExchange(&m_Value, v); }
				_BaseType operator++() { return _InterlockedIncrement(&m_Value); }
				_BaseType operator++(int) { return _InterlockedIncrement(&m_Value) - 1; }
				_BaseType operator--() { return _InterlockedDecrement(&m_Value); }
				_BaseType operator--(int) { return _InterlockedDecrement(&m_Value) + 1; }
				_BaseType operator+=(_BaseType v) { return _InterlockedExchangeAdd(&m_Value, v) + v; }
				_BaseType operator-=(_BaseType v) { return _InterlockedExchangeAdd(&m_Value, -v) - v; }

				_BaseType operator|=(_BaseType v) { return _InterlockedOr(&m_Value, v) | v; }
				_BaseType operator&=(_BaseType v) { return _InterlockedAnd(&m_Value, v) & v; }
				_BaseType operator^=(_BaseType v) { return _InterlockedXor(&m_Value, v) ^ v; }

				_BaseType GetAndOr(_BaseType v) { return _InterlockedOr(&m_Value, v); }
				_BaseType GetAndAnd(_BaseType v) { return _InterlockedAnd(&m_Value, v); }
				_BaseType GetAndXor(_BaseType v) { return _InterlockedXor(&m_Value, v); }

				bool CompareAndExchange(_BaseType oldVal, _BaseType newVal) { return _InterlockedCompareExchange(&m_Value, newVal, oldVal) == oldVal; }

				inline void* operator new(size_t){
					return gInstanceGlobleRuntime->pagedPoolLookAside->New<_ThisType>();
				}
					inline void operator delete(void *ptr, size_t){
					return gInstanceGlobleRuntime->pagedPoolLookAside->Delete<_ThisType>((_ThisType*)ptr);
				}
			};

			//#ifdef BZSLIB64
			template<class _BaseType> class LookasideAtomic64
			{
			private:
				mutable LONGLONG m_Value;

			public:
				typedef _BaseType _UnderlyingType;
				typedef typename LookasideAtomic64<_BaseType> _ThisType;

				LookasideAtomic64(_BaseType val) { m_Value = val; C_ASSERT(sizeof(m_Value) == sizeof(_BaseType)); }
				LookasideAtomic64(){}
				operator _BaseType() { return m_Value; }

				_BaseType load()const{ return _InterlockedOr64(&m_Value, 0); }
				_BaseType store(_BaseType v){ return _InterlockedExchange64(&m_Value, v); }
				_BaseType operator++() { return _InterlockedIncrement64(&m_Value); }
				_BaseType operator++(int) { return _InterlockedIncrement64(&m_Value) - 1; }
				_BaseType operator--() { return _InterlockedDecrement64(&m_Value); }
				_BaseType operator--(int) { return _InterlockedDecrement64(&m_Value) + 1; }
				_BaseType operator+=(_BaseType v) { return _InterlockedExchangeAdd64(&m_Value, v) + v; }
				_BaseType operator-=(_BaseType v) { return _InterlockedExchangeAdd64(&m_Value, -v) - v; }

				_BaseType operator|=(_BaseType v) { return _InterlockedOr64(&m_Value, v) | v; }
				_BaseType operator&=(_BaseType v) { return _InterlockedAnd64(&m_Value, v) & v; }
				_BaseType operator^=(_BaseType v) { return _InterlockedXor64(&m_Value, v) ^ v; }

				_BaseType GetAndOr(_BaseType v) { return _InterlockedOr64(&m_Value, v); }
				_BaseType GetAndAnd(_BaseType v) { return _InterlockedAnd64(&m_Value, v); }
				_BaseType GetAndXor(_BaseType v) { return _InterlockedXor64(&m_Value, v); }

				bool CompareAndExchange(_BaseType oldVal, _BaseType newVal) { return _InterlockedCompareExchange64(&m_Value, newVal, oldVal) == oldVal; }

				inline void* operator new(size_t){
					return gInstanceGlobleRuntime->pagedPoolLookAside->New<_ThisType>();
				}
					inline void operator delete(void *ptr, size_t){
					return gInstanceGlobleRuntime->pagedPoolLookAside->Delete<_ThisType>((_ThisType*)ptr);
				}
			};
		}//namespace paged

		namespace none_paged{
			//////////////////////////////////////////////////////////////////////////
			template<class _BaseType> class LookasideAtomic32
			{
			private:
				mutable long m_Value;

			public:
				typedef _BaseType _UnderlyingType;
				typedef typename LookasideAtomic32<_BaseType> _ThisType;

				LookasideAtomic32(_BaseType val) { m_Value = val; C_ASSERT(sizeof(m_Value) == sizeof(_BaseType)); }
				LookasideAtomic32(){}
				operator _BaseType() { return m_Value; }

				_BaseType load()const{ return _InterlockedOr(&m_Value, 0); }
				_BaseType store(_BaseType v){ return _InterlockedExchange(&m_Value, v); }
				_BaseType operator++() { return _InterlockedIncrement(&m_Value); }
				_BaseType operator++(int) { return _InterlockedIncrement(&m_Value) - 1; }
				_BaseType operator--() { return _InterlockedDecrement(&m_Value); }
				_BaseType operator--(int) { return _InterlockedDecrement(&m_Value) + 1; }
				_BaseType operator+=(_BaseType v) { return _InterlockedExchangeAdd(&m_Value, v) + v; }
				_BaseType operator-=(_BaseType v) { return _InterlockedExchangeAdd(&m_Value, -v) - v; }

				_BaseType operator|=(_BaseType v) { return _InterlockedOr(&m_Value, v) | v; }
				_BaseType operator&=(_BaseType v) { return _InterlockedAnd(&m_Value, v) & v; }
				_BaseType operator^=(_BaseType v) { return _InterlockedXor(&m_Value, v) ^ v; }

				_BaseType GetAndOr(_BaseType v) { return _InterlockedOr(&m_Value, v); }
				_BaseType GetAndAnd(_BaseType v) { return _InterlockedAnd(&m_Value, v); }
				_BaseType GetAndXor(_BaseType v) { return _InterlockedXor(&m_Value, v); }

				bool CompareAndExchange(_BaseType oldVal, _BaseType newVal) { return _InterlockedCompareExchange(&m_Value, newVal, oldVal) == oldVal; }

				inline void* operator new(size_t){
					KdPrint((DBG_TAG"none_paged::LookasideAtomic32 operator new\n"));
					return gInstanceGlobleRuntime->poolLookAside.Malloc<_ThisType>();
				}
				inline void operator delete(void *ptr, size_t){
					KdPrint((DBG_TAG"none_paged::LookasideAtomic32 operator delete\n"));
					return gInstanceGlobleRuntime->poolLookAside.Free((_ThisType*)ptr);
				}
			};

			//#ifdef BZSLIB64
			template<class _BaseType> class LookasideAtomic64
			{
			private:
				mutable LONGLONG m_Value;

			public:
				typedef _BaseType _UnderlyingType;
				typedef typename LookasideAtomic64<_BaseType> _ThisType;

				LookasideAtomic64(_BaseType val) { m_Value = val; C_ASSERT(sizeof(m_Value) == sizeof(_BaseType)); }
				LookasideAtomic64(){}
				operator _BaseType() { return m_Value; }

				_BaseType load()const{ return _InterlockedOr64(&m_Value, 0); }
				_BaseType store(_BaseType v){ return _InterlockedExchange64(&m_Value, v); }
				_BaseType operator++() { return _InterlockedIncrement64(&m_Value); }
				_BaseType operator++(int) { return _InterlockedIncrement64(&m_Value) - 1; }
				_BaseType operator--() { return _InterlockedDecrement64(&m_Value); }
				_BaseType operator--(int) { return _InterlockedDecrement64(&m_Value) + 1; }
				_BaseType operator+=(_BaseType v) { return _InterlockedExchangeAdd64(&m_Value, v) + v; }
				_BaseType operator-=(_BaseType v) { return _InterlockedExchangeAdd64(&m_Value, -v) - v; }

				_BaseType operator|=(_BaseType v) { return _InterlockedOr64(&m_Value, v) | v; }
				_BaseType operator&=(_BaseType v) { return _InterlockedAnd64(&m_Value, v) & v; }
				_BaseType operator^=(_BaseType v) { return _InterlockedXor64(&m_Value, v) ^ v; }

				_BaseType GetAndOr(_BaseType v) { return _InterlockedOr64(&m_Value, v); }
				_BaseType GetAndAnd(_BaseType v) { return _InterlockedAnd64(&m_Value, v); }
				_BaseType GetAndXor(_BaseType v) { return _InterlockedXor64(&m_Value, v); }

				bool CompareAndExchange(_BaseType oldVal, _BaseType newVal) { return _InterlockedCompareExchange64(&m_Value, newVal, oldVal) == oldVal; }

				inline void* operator new(size_t){
					return gInstanceGlobleRuntime->poolLookAside->Malloc<_ThisType>();
				}
					inline void operator delete(void *ptr, size_t){
					return gInstanceGlobleRuntime->poolLookAside->Free<_ThisType>((_ThisType*)ptr);
				}
			};
		}//namespace paged

		typedef none_paged::LookasideAtomic32<unsigned> none_page_atomic_unsigned;
	}//namespace runtime
}//namespace skl