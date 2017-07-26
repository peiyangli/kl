#pragma once

#include "kernel.h"
#include "initializer_list.hpp"
namespace skl{

	template<class _Ty, POOL_TYPE poolType = NonPagedPool>
	class vector{
		typedef vector<_Ty, poolType> _MyType;
		typedef _Ty _DataType;
		enum{
			SIZE_OF_TYPE = sizeof(_Ty),
			Def_Reserve = 10
		};
		_Ty* _data;//need placement new
		unsigned _capacity;
		unsigned _size;


		template<class _Ty0, POOL_TYPE poolType0>
		friend class vector;

	public:
		vector(unsigned defSize = Def_Reserve) : _data((_Ty*)ExAllocatePoolWithTag(poolType, defSize*SIZE_OF_TYPE, DEF_ALLOCAL_TAG)), _capacity(0), _size(0){
			if (_data){
				_capacity = defSize;
			}
		}
		~vector(){
			clear();
			if (_data)
				ExFreePool(_data);
// 			_data = 0;
// 			_capacity = 0;

		}

		vector(std::initializer_list<_Ty> _Ilist) :_data(0), _capacity(0), _size(0){
			insert(0, _Ilist.begin(), _Ilist.end());
		}

		//if no copy-constructor, the compiler would not choose the template below?
		vector(const vector& r) :_capacity(0), _size(0), _data((_Ty*)ExAllocatePoolWithTag(poolType, r._capacity*SIZE_OF_TYPE, DEF_ALLOCAL_TAG)){
			//make all copy, and all construct
			if (_data){
				_capacity = r._capacity;
				RtlCopyMemory(_data, r._data, r._size*SIZE_OF_TYPE);
				//construct
				while (_size < r._size) {
					new(_data + _size)_Ty(r._data[_size]); //placement new
					_size++;
				}
			}
		}
		template<POOL_TYPE opoolType>
		vector(const vector<_Ty, opoolType>& r) :_capacity(0), _size(0),_data((_Ty*)ExAllocatePoolWithTag(poolType, r._capacity*SIZE_OF_TYPE, DEF_ALLOCAL_TAG)){
			//make all copy, and all construct
			if (_data){
				_capacity = r._capacity;
				RtlCopyMemory(_data, r._data, r._size*SIZE_OF_TYPE);
				//construct
				while (_size < r._size) {
					new(_data + _size)_Ty(r._data[_size]); //placement new
					_size++;
				}
			}
		}
		vector(vector&& r):_data(r._data), _capacity(r._capacity), _size(r._size){
			//
			r._data = 0;
			r._capacity = 0;
			r._size = 0;
		}
		template<POOL_TYPE opoolType>
		vector(vector<_Ty, opoolType>&& r) :_capacity(0), _size(0), _data((_Ty*)ExAllocatePoolWithTag(poolType, r._capacity*SIZE_OF_TYPE, DEF_ALLOCAL_TAG)){
			//make all copy, and all construct
			if (_data){
				_capacity = r._capacity;
				RtlCopyMemory(_data, r._data, r._size*SIZE_OF_TYPE);
				_size = r._size;

				r._size = 0;
 				r._data = 0;
 				r._capacity = 0;
			}
		}

		vector& swap(vector& other){
			if (this == &other)return *this;

			auto tmp_data = other._data;
			auto tmp_allocCount = other._capacity;
			auto tmp_size = other._size;

			other._data = _data;
			other._capacity = _capacity;
			other._size = _size;

			_data = tmp_data;
			_capacity = tmp_allocCount;
			_size = tmp_size;

			return *this;
		}

		vector& operator=(const vector& r){
			vector(r).swap(*this);
			return *this;
		}
		vector& operator=(vector&& r){
			return r.swap(*this);
			//
// 			if (this == &r)return *this;
// 			_data = r._data; _capacity = r._capacity; _size = r._size;
// 			r._data = 0;
// 			r._capacity = 0;
// 			r._size = 0;
// 			return *this;
		}

		bool push_back(const _Ty& value){//copy constructor
			if (_capacity > _size){
				new(_data + _size)_Ty(value);//placement new the element at _size
				_size++;
			}
			else if(_capacity == _size){
				auto new_allocCount = _capacity > 0 ? (_capacity > 1024 ? _capacity + 1024 : _capacity * 2) : Def_Reserve;
				auto new_data = (_Ty*)ExAllocatePoolWithTag(poolType, new_allocCount*SIZE_OF_TYPE, DEF_ALLOCAL_TAG);
				if (!new_data)
					return false;
				RtlCopyMemory(new_data, _data, _size*SIZE_OF_TYPE);
				if (_data)
					ExFreePool(_data);
				_data = new_data;
				_capacity = new_allocCount;

				new(_data+_size)_Ty(value);//placement new the element at _size
				_size++;
				return true;
			}
			return false;
		}
		bool push_back(_Ty&& value){
			if (_capacity > _size){
				new(_data+_size)_Ty(skl::move(value));//placement new the element at _size
				_size++;
			}
			else if (_capacity == _size){
				auto new_allocCount = _capacity > 0 ? (_capacity > 1024 ? _capacity + 1024 : _capacity * 2) : Def_Reserve;
				auto new_data = (_Ty*)ExAllocatePoolWithTag(poolType, new_allocCount*SIZE_OF_TYPE, DEF_ALLOCAL_TAG);
				if (!new_data)
					return false;
				RtlCopyMemory(new_data, _data, _size*SIZE_OF_TYPE);
				if (_data)
					ExFreePool(_data);
				_data = new_data;
				_capacity = new_allocCount;

				new(_data + _size)_Ty(skl::move(value));//placement new the element at _size
				_size++;
				return true;
			}
			return false;
		}

		template< class... Args >
		void emplace_back(Args&&... args){
			if (_capacity > _size){
				new(_data + _size)_Ty(skl::forward(args));//placement new the element at _size
				_size++;
			}
			else if (_capacity == _size){
				auto new_allocCount = _capacity > 0 ? (_capacity > 1024 ? _capacity + 1024 : _capacity * 2) : Def_Reserve;
				auto new_data = (_Ty*)ExAllocatePoolWithTag(poolType, new_allocCount*SIZE_OF_TYPE, DEF_ALLOCAL_TAG);
				if (!new_data)
					return false;
				RtlCopyMemory(new_data, _data, _size*SIZE_OF_TYPE);
				if (_data)
					ExFreePool(_data);
				_data = new_data;
				_capacity = new_allocCount;

				new(_data + _size)_Ty(skl::forward(args));//placement new the element at _size
				_size++;
				return true;
			}
			return false;
		}
		bool pop_back(){
			if (_size > 0){
				(_data + (--_size))->~_Ty();
			}
			return false;
		}
		
		bool empty()const{
			return _size == 0;
		}
		unsigned size()const{
			return _size;
		}
		unsigned capacity()const{
			return _capacity;
		}
		_Ty* data(){
			return _data;
		}
		const _Ty* data() const{
			return _data;
		}

		bool at(unsigned pos, _Ty& o){
			if (_size < pos && _size > 0){
				o = _data[pos];
				return true;
			}
			return false;
		}
		_Ty& at(unsigned pos){//throw?
			return _data[pos];//not check pos
		}
		const _Ty& at(unsigned pos)const{//throw?
			return _data[pos];//not check pos
		}
		void clear(){
			while (_size > 0){
				(_data + (--_size))->~_Ty();
			}
		}
		//insert
		bool insert(unsigned pos, const _Ty& value){
			if (pos > _size)return false;//this is an error
			if (pos == _size){
				return push_back(value);
			}
			if (_size < _capacity){
				//just move item and create a new one
				RtlMoveMemory(_data + pos + 1, _data + pos, SIZE_OF_TYPE*(_size-pos));

				new(_data + pos)_Ty(value);//placement new the element at _size
				_size++;
			}
			else if (_size == _capacity){
				//relocate
				auto new_allocCount = _capacity > 0 ? (_capacity > 1024 ? _capacity + 1024 : _capacity * 2) : Def_Reserve;
				auto new_data = (_Ty*)ExAllocatePoolWithTag(poolType, new_allocCount*SIZE_OF_TYPE, DEF_ALLOCAL_TAG);
				if (!new_data)
					return false;
				RtlCopyMemory(new_data, _data, pos*SIZE_OF_TYPE);
				RtlCopyMemory(new_data + pos + 1, _data + pos, (_size-pos)*SIZE_OF_TYPE);
				if (_data)
					ExFreePool(_data);

				_data = new_data;
				_capacity = new_allocCount;

				new(_data + pos)_Ty(value);//placement new the element at _size
				_size++;
				return true;
			}
			//move memory after pos and allocate a new one
			return false;//not implement
		}
		bool insert(unsigned pos, _Ty&& value){
			if (pos > _size)return false;//this is an error
			if (pos == _size){
				return push_back(skl::move(value));
			}
			if (_size < _capacity){
				//just move item and create a new one
				RtlMoveMemory(_data + pos + 1, _data + pos, SIZE_OF_TYPE*(_size - pos));

				new(_data + pos)_Ty(skl::move(value));//placement new the element at _size
				_size++;
			}
			else if (_size == _capacity){
				//relocate
				auto new_allocCount = _capacity > 0 ? (_capacity > 1024 ? _capacity + 1024 : _capacity * 2) : Def_Reserve;
				auto new_data = (_Ty*)ExAllocatePoolWithTag(poolType, new_allocCount*SIZE_OF_TYPE, DEF_ALLOCAL_TAG);
				if (!new_data)
					return false;
				RtlCopyMemory(new_data, _data, pos*SIZE_OF_TYPE);
				RtlCopyMemory(new_data + pos + 1, _data + pos, (_size - pos)*SIZE_OF_TYPE);
				if (_data)
					ExFreePool(_data);

				_data = new_data;
				_capacity = new_allocCount;

				new(_data + pos)_Ty(skl::move(value));//placement new the element at _size
				_size++;
				return true;
			}
			//move memory after pos and allocate a new one
			return false;//not implement
		}

		template< class... Args >
		void emplace_insert(unsigned pos, Args&&... args){
			if (pos > _size)return false;//this is an error
			if (pos == _size){
				return push_back(skl::forward(value));
			}
			if (_size < _capacity){
				//just move item and create a new one
				RtlMoveMemory(_data + pos + 1, _data + pos, SIZE_OF_TYPE*(_size - pos));

				new(_data + pos)_Ty(skl::forward(value));//placement new the element at _size
				_size++;
			}
			else if (_size == _capacity){
				//relocate
				auto new_allocCount = _capacity > 0 ? (_capacity > 1024 ? _capacity + 1024 : _capacity * 2) : Def_Reserve;
				auto new_data = (_Ty*)ExAllocatePoolWithTag(poolType, new_allocCount*SIZE_OF_TYPE, DEF_ALLOCAL_TAG);
				if (!new_data)
					return false;
				RtlCopyMemory(new_data, _data, pos*SIZE_OF_TYPE);
				RtlCopyMemory(new_data + pos + 1, _data + pos, (_size - pos)*SIZE_OF_TYPE);
				if (_data)
					ExFreePool(_data);

				_data = new_data;
				_capacity = new_allocCount;

				new(_data + pos)_Ty(skl::forward(value));//placement new the element at _size
				_size++;
				return true;
			}
			//move memory after pos and allocate a new one
			return false;//not implement
		}

		bool reserve(unsigned ns){
			if (_capacity < ns){
				auto new_data = (_Ty*)ExAllocatePoolWithTag(poolType, ns*SIZE_OF_TYPE, DEF_ALLOCAL_TAG);
				if (!new_data)
					return false;
				RtlCopyMemory(new_data, _data, _size*SIZE_OF_TYPE);
				if (_data)
					ExFreePool(_data);
				_data = new_data;
				_capacity = ns;
				return true;
			}
			return true;
		}

// 
// 		bool reserve(unsigned ns, unsigned skipWhr, unsigned skipCnt){
// 			if (_capacity < ns){
// 				auto new_data = (_Ty*)ExAllocatePoolWithTag(poolType, ns*SIZE_OF_TYPE, DEF_ALLOCAL_TAG);
// 				if (!new_data)
// 					return false;
// 				RtlCopyMemory(new_data, _data, _size*SIZE_OF_TYPE);
// 				if (_data)
// 					ExFreePool(_data);
// 				_data = new_data;
// 				_capacity = ns;
// 				return true;
// 			}
// 			return true;
// 		}

		//insert an array
		template<class _Iter>
		bool insert(unsigned pos, _Iter _First, _Iter _Last)
		{
			if (!(_First < _Last)){
				return false;
			}
			unsigned range = _Last - _First;
			auto ns = _size + range;
			auto new_allocCount = _capacity > 0 ? (_capacity > 1024 ? _capacity + 1024 : _capacity * 2) : Def_Reserve;
			ns = ns > new_allocCount ? ns : new_allocCount;
			if (_capacity < ns){
				auto new_data = (_Ty*)ExAllocatePoolWithTag(poolType, ns*SIZE_OF_TYPE, DEF_ALLOCAL_TAG);
				if (!new_data)
					return false;
				//skip n
				//RtlMoveMemory
				RtlCopyMemory(new_data, _data, pos*SIZE_OF_TYPE);
				RtlCopyMemory(new_data + pos + range, _data + pos, (_size - pos)*SIZE_OF_TYPE);
				if (_data)
					ExFreePool(_data);
				_data = new_data;
				_capacity = ns;
			}
			else{
				//skip n
				RtlMoveMemory(_data + pos + range, _data + pos, (_size - pos)*SIZE_OF_TYPE);
			}
			//from pos to pos+range, construct
			for (unsigned index = pos; _First != _Last; ++_First, ++index)
				new(_data + index)_Ty(*_First);

			_size = _size + range;
			return true;
		}
		bool insert(unsigned pos, std::initializer_list<_Ty> _Ilist){
			return insert(pos, _Ilist.begin(), _Ilist.end());
		}

		//append
// 		bool append(const vector& r){
// 			//resize memory
// 			if (_capacity - _size < r._size){ 
// 				if (!reserve(_size + r._size))//_size if ok, but a 
// 					return false;
// 			}
// 			//make copy
// 			RtlCopyMemory(_data + _size, r._data, r._size*SIZE_OF_TYPE);
// 
// 			for (int i = 0; i < r._size; ++i, ++_size) {
// 				new(_data + _size + i)_Ty(r._data[i]);
// 			}
// 			return true;
// 		}
		template<POOL_TYPE opoolType>
		bool append(const vector<_Ty, opoolType>& r){
			if (!reserve(_size + r._size))//_size if ok, but a 
				return false;
			//make copy
			RtlCopyMemory(_data + _size, r._data, r._size*SIZE_OF_TYPE);

			for (unsigned i = 0; i < r._size; ++i) {
				new(_data + (_size++))_Ty(r._data[i]);
			}
			return true;
		}
		bool append(vector&& other){
			//resize memory
			if (!reserve(_size + r._size))//_size if ok, but a 
				return false;
			//make copy
			RtlCopyMemory(_data + _size, r._data, r._size*SIZE_OF_TYPE);
			_size = _size + r._size;
			r._size = 0;
			return true;
		}

		//erase
		bool erase(unsigned pos){
			if (pos < _size){
				//first erase
				(_data + pos)->~_Ty();
				//move memory and --_size
				RtlMoveMemory(_data + pos, _data + pos + 1, (--_size - pos)*SIZE_OF_TYPE);
				return true;
			}
			return false;
		}
		//iterator
		class iterator{
		protected:
			unsigned _pos;
			_MyType& _owner;
		public:
			iterator(_MyType& o, unsigned pos = 0) :_owner(o), _pos(pos){
			}

			iterator& swap(iterator& r){
				if (this != &r){
					{
						auto tmp = _pos;
						_pos = r._pos;
						r._pos = tmp;
					}
					{
						//this is an error
						auto tmp = _owner;
						_owner = r._owner;
						r._owner = tmp;
					}
				}
				return *this;
			}

			iterator(const iterator& r): _pos(r._pos), _owner(r._owner){
			}
			iterator& operator=(const iterator& r){
				iterator(r).swap(*this);
				return *this;
			}
			iterator& operator=(iterator&& r){
				r.swap(*this);
				return *this;
			}

			bool operator==(const iterator& r){
				return _pos == r._pos;
			}
			bool operator<(const iterator& r){
				return _pos < r._pos;
			}
			unsigned operator-(const iterator& r){
				return _pos - r._pos;
			}
			bool operator!=(const iterator& r){
				return _pos != r._pos;
			}
			iterator& operator++(){
				_pos++;
				return *this;
			}

			_Ty& operator *() {
				return _owner.at(_pos);
			}
			_Ty* operator ->() {
				auto& item = _owner.at(_pos);
				return &item;
			}
		};

		iterator begin(){
			iterator tmp(*this);
			return skl::move(tmp);
		}
		iterator end(){
			return iterator(*this, _size);
		}
	};

}//namespace skl