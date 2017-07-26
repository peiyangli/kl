
/*////////////////////////////////////////////////////////
(c)22016
filename: map.hpp
author: pei
version: 0.0.1
date:
description: template map
function/class list:
history:
////////////////////////////////////////////////////////*/

#pragma once


#include"utility.h"
#include "refholder.hpp"

namespace skl{

	template<class _KeyT, class _ValueT, POOL_TYPE poolType = NonPagedPool>
	class map{
		//typedef typename skl::pair<_KeyT, _ValueT> item_type;
		typedef typename map<_KeyT, _ValueT> _MyT;
		
		class map_ref_item : public ref_item{
			friend class map;
			friend class ref_holder<map_ref_item>;
		public:
			map_ref_item(){}
			map_ref_item(const _KeyT& k, const _ValueT& v) : first(k), second(v), ref_item(1){
			}
			_KeyT first;
			_ValueT second;
		};

// 		template<>
// 		inline bool check_valid<map_ref_item>(const map_ref_item& v){
// 			return check_valid(v.first) && check_valid(v.second);
// 		}

		class item_type{ //no construct/deconstruct will be called
		public:
			map_ref_item* _item;
		};

		struct item_type_avl
		{
			_RTL_BALANCED_LINKS reserved1;
			item_type _reserved2;
		};
		typedef item_type_avl *Pitem_type_avl;

		static RTL_GENERIC_COMPARE_RESULTS
			NTAPI
			Compare(
			__in struct _RTL_AVL_TABLE *Table,
			__in PVOID FirstStruct,
			__in PVOID SecondStruct // 已存在的项
			)
		{
			UNREFERENCED_PARAMETER(Table);
			auto f = (item_type*)FirstStruct;
			auto s = (item_type*)SecondStruct;
			//GenericEqual GenericGreaterThan GenericLessThan
			return (f->_item->first < s->_item->first) ? GenericLessThan : ((s->_item->first < f->_item->first) ? GenericGreaterThan : GenericEqual);
		}
		static PVOID
			NTAPI
			Allocate(
			__in struct _RTL_AVL_TABLE *Table,
			__in CLONG ByteSize
			)
		{
			UNREFERENCED_PARAMETER(Table);
			//ByteSize == sizeof(_RTL_BALANCED_LINKS) + sizeof(runtime::Uint64Item);
			//ASSERT(ByteSize == sizeof(item_type));
			ASSERT(ByteSize == sizeof(item_type_avl));
			return runtime::gInstanceGlobleRuntime->poolLookAside.Malloc<item_type_avl>();

// 			PVOID ptr = ExAllocatePoolWithTag(poolType, ByteSize, 'avla');
// 			return ptr;
		}
		static void
			NTAPI
			Free(
			__in struct _RTL_AVL_TABLE *Table,
			__in __drv_freesMem(Mem) __post_invalid PVOID Buffer
			)
		{
			UNREFERENCED_PARAMETER(Table);
			runtime::gInstanceGlobleRuntime->poolLookAside.Free((item_type_avl*)Buffer);
			//ExFreePool(Buffer);
		}

		//swap is not ok for the _root
		RTL_AVL_TABLE _root;

	public:
		map() { //:itr_end(*this, (PVOID)-1)
			RtlInitializeGenericTableAvl(&_root, &map::Compare, &map::Allocate, &map::Free, NULL);
		}
		~map() {
			clear();
		}

		void clear() {
			for (auto p = (item_type*)RtlEnumerateGenericTableAvl(&_root, TRUE);
				p != NULL;
				p = (item_type*)RtlEnumerateGenericTableAvl(&_root, FALSE))
			{
				RtlDeleteElementGenericTableAvl(&_root, p); // 会调用Compare！
				if (p->_item)
					p->_item->release();
			}
		}
		ULONG count() {
			return RtlNumberGenericTableElementsAvl(&_root);
		}
		BOOLEAN empty() {
			return RtlIsGenericTableEmptyAvl(&_root);
		}

		skl::pair<ref_holder<map_ref_item>, bool> find(const _KeyT& val) {
			map_ref_item temp_item;
			temp_item.first = val;
			if (!check_valid(temp_item.first))
				return skl::pair<ref_holder<map_ref_item>, bool>(ref_holder<map_ref_item>(nullptr), false);
			item_type temp;
			temp._item = &temp_item;
			auto result = (item_type*)RtlLookupElementGenericTableAvl(&_root, (PVOID)(&temp));
			return skl::pair<ref_holder<map_ref_item>, bool>(ref_holder<map_ref_item>(result ? result->_item : nullptr), true);
		}

		skl::pair<ref_holder<map_ref_item>, bool> remove(const _KeyT& value) {
			map_ref_item temp_item;
			temp_item.first = value;
			if (!check_valid(temp_item.first))
				return skl::pair<ref_holder<map_ref_item>, bool>(ref_holder<map_ref_item>(nullptr), false);
			item_type temp;
			temp._item = &temp_item;
			auto result = (item_type*)RtlLookupElementGenericTableAvl(&_root, (PVOID)(&temp));
			BOOLEAN ok = FALSE;
			ref_holder<map_ref_item> holder(result ? result->_item : nullptr);
			if (result){
				auto val = result->_item;
				ok = RtlDeleteElementGenericTableAvl(&_root, (PVOID)(&temp));
				if (ok && val)
					val->release();
			}
			return skl::pair<ref_holder<map_ref_item>, bool>(skl::move(holder), ok == TRUE);
		}
		//T& operator[]( const Key& key );
		skl::pair<ref_holder<map_ref_item>, bool> insert(const _KeyT& key, const _ValueT& value) {
			map_ref_item temp_item;
			temp_item.first = key;
			if(!check_valid(temp_item.first))
				return skl::pair<ref_holder<map_ref_item>, bool>(ref_holder<map_ref_item>(nullptr), false);
			item_type temp;
			temp._item = &temp_item;
			BOOLEAN newElement = FALSE;
			item_type* result = (item_type*)RtlInsertElementGenericTableAvl(&_root,
				&temp,
				sizeof(item_type),
				&newElement);
			if (result && newElement) {
				result->_item = new(poolType)map_ref_item(key, value); //replace the stack memory
				if (!result->_item){ //no resource of memory
					//do remove and report error. This is The New Item, it's on the stack, so we do not need to release its refcount
					RtlDeleteElementGenericTableAvl(&_root, (PVOID)(&temp));
					result = nullptr;
				}

				if (!check_valid(result->_item->first)){
					result->_item->release();
					result = nullptr;
					RtlDeleteElementGenericTableAvl(&_root, (PVOID)(&temp));
				}
// 				else{
// 					result->_item->retain();//give a ref-count
// 					result->_item->second = value;//give the value
// 				}
			}
			return skl::pair<ref_holder<map_ref_item>, bool>(ref_holder<map_ref_item>(result ? result->_item : nullptr), newElement == TRUE);
		}

		//Should be set to NULL if the enumeration is to start at the first element in the table.

		class iterator {
			PVOID handle;
			//++
			_MyT& _root;
		public:
			ref_holder<map_ref_item> value;

			iterator(const iterator& r) : _root(r._root), value(r.value), handle(r.handle) {
			}
			bool operator==(const iterator& r){
				return handle == r.handle;
			}
			bool operator!=(const iterator& r){
				return handle != r.handle;
			}
			iterator& operator++(){
				auto rt = _root.next(&handle);
				value = ref_holder<map_ref_item>(rt ? rt->_item : nullptr);
				if (!rt){
					handle = (PVOID)-1;
				}
				return *this;
			}
			iterator(_MyT& me, PVOID v = NULL) :_root(me), handle(v){
			}
			iterator& swap(iterator& r){
				if (this != &r){
					{
						auto tmp = handle;
						handle = r.handle;
						r.handle = skl::move(tmp);
					}
					{
						auto tmp = value;
						value = r.value;
						r.value = skl::move(tmp);
					}
				}
				return *this;
			}

			iterator& operator=(const iterator& r){
				iterator(r).swap(*this);
				return *this;
			}
			iterator& operator=(iterator&& r){
				r.swap(*this);
				return *this;
			}

			ref_holder<map_ref_item>& operator *() {
				return value;
			}
			map_ref_item* operator ->() {
				return value.get();
			}
			explicit operator bool() const{
				return value;
			}
// 			iterator& operator *() {
// 				return *this;
// 			}
		};
		iterator begin(){
			iterator itr(*this, NULL);
			++itr;
			return skl::move(itr);
		}
		iterator end(){
			//return itr_end;
			return iterator(*this, (PVOID)-1);
		}
private:
		inline item_type* next(PVOID* handle) {
			return (item_type*)RtlEnumerateGenericTableWithoutSplayingAvl(&_root, handle);
		}

		//iterator itr_end;
	};
}//namespace skl