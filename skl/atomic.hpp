#pragma once

namespace skl{

	//////////////////////////////////////////////////////////////////////////
	template<class _BaseType> class _AtomicInt32
	{
	private:
		mutable long m_Value;

	public:
		typedef _BaseType _UnderlyingType;

		_AtomicInt32(_BaseType val) { m_Value = val; C_ASSERT(sizeof(m_Value) == sizeof(_BaseType)); }
		_AtomicInt32(){}
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
	};

	//#ifdef BZSLIB64
	template<class _BaseType> class _AtomicInt64
	{
	private:
		mutable LONGLONG m_Value;

	public:
		typedef _BaseType _UnderlyingType;

		_AtomicInt64(_BaseType val) { m_Value = val; C_ASSERT(sizeof(m_Value) == sizeof(_BaseType)); }
		_AtomicInt64(){}
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
	};


	typedef _AtomicInt32<int> AtomicInt32;
	typedef _AtomicInt32<unsigned> AtomicUInt32;

	typedef _AtomicInt64<long long> AtomicInt64;
	typedef _AtomicInt64<unsigned long long> AtomicUInt64;


}//namespace skl