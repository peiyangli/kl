/*////////////////////////////////////////////////////////
(c)22016
filename: lookaside.hpp
author: pei
version: 0.0.1
date:
description: easy to make lookaside by size or by type
function/class list:
history:
////////////////////////////////////////////////////////*/

#pragma once

#include "kernel.h"
#include "type_traits.hpp"
//////////////////////////////////////////////////////////////////////////
//mostly, you should overwrite operator new/delete for c++ class
//
//////////////////////////////////////////////////////////////////////////

namespace skl{
	template<bool c, typename Then, typename Else> class IF_ { };
	template<typename Then, typename Else>
	class IF_<true, Then, Else> { public: typedef Then reType; };
	template<typename Then, typename Else>
	class IF_<false, Then, Else> { public: typedef Else reType; };



	template<typename T1, typename T2> class same_type { public: enum { ret = false }; };
	template<typename T> class same_type<T, T> { public: enum { ret = true }; };
	//////////////////////////////////////////////////////////////////////////
	template<class T, class VaT>struct MatchTypeFinder
	{
		typedef typename MatchTypeFinder tFinder;
		typedef typename MatchTypeFinder<T, typename VaT::NextType> tNextFinder;
		typedef typename IF_<same_type<T, typename VaT::Type>::ret, typename VaT::ThisType, typename tNextFinder::TType>::reType TType;
	};

	template<int T1Size, int T2Size> class same_size { public: enum { ret = false }; };
	template<int TSize> class same_size<TSize, TSize> { public: enum { ret = true }; };
	//////////////////////////////////////////////////////////////////////////
	template<int T, class VaT>struct MatchSizeTypeFinder
	{
		typedef typename MatchSizeTypeFinder tFinder;
		typedef typename MatchSizeTypeFinder<T, typename VaT::NextType> tNextFinder;
		typedef typename IF_<same_size<T, VaT::TypeSize>::ret, typename VaT::ThisType, typename tNextFinder::TType>::reType TType;
	};
	//////////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////

namespace skl{
	//allocator
	template<POOL_TYPE, int TypeSize>
	class LookasideAllocator{
	public:
	};
	template<int TypeSize>
	class LookasideAllocator<NonPagedPool, TypeSize>{
		LookasideAllocator(const LookasideAllocator&) = delete;
		LookasideAllocator& operator=(const LookasideAllocator&) = delete;
		LookasideAllocator(LookasideAllocator&& r) = delete;
		LookasideAllocator& operator=(LookasideAllocator&& r) = delete;


		NPAGED_LOOKASIDE_LIST _Lookaside;
	public:
		LookasideAllocator(){
			ExInitializeNPagedLookasideList(&_Lookaside, NULL, NULL, 0, TypeSize, 'gbwf', 0);
		}
		~LookasideAllocator(){
			ExDeleteNPagedLookasideList(&_Lookaside);
		}
		inline void* allocate(){
			return ExAllocateFromNPagedLookasideList(&_Lookaside);
		}
		inline void free(void* p){
			ExFreeToNPagedLookasideList(&_Lookaside, p);
		}
	};
	template<int TypeSize>
	class LookasideAllocator<PagedPool, TypeSize>{
		LookasideAllocator(const LookasideAllocator&) = delete;
		LookasideAllocator& operator=(const LookasideAllocator&) = delete;
		LookasideAllocator(LookasideAllocator&& r) = delete;
		LookasideAllocator& operator=(LookasideAllocator&& r) = delete;


		PAGED_LOOKASIDE_LIST _Lookaside;
	public:
		LookasideAllocator(){
			ExInitializePagedLookasideList(&_Lookaside, NULL, NULL, 0, TypeSize, 'gbwf', 0);
		}
		~LookasideAllocator(){
			ExDeletePagedLookasideList(&_Lookaside);
		}
		inline void* allocate(){
			return ExAllocateFromPagedLookasideList(&_Lookaside);
		}
		inline void free(void* p){
			ExFreeToPagedLookasideList(&_Lookaside, p);
		}
	};
}

//////////////////////////////////////////////////////////////////////////
namespace skl{

	//////////////////////////////////////////////////////////////////////////
	//none static
	template<POOL_TYPE poolType, typename... A> struct LookAsidePool{};
	template<POOL_TYPE poolType, typename Head, typename... Tail>
	struct LookAsidePool<poolType, Head, Tail...> : public LookAsidePool<poolType, Tail...>
	{
		typedef Head Type;
		typedef LookAsidePool<poolType, Head, Tail...> ThisType;
		typedef LookAsidePool<poolType, Tail...> NextType;
		//static const int size = sizeof(Head);

	protected:
		inline void* get(){ 
			return _Lookaside.allocate();
			//return nullptr;
		}
		inline void free(void* p){ 
			_Lookaside.free(p);
		}
		LookasideAllocator<poolType, sizeof(Head)> _Lookaside;
	public:
		template<typename T>
		inline T* Malloc(){
			return (T*)MatchTypeFinder<T, ThisType>::TType::get();
		}
		template<typename T>
		inline void Free(T* p){
			MatchTypeFinder<T, ThisType>::TType::free(p);
		}

		template<typename T,
		class... _Types>
			inline T* New(_Types&&... _Args){
			typedef typename T TheT;
			auto ptr = (T*)MatchTypeFinder<T, ThisType>::TType::get();
			if (ptr){
				ptr = new(ptr)TheT(forward<_Types>(_Args)...);
			}
			return ptr;
		}
		template<typename T>
		inline void Delete(T* p){
			if (p){
				typedef typename T TheT;
				p->~TheT();
				MatchTypeFinder<T, ThisType>::TType::free(p);
			}
		}
	};
	template<POOL_TYPE poolType> struct LookAsidePool<poolType>{ //do a static assert
	};

	template<class T>struct MatchTypeFinder<T, LookAsidePool<NonPagedPool>>{
		typedef void TType;
	};
	template<class T>struct MatchTypeFinder<T, LookAsidePool<PagedPool>>{
		typedef void TType;
	};
}

//////////////////////////////////////////////////////////////////////////
//e.g.
// skl::LookAsidePool<NonPagedPool, int, bool, Test> poolLookAside;
// auto pint = poolLookAside.New<int>();
// poolLookAside.Delete(pint);
//////////////////////////////////////////////////////////////////////////


namespace skl{
	template<POOL_TYPE poolType, int... A> struct LookAsideSizePool{};
	template<POOL_TYPE poolType, int Head, int... Tail>
	struct LookAsideSizePool<poolType, Head, Tail...> : public LookAsideSizePool<poolType, Tail...>
	{
		enum{ TypeSize = Head };
		typedef LookAsideSizePool<poolType, Head, Tail...> ThisType;
		typedef LookAsideSizePool<poolType, Tail...> NextType;
		//static const int size = NextType::size + 1;

		LookasideAllocator<poolType, TypeSize> _Lookaside;
	protected:
		inline void* get(){
			return _Lookaside.allocate();
		}
		inline void free(void* p){
			_Lookaside.free(p);
		}
	public:
		template<typename T>
		inline T* Malloc(){
			return (T*)MatchSizeTypeFinder<sizeof(T), ThisType>::TType::get();
		}

		template<typename T>
		inline void Free(T* p){
			return MatchSizeTypeFinder<sizeof(T), ThisType>::TType::free(p);
		}

		template<typename T,
		class... _Types>
			inline T* New(_Types&&... _Args){
			typedef typename T TheT;
			auto ptr = (T*)MatchSizeTypeFinder<sizeof(T), ThisType>::TType::get();
			if (ptr){
				ptr = new(ptr)TheT(forward<_Types>(_Args)...);
			}
			return ptr;
		}

		template<typename T>
		inline void Delete(T* p){
			if (p){
				typedef typename T TheT;
				p->~TheT();
				MatchSizeTypeFinder<sizeof(T), ThisType>::TType::free(p);
			}
		}
	};
	template<POOL_TYPE poolType> struct LookAsideSizePool<poolType>{ //do a static assert
	};

	template<int T>struct MatchSizeTypeFinder<T, LookAsideSizePool<NonPagedPool>>{
		typedef void TType;
	};
	template<int T>struct MatchSizeTypeFinder<T, LookAsideSizePool<PagedPool>>{
		typedef void TType;
	};
}