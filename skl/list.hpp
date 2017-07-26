/*////////////////////////////////////////////////////////
(c)22016
filename: list.hpp
author: pei
version: 0.0.1
date: 
description: list template
function/class list:
history:
////////////////////////////////////////////////////////*/
#pragma once

//#include"shared_ptr.hpp"

//#include "globle_runtime.h"
#pragma warning(push)
#pragma warning(disable: 4127) // while (false);
namespace skl{

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	class list_item{
	public:
		list_item(const T& v)
			: _v(v){}
		list_item(T&& v)
			: _v(skl::move(v)){}
		~list_item(){}

		LIST_ENTRY  _listEntry;
		T _v;
	};
	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	class slist_item{
	public:
		slist_item(const T& v)
			: _v(v){}
		slist_item(T&& v)
			: _v(skl::move(v)){}
		~slist_item(){}

		SINGLE_LIST_ENTRY  _listEntry;
		T _v;
	};

	//////////////////////////////////////////////////////////////////////////
	template<typename T, POOL_TYPE poolType = NonPagedPool>
	class stack{
		typedef stack<T> _ThisType;
		typedef typename slist_item<T> item_type;

		SINGLE_LIST_ENTRY _head;
	public:
		stack(){
			_head.Next = NULL;
		}
		~stack(){
			clear();
		}
		void clear(){
			while (auto entry = PopEntryList(&_head)) {
				auto item = (CONTAINING_RECORD(entry, item_type, _listEntry));
				if (item)delete item;
			}
		}
		bool push(const T& v){ //create
			auto item = new(poolType)item_type(v);
			if (!item)return false;
			PushEntryList(&_head, &item->_listEntry);
			return true;
		}
		bool pop(T& v){ //get top should delete outside
			auto entry = PopEntryList(&_head);
			if (!entry)return false;
			auto item = (CONTAINING_RECORD(entry, item_type, _listEntry));
			v =  skl::move(item->_v);
			if(item)delete item;
			return true;
		}
		bool empty(){
			return _head.Next == NULL;
		}

		void swap(_ThisType& other){
			if (&other == this)return;
			swap(_head, other._head);
		}


		class iterator{
			friend class stack;
		protected:
			PSINGLE_LIST_ENTRY _head;
			PSINGLE_LIST_ENTRY _current;
		public:
			iterator(PSINGLE_LIST_ENTRY head, PSINGLE_LIST_ENTRY c) :_head(head), _current(c){
			}
			bool operator==(const iterator& r){
				return _current == r._current;
			}
			bool operator!=(const iterator& r){
				return _current != r._current;
			}
			iterator& operator++(){
				_current = _current->Next;
				return *this;
			}

			T& operator *() {
				auto item = (CONTAINING_RECORD(_current, item_type, _listEntry));
				return item->_v;
			}
			T* operator ->() {
				auto item = (CONTAINING_RECORD(_current, item_type, _listEntry));
				return &item->_v;
			}
			explicit operator bool() const{
				return _current != NULL;
			}
		};
		iterator begin(){
			iterator tmp(&_head, _head.Next);
			//tmp._current = _head.Flink;
			//tmp.value = itr_type(CONTAINING_RECORD(tmp._current, item_type, _listEntry));
			return skl::move(tmp);
		}
		iterator end(){
			//return itr_end;
			return iterator(&_head, NULL);
		}
	};
	//////////////////////////////////////////////////////////////////////////


	template<typename T, POOL_TYPE poolType = NonPagedPool>
	class queue{

		typedef queue<T> _ThisType;
		typedef typename list_item<T> item_type;


		LIST_ENTRY _head;
		queue(const queue&);
		queue& operator = (const queue&);
	public:
		queue(){
			InitializeListHead(&_head);
		}
		~queue(){
			clear();
		}
		void clear(){
			while (!IsListEmpty(&_head))
			{
				auto entry = RemoveHeadList(&_head);
				auto item = CONTAINING_RECORD(entry, item_type, _listEntry);
				if (item)
					delete item;
			}
		}
		bool empty(){
			return TRUE == IsListEmpty(&_head);
		}
		void swap(_ThisType& other){
			if (&other == this)return;
			auto F = _head.Flink;
			auto B = _head.Blink;
			auto f = other._head.Flink;
			auto b = other._head.Blink;

			auto otherempty = IsListEmpty(&other._head);
			auto thisempty = IsListEmpty(&_head);
			if (otherempty){
				InitializeListHead(&_head);
			}
			else {
				_head.Flink = f;
				_head.Blink = b;
				f->Blink = &_head;
				b->Flink = &_head;
			}

			if (thisempty){
				InitializeListHead(&other._head);
			}
			else {
				other._head.Flink = F;
				other._head.Blink = B;
				F->Blink = &other._head;
				B->Flink = &other._head;
			}
		}

		bool push_back(const T& v){ //create
			auto item = new(poolType)item_type(v);
			if (!item)return false;
			InsertTailList(&_head, &item->_listEntry);
			return true;
		}
// 		bool push_front(const T& v){//return back
// 			auto item = new(poolType)item_type(v);
// 			if (!item)return false;
// 			InsertHeadList(&_head, &item->_listEntry);
// 			return true;
// 		}
		bool pop_front(T& v){ //get top should delete outside
			if (IsListEmpty(&_head))
				return false;
			auto entry = RemoveHeadList(&_head);
			auto item = (CONTAINING_RECORD(entry, item_type, _listEntry));
			v = skl::move(item->_v);
			delete item;
			return true;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	

}


#include "utility.h"
//#include "shared_ptr.hpp"
#include "refholder.hpp"
namespace skl{

	template<typename T, POOL_TYPE poolType = NonPagedPool>
	class list{

		class list_ref_item : public ref_item{
			friend class list;
			friend class ref_holder<list_ref_item>;
		public:
			list_ref_item(const T& v)
				: _v(v), ref_item(1){}
			list_ref_item(T&& v)
				: _v(skl::move(v)), ref_item(1){}
			~list_ref_item(){}

			LIST_ENTRY  _listEntry;
			T _v;
		};

		typedef list<T> _ThisType;
		typedef typename list_ref_item item_type;
		typedef typename ref_holder<item_type> itr_type;

		LIST_ENTRY _head;
		list(const list&);
		list& operator = (const list&);

	public:
		list() {
			//:itr_end(&_head, &_head)
			InitializeListHead(&_head);
		}
		~list(){
			clear();
		}
		void clear(){
			while (!IsListEmpty(&_head))
			{
				auto entry = RemoveHeadList(&_head);
				auto item = CONTAINING_RECORD(entry, item_type, _listEntry);
				if (item){
					item->_listEntry.Flink = NULL;
					item->_listEntry.Blink = NULL;
					item->release();
				}
			}
		}
		bool empty(){
			return TRUE == IsListEmpty(&_head);
		}

		//l and r must be head of a list
		static void swap(PLIST_ENTRY l, PLIST_ENTRY r){
			if (l == r)return;

			auto F = l->Flink;
			auto B = l->Blink;
			auto f = r->Flink;
			auto b = r->Blink;

			auto otherempty = IsListEmpty(r);
			auto thisempty = IsListEmpty(l);
			if (otherempty){
				InitializeListHead(l);
			}
			else {
				l->Flink = f;
				l->Blink = b;
				f->Blink = l;
				b->Flink = l;
			}

			if (thisempty){
				InitializeListHead(r);
			}
			else {
				r->Flink = F;
				r->Blink = B;
				F->Blink = r;
				B->Flink = r;
			}
		}
		void swap(_ThisType& other){
			if (&other == this)return;
			swap(&_head, &other._head);
		}

		void swap(_ThisType&& other){
			if (&other == this)return;
			swap(&_head, &other._head);
		}

		itr_type push_back(const T& v){ //create
			auto item = new(poolType)item_type(v);
			if (!item)return itr_type();
			if (!check_valid(item->_v)){ item->release(); return itr_type(); }
			InsertTailList(&_head, &item->_listEntry);
			return itr_type(item);
		}
		itr_type push_back(T&& v){ //create
			auto item = new(poolType)item_type(skl::move(v));
			if (!item)return itr_type();
			if (!check_valid(item->_v)){ item->release(); return itr_type(); }
			InsertTailList(&_head, &item->_listEntry);
			return itr_type(item);
		}
		itr_type push_front(const T& v){//return back
			auto item = new(poolType)item_type(v);
			if (!item)return itr_type();
			if (!check_valid(item->_v)){ item->release(); return itr_type(); }
			InsertHeadList(&_head, &item->_listEntry);
			return itr_type(item);
		}
		itr_type push_front(T&& v){//return back
			auto item = new(poolType)item_type(skl::move(v));
			if (!item)return itr_type();
			if (!check_valid(item->_v)){ item->release(); return itr_type(); }
			InsertHeadList(&_head, &item->_listEntry);
			return itr_type(item);
		}
		itr_type pop_front(){ //get top should delete outside
			if (IsListEmpty(&_head))
				return itr_type();
			auto entry = RemoveHeadList(&_head);
			auto item = (CONTAINING_RECORD(entry, item_type, _listEntry));
			item->_listEntry.Flink = NULL;
			item->_listEntry.Blink = NULL;
			auto v = itr_type(item);
			item->release();
			return v;
		}


		//l and r must be head of a list
		//append r to l and empty r, or may be shared item is better but should take carefully
		static bool append(PLIST_ENTRY l, PLIST_ENTRY r){
			if (IsListEmpty(r))
				return true;
			if (l == r)
				return false;
			auto F = l->Flink;
			auto b = r->Blink;
			auto f = r->Flink;

			F->Blink = b;
			b->Flink = F;

			l->Flink = f;
			f->Blink = l;

			InitializeListHead(r);

			return true;
		}
		bool append(list&& other){
			//AppendTailList(_head, other._head);
			return append(&_head, &other._head);
		}

		//take care that this copy shares the content of the old list
		static bool copy_wraper(const list& i, list& o){
			for (auto one: i) {
				one.value.retain();
				auto item = one.value.get();
				InsertHeadList(&o._head, &item->_listEntry);
			}
		}

		bool remove(itr_type& itr){
			if (!itr || !itr->_listEntry.Flink || !itr->_listEntry.Blink)
				return false;

			auto item = CONTAINING_RECORD(&itr->_listEntry, item_type, _listEntry);
			//RemoveEntryList
			PLIST_ENTRY PrevEntry;
			PLIST_ENTRY NextEntry;
			NextEntry = itr->_listEntry.Flink;
			PrevEntry = itr->_listEntry.Blink;
			PrevEntry->Flink = NextEntry;
			NextEntry->Blink = PrevEntry;
			itr->_listEntry.Flink = NULL;
			itr->_listEntry.Blink = NULL;

			item->release();
			return true;
		}
		//at. iterator, find
		class iterator{
			friend class list;
		protected:
			PLIST_ENTRY _head;
			PLIST_ENTRY _current;
			ref_holder<item_type> value;
		public:
			iterator(PLIST_ENTRY head, PLIST_ENTRY c) :_head(head), _current(c){
				value = itr_type(_head == _current?nullptr:CONTAINING_RECORD(_current, item_type, _listEntry));
			}
			bool operator==(const iterator& r){
				return _current == r._current;
			}
			bool operator!=(const iterator& r){
				return _current != r._current;
			}
			iterator& operator++(){
				_current = _current->Flink;
				value = itr_type(_head == _current ? nullptr : CONTAINING_RECORD(_current, item_type, _listEntry));
				return *this;
			}

			ref_holder<item_type>& operator *() {
				return value;
			}
			item_type* operator ->() {
				return value.get();
			}
			explicit operator bool() const{
				return value;
			}
		};
		iterator begin(){
			iterator tmp(&_head, _head.Flink);
			//tmp._current = _head.Flink;
			//tmp.value = itr_type(CONTAINING_RECORD(tmp._current, item_type, _listEntry));
			return skl::move(tmp);
		}
		iterator end(){
			//return itr_end;
			return iterator(&_head, &_head);
		}
// 		private:
// 			iterator itr_end;
	};
}


#pragma warning(pop)