#pragma once

#include "kernel.hpp"

#include "atomic.hpp"

// 
// 
// //////////////////////////////////////////////////////////////////////////
// #pragma pack(push,_CRT_PACKING)
// #pragma warning(push,3)
// #pragma push_macro("new")
// #undef new
// 
// namespace skl{
// 	// TEMPLATE CLASS integral_constant
// 	template<class _Ty,
// 		_Ty _Val>
// 	struct integral_constant
// 	{	// convenient template for integral constant types
// 		static const _Ty value = _Val;
// 
// 		typedef _Ty value_type;
// 		typedef integral_constant<_Ty, _Val> type;
// 
// 		operator value_type() const
// 		{	// return stored value
// 			return (value);
// 		}
// 	};
// 
// 	typedef integral_constant<bool, true> true_type;
// 	typedef integral_constant<bool, false> false_type;
// 
// 
// 	// TEMPLATE CLASS _Cat_base
// 	template<bool>
// 	struct _Cat_base
// 		: false_type
// 	{	// base class for type predicates
// 	};
// 
// 	template<>
// 	struct _Cat_base<true>
// 		: true_type
// 	{	// base class for type predicates
// 	};
// 
// 
// 	// TEMPLATE CLASS is_lvalue_reference
// 	template<class _Ty>
// 	struct is_lvalue_reference
// 		: false_type
// 	{	// determine whether _Ty is an lvalue reference
// 	};
// 
// 	template<class _Ty>
// 	struct is_lvalue_reference<_Ty&>
// 		: true_type
// 	{	// determine whether _Ty is an lvalue reference
// 	};
// 
// 	// TEMPLATE CLASS is_rvalue_reference
// 	template<class _Ty>
// 	struct is_rvalue_reference
// 		: false_type
// 	{	// determine whether _Ty is an rvalue reference
// 	};
// 
// 	template<class _Ty>
// 	struct is_rvalue_reference<_Ty&&>
// 		: true_type
// 	{	// determine whether _Ty is an rvalue reference
// 	};
// 
// 	// TEMPLATE CLASS is_reference
// 	template<class _Ty>
// 	struct is_reference
// 		: _Cat_base<is_lvalue_reference<_Ty>::value
// 		|| is_rvalue_reference<_Ty>::value>
// 	{	// determine whether _Ty is a reference
// 	};
// 
// 
// 
// 	// TEMPLATE remove_reference
// 	template<class _Ty>
// 	struct remove_reference
// 	{	// remove reference
// 		typedef _Ty type;
// 	};
// 
// 	template<class _Ty>
// 	struct remove_reference<_Ty&>
// 	{	// remove reference
// 		typedef _Ty type;
// 	};
// 
// 	template<class _Ty>
// 	struct remove_reference<_Ty&&>
// 	{	// remove rvalue reference
// 		typedef _Ty type;
// 	};
// 
// 	// TEMPLATE STRUCT _Copy_cv
// 	template<class _Tgt,
// 	class _Src>
// 	struct _Copy_cv
// 	{	// plain version
// 		typedef typename remove_reference<_Tgt>::type _Tgtx;
// 		typedef _Tgtx& type;
// 	};
// 
// 	template<class _Tgt,
// 	class _Src>
// 	struct _Copy_cv<_Tgt, const _Src>
// 	{	// const version
// 		typedef typename remove_reference<_Tgt>::type _Tgtx;
// 		typedef const _Tgtx& type;
// 	};
// 
// 	template<class _Tgt,
// 	class _Src>
// 	struct _Copy_cv<_Tgt, volatile _Src>
// 	{	// volatile version
// 		typedef typename remove_reference<_Tgt>::type _Tgtx;
// 		typedef volatile _Tgtx& type;
// 	};
// 
// 	template<class _Tgt,
// 	class _Src>
// 	struct _Copy_cv<_Tgt, const volatile _Src>
// 	{	// const volatile version
// 		typedef typename remove_reference<_Tgt>::type _Tgtx;
// 		typedef const volatile _Tgtx& type;
// 	};
// 
// 	template<class _Tgt,
// 	class _Src>
// 	struct _Copy_cv<_Tgt, _Src&>
// 	{	// remove reference from second argument
// 		typedef typename _Copy_cv<_Tgt, _Src>::type type;
// 	};
// 
// 
// 	//////////////////////////////////////////////////////////////////////////
// #define _NOEXCEPT
// 	// TEMPLATE FUNCTION forward
// 	template<class _Ty> inline
// 		_Ty&& forward(typename remove_reference<_Ty>::type& _Arg)
// 	{	// forward an lvalue
// 			return (static_cast<_Ty&&>(_Arg));
// 	}
// 
// 	template<class _Ty> inline
// 		_Ty&& forward(typename remove_reference<_Ty>::type&& _Arg) _NOEXCEPT
// 	{	// forward anything
// 		static_assert(!is_lvalue_reference<_Ty>::value, "bad forward call");
// 		return (static_cast<_Ty&&>(_Arg));
// 	}
// 
// 		// TEMPLATE FUNCTION move
// 		template<class _Ty> inline
// 		typename remove_reference<_Ty>::type&&
// 		move(_Ty&& _Arg) _NOEXCEPT
// 	{	// forward _Arg as movable
// 		return ((typename remove_reference<_Ty>::type&&)_Arg);
// 	}
// 
// 		// TEMPLATE FUNCTION _Move
// 		template<class _Ty> inline
// 		typename remove_reference<_Ty>::type&&
// 		_Move(_Ty&& _Arg) _NOEXCEPT
// 	{	// forward _Arg as movable
// 		return ((typename remove_reference<_Ty>::type&&)_Arg);
// 	}
// 
// 
// 
// 		//////////////////////////////////////////////////////////////////////////
// 		template<class T> inline void checked_delete(T * x)
// 	{
// 			// intentionally complex - simplification causes regressions
// 			typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
// 			(void) sizeof(type_must_be_complete);
// 			delete x;
// 	}
// 
// }//namespace skl
// //////////////////////////////////////////////////////////////////////////
// 
// 
// #pragma pop_macro("new")
// #pragma warning(pop)
// #pragma pack(pop)

#include "type_traits.hpp"

//////////////////////////////////////////////////////////////////////////
namespace skl{

	//////////////////////////////////////////////////////////////////////////
	template<class T> inline void checked_delete(T * x)
	{
		// intentionally complex - simplification causes regressions
		typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
		(void) sizeof(type_must_be_complete);
		delete x;
	}
	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	class shared_ptr{

		static const int value = sizeof(T);

		friend class helper;

		typedef shared_ptr<T> this_type;

		T* _ptr;
		AtomicUInt32* _pn;

		shared_ptr(T* ptr, AtomicUInt32* pn) :_ptr(ptr), _pn(pn){}
	public:
		int getV(){ return value; }
		shared_ptr() :_ptr(0), _pn(0){}
		shared_ptr(const shared_ptr& r) :_ptr(r._ptr), _pn(r._pn){//copy
			if (_pn)(*_pn)++;
		}
		shared_ptr(shared_ptr&& r) :_ptr(r._ptr), _pn(r._pn){//rref copy
			r._ptr = 0;
			r._pn = 0;
		}
		~shared_ptr(){
			if (_pn && --*_pn == 0) {
				checked_delete(_ptr);
				delete _pn;
			}
		}
		void swap(shared_ptr& r) {
			T* ptr = r._ptr;
			AtomicUInt32* pn = r._pn;
			r._ptr = _ptr;
			r._pn = _pn;
			_ptr = ptr;
			_pn = pn;
		}
		shared_ptr& operator=(const shared_ptr& r) {
			this_type(r).swap(*this);
			return *this;
		}

		
		//check null before call
		T& operator *() const {
			return *_ptr;
		}
		T* operator ->() const {
			return _ptr;
		}
		T* get() const {
			return _ptr;
		}
		explicit operator bool() const{
			return _ptr != 0;
		}
		long use_count() const{
			return _pn ? *_pn : 0;
		}
	};

#pragma warning(push)
#pragma warning(disable: 4127) // while (false);

	class helper{
	public:
		template<class _Ty, POOL_TYPE poolType, 
		class... _Types> static inline
			shared_ptr<_Ty> make_shared(_Types&&... _Args)
		{	// make a shared_ptr
				AtomicUInt32* ref = nullptr;
				do
				{
					ref = new (poolType)AtomicUInt32(1);
					if (!ref)break;
					_Ty* ptr = new (poolType)_Ty(forward<_Types>(_Args)...);
					if (!ptr)break;
					return shared_ptr<_Ty>(ptr, ref);
				} while (false);
				if (ref)delete ref;
				return shared_ptr<_Ty>();
		}

		template<class _Ty,
		class... _Types> static inline
			shared_ptr<_Ty> make_shared(_Types&&... _Args)
		{	// make a shared_ptr
				AtomicUInt32* ref = nullptr;
				do
				{
					ref = new AtomicUInt32(1);
					if (!ref)break;
					_Ty* ptr = new _Ty(forward<_Types>(_Args)...);
					if (!ptr)break;
					return shared_ptr<_Ty>(ptr, ref);
				} while (false);
				if (ref)delete ref;
				return shared_ptr<_Ty>();
		}


		//
		template<class _Ty, int size, POOL_TYPE poolType = NonPagedPool> static inline
			shared_ptr<_Ty> make_shared_array()
		{	// make a shared_ptr
				AtomicUInt32* ref = nullptr;
				do
				{
					ref = new (poolType)AtomicUInt32(1);
					if (!ref)break;
					_Ty* ptr = new (poolType)_Ty[size];
					if (!ptr)break;
					return shared_ptr<_Ty>(ptr, ref);
				} while (false);
				if (ref)delete ref;
				return shared_ptr<_Ty>();
		}
	};

	//#pragma warning(enable: 4127) // while (false);
#pragma warning(pop)

}//namespace skl

