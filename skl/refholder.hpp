/*////////////////////////////////////////////////////////
(c)22016
filename: refholder.hpp
author: pei
version: 0.0.1
date:
description: internal use, a ref-count-based class
function/class list:
history:
////////////////////////////////////////////////////////*/

#pragma once


#include "atomic.hpp"

namespace skl{

	template<class RefClass>
	class ref_holder{
		RefClass* _ptr;
	public:
		ref_holder(RefClass* ptr = nullptr) : _ptr(ptr){
			if (_ptr)_ptr->retain();
		}
		ref_holder(const ref_holder& r) : _ptr(r._ptr){
			if (_ptr)_ptr->retain();
		}
		ref_holder(ref_holder&& r) : _ptr(r._ptr){
			r._ptr = 0;
		}
		ref_holder& operator=(const ref_holder& r){
			if (this != &r){ //ref_holder(r).swap(this)
				if (_ptr)_ptr->release();
				_ptr = r._ptr;
				if (_ptr)_ptr->retain();
			}
			return *this;
		}
		ref_holder& operator=(ref_holder&& r){
			if (this != &r){ //r.swap(*this)
				if (_ptr)_ptr->release();
				_ptr = r._ptr;
				r._ptr = 0;
			}
			return *this;
		}
		~ref_holder(){
			if (_ptr)_ptr->release();
		}
		RefClass& operator *() const {
			return *_ptr;
		}
		RefClass* operator ->() const {
			return _ptr;
		}
		RefClass* get() const {
			return _ptr;
		}
		explicit operator bool() const{
			return _ptr != 0;
		}
	};

	class ref_item{
		ref_item(const ref_item&) = delete;
		ref_item(ref_item&&) = delete;
		ref_item& operator=(const ref_item&) = delete;
		ref_item& operator=(ref_item&&) = delete;

	protected:
		atomic_int32 _pn;
		virtual ~ref_item(){}
	public:
		ref_item(long n = 0) :_pn(n){
		}
	protected:
		long retain() {
			return ++_pn;
		}
		long release() {
			register long r = --_pn;
			if (r < 1){
				delete this;
			}
			return r;
		}
	};

	template<class _T>
	inline bool check_valid(const _T&){
		return true;
	}

// 	template<typename T, typename... Args>
// 	struct VarStruct{
// 		VarStruct<Args...> _next;
// 		T _this;
// 	};
}

namespace skl{
}
