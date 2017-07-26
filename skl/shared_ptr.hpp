/*////////////////////////////////////////////////////////
(c)22016
filename: shared_ptr.hpp
author: pei
version: 0.0.1
date:
description: shared_ptr
function/class list:
history:
////////////////////////////////////////////////////////*/

#pragma once

#include "kernel.h"

#include "atomic.hpp"
#include "lookaside_atomic.hpp"
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
//#include <wdm.h>
#include<ntddk.h>
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

	template<typename T>
	class unique_ptr{
		typedef unique_ptr<T> this_type;

		T* _ptr;

		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;
	public:
		unique_ptr(T* ptr) :_ptr(ptr){
		}
		~unique_ptr(){
			if (_ptr) checked_delete(_ptr);
		}
		unique_ptr(unique_ptr&&) :_ptr(r._ptr){
			r._ptr = 0;
		}
		unique_ptr& operator=(unique_ptr&& r){
			r.swap(*this);
			return *this;
		}

		void swap(unique_ptr& r) {
			if (this == &r)return;
			T* ptr = r._ptr;
			r._ptr = _ptr;
			_ptr = ptr;
		}
		T& operator *() const {
			return *_ptr;
		}
		T* operator ->() const {
			return _ptr;
		}
		T* get() const {
			return _ptr;
		}
		T* detach(){
			auto temp = _ptr;
			_ptr = 0;
			return temp;
		}
		explicit operator bool() const{
			return _ptr != 0;
		}
	};


	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	class shared_ptr{

		//static const int value = sizeof(T);

		friend class helper;

		typedef shared_ptr<T> this_type;

		template<class _Ty0>
		friend class shared_ptr;

		T* _ptr;
		//AtomicUInt32* _pn;
		runtime::none_page_atomic_unsigned* _pn;

		shared_ptr(T* ptr, runtime::none_page_atomic_unsigned* pn) :_ptr(ptr), _pn(pn){}

		void dec(){
			if (_pn && --*_pn == 0) {
				if (_ptr)checked_delete(_ptr);
				delete _pn;
			}
		}
	public:
		//int getV(){ return value; }
		shared_ptr() :_ptr(0), _pn(0){}
		shared_ptr(const shared_ptr& r) :_ptr(r._ptr), _pn(r._pn){//copy
			if (_pn)(*_pn)++;
		}
		shared_ptr(shared_ptr&& r) :_ptr(r._ptr), _pn(r._pn){//rref copy
			r._ptr = 0;
			r._pn = 0;
		}
		~shared_ptr(){
			dec();
		}

		void swap(shared_ptr& r) {
			T* ptr = r._ptr;
			auto* pn = r._pn;
			r._ptr = _ptr;
			r._pn = _pn;
			_ptr = ptr;
			_pn = pn;
		}
		shared_ptr& operator=(const shared_ptr& r) {
			this_type(r).swap(*this);
			return *this;
		}

		shared_ptr& operator=(shared_ptr&& r) {
			r.swap(*this);
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		//child
		template <class TChild>
		shared_ptr(const shared_ptr<TChild>& r) :_ptr(r._ptr), _pn(r._pn){//copy
			if (_pn)(*_pn)++;
		}
		template <class TChild>
		shared_ptr(shared_ptr<TChild>&& r) : _ptr(r._ptr), _pn(r._pn){//rref copy
			r._ptr = 0;
			r._pn = 0;
		}
		template <class TChild>
		shared_ptr& operator=(const shared_ptr<TChild>& r) {
			this_type(r).swap(*this);
			return *this;
		}

		template <class TChild>
		shared_ptr& operator=(shared_ptr<TChild>&& r) {
			dec();
			_ptr = r._ptr; _pn = r._pn;
			r._ptr = 0;
			r._pn = 0;
			return *this;
		}
		//////////////////////////////////////////////////////////////////////////

		
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

	//add allocator/deallocator
	class helper{
	public:
		template<class _Ty, POOL_TYPE poolType, 
		class... _Types> static inline
			shared_ptr<_Ty> make_shared(_Types&&... _Args)
		{	// make a shared_ptr
			runtime::none_page_atomic_unsigned* ref = nullptr;
				do
				{
					ref = new runtime::none_page_atomic_unsigned(1);//better from lookaside
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
			runtime::none_page_atomic_unsigned* ref = nullptr;
				do
				{
					ref = new runtime::none_page_atomic_unsigned(1);//better from lookaside, and maybe non-page memory
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
			runtime::none_page_atomic_unsigned* ref = nullptr;
				do
				{
					ref = new runtime::none_page_atomic_unsigned(1);
					if (!ref)break;
					_Ty* ptr = new (poolType)_Ty[size];
					if (!ptr)break;
					return shared_ptr<_Ty>(ptr, ref);
				} while (false);
				if (ref)delete ref;
				return shared_ptr<_Ty>();
		}
	};

	template<class _Ty, POOL_TYPE poolType,
	class... _Types> inline
		shared_ptr<_Ty> make_shared(_Types&&... _Args)
	{
		return helper::make_shared<_Ty, poolType>(_Args...);
	}

	template<class _Ty,
	class... _Types> inline
		shared_ptr<_Ty> make_shared(_Types&&... _Args)
	{
		return helper::make_shared<_Ty>(_Args...);
	}

	template<class _Ty, int size, POOL_TYPE poolType = NonPagedPool> inline
		shared_ptr<_Ty> make_shared_array()
	{
		return helper::make_shared_array<_Ty, Size, poolType>();
	}
	//#pragma warning(enable: 4127) // while (false);
#pragma warning(pop)

}//namespace skl
