#pragma once

namespace skl{

	//////////////////////////////////////////////////////////////////////////
	template<class _Ty> class _atomic_int32
	{
	private:
		mutable long m_Value;

	public:
		typedef _Ty _UnderlyingType;

		_atomic_int32(_Ty val) { m_Value = val; C_ASSERT(sizeof(m_Value) == sizeof(_Ty)); }
		_atomic_int32(){}
		operator _Ty() { return m_Value; }

		_Ty load()const{ return _InterlockedOr(&m_Value, 0); }
		_Ty store(_Ty v){ return _InterlockedExchange(&m_Value, v); }
		_Ty operator++() { return _InterlockedIncrement(&m_Value); }
		_Ty operator++(int) { return _InterlockedIncrement(&m_Value) - 1; }
		_Ty operator--() { return _InterlockedDecrement(&m_Value); }
		_Ty operator--(int) { return _InterlockedDecrement(&m_Value) + 1; }
		_Ty operator+=(_Ty v) { return _InterlockedExchangeAdd(&m_Value, v) + v; }
		_Ty operator-=(_Ty v) { return _InterlockedExchangeAdd(&m_Value, -v) - v; }

		_Ty operator|=(_Ty v) { return _InterlockedOr(&m_Value, v) | v; }
		_Ty operator&=(_Ty v) { return _InterlockedAnd(&m_Value, v) & v; }
		_Ty operator^=(_Ty v) { return _InterlockedXor(&m_Value, v) ^ v; }

		_Ty GetAndOr(_Ty v) { return _InterlockedOr(&m_Value, v); }
		_Ty GetAndAnd(_Ty v) { return _InterlockedAnd(&m_Value, v); }
		_Ty GetAndXor(_Ty v) { return _InterlockedXor(&m_Value, v); }

		bool CompareAndExchange(_Ty oldVal, _Ty newVal) { return _InterlockedCompareExchange(&m_Value, newVal, oldVal) == oldVal; }
	};

	//#ifdef BZSLIB64
	template<class _Ty> class _atomic_int64
	{
	private:
		mutable LONGLONG m_Value;

	public:
		typedef _Ty _UnderlyingType;

		_atomic_int64(_Ty val) { m_Value = val; C_ASSERT(sizeof(m_Value) == sizeof(_Ty)); }
		_atomic_int64(){}
		operator _Ty() { return m_Value; }

		_Ty load()const{ return _InterlockedOr64(&m_Value, 0); }
		_Ty store(_Ty v){ return _InterlockedExchange64(&m_Value, v); }
		_Ty operator++() { return _InterlockedIncrement64(&m_Value); }
		_Ty operator++(int) { return _InterlockedIncrement64(&m_Value) - 1; }
		_Ty operator--() { return _InterlockedDecrement64(&m_Value); }
		_Ty operator--(int) { return _InterlockedDecrement64(&m_Value) + 1; }
		_Ty operator+=(_Ty v) { return _InterlockedExchangeAdd64(&m_Value, v) + v; }
		_Ty operator-=(_Ty v) { return _InterlockedExchangeAdd64(&m_Value, -v) - v; }

		_Ty operator|=(_Ty v) { return _InterlockedOr64(&m_Value, v) | v; }
		_Ty operator&=(_Ty v) { return _InterlockedAnd64(&m_Value, v) & v; }
		_Ty operator^=(_Ty v) { return _InterlockedXor64(&m_Value, v) ^ v; }

		_Ty GetAndOr(_Ty v) { return _InterlockedOr64(&m_Value, v); }
		_Ty GetAndAnd(_Ty v) { return _InterlockedAnd64(&m_Value, v); }
		_Ty GetAndXor(_Ty v) { return _InterlockedXor64(&m_Value, v); }

		bool CompareAndExchange(_Ty oldVal, _Ty newVal) { return _InterlockedCompareExchange64(&m_Value, newVal, oldVal) == oldVal; }
	};


	typedef _atomic_int32<int> atomic_int32;
	typedef _atomic_int32<unsigned> atomic_uint32;

	typedef _atomic_int64<long long> atomic_int64;
	typedef _atomic_int64<unsigned long long> atomic_uint64;


}//namespace skl