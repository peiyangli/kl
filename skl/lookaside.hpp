#pragma once

#include "kernel.hpp"

//////////////////////////////////////////////////////////////////////////

namespace skl{

#ifndef PRINT_TAG
#define PRINT_TAG
#endif

	template<typename T1, typename T2> class same_type { public: enum { ret = false }; };
	template<typename T> class same_type<T, T> { public: enum { ret = true }; };

	template<bool c, typename Then, typename Else> class IF_ { };
	template<typename Then, typename Else>
	class IF_<true, Then, Else> { public: typedef Then reType; };
	template<typename Then, typename Else>
	class IF_<false, Then, Else> { public: typedef Else reType; };

	//////////////////////////////////////////////////////////////////////////
	template<class T, class VaT>struct MatchTypeFinder
	{
		typedef MatchTypeFinder tFinder;
		typedef MatchTypeFinder<T, typename VaT::NextType> tNextFinder;
		typedef typename IF_<same_type<T, typename VaT::Type>::ret, typename VaT::ThisType, typename tNextFinder::TType>::reType TType;
	};

	//////////////////////////////////////////////////////////////////////////
	template<typename... A> struct LookAside{};
	template<typename Head, typename... Tail>
	struct LookAside<Head, Tail...>
	{
		typedef Head Type;
		typedef LookAside<Head, Tail...> ThisType;
		typedef LookAside<Tail...> NextType;
		//static const int size = NextType::size + 1;

		static Head* get(){ 
			KdPrint((PRINT_TAG"LookAside::get(%d)", sizeof(Type)));
			return (Head*)ExAllocateFromNPagedLookasideList(&_Lookaside); 
		}
		static void free(Head* p){ 
			KdPrint((PRINT_TAG"LookAside::free(%d)", sizeof(Type)));
			ExFreeToNPagedLookasideList(&_Lookaside, p); 
		}
		//typedef typename IF_<same_type<T, typename VaT::Type>::ret, typename VaT::ThisType, typename tnextFinder::TType>::reType TType;

		static NPAGED_LOOKASIDE_LIST _Lookaside;
		static void init_once(bool destroy){
			if (destroy){
				ExDeleteNPagedLookasideList(&_Lookaside);
			}
			else{
				ExInitializeNPagedLookasideList(&_Lookaside, NULL, NULL, 0, sizeof(Type), 'gbwf', 0);
			}
			KdPrint((PRINT_TAG"init_once(%d)(%d)", destroy, sizeof(Type)));
			NextType::init_once(destroy);
		}
	};
	template<> struct LookAside<>{ //do a static assert
		//static const int size = 0;
		static void init_once(bool destroy){ destroy; }
	};

	template<typename Head, typename... Tail>
	NPAGED_LOOKASIDE_LIST LookAside<Head, Tail...>::_Lookaside;

	template<class T>struct MatchTypeFinder<T, LookAside<>>{
		typedef void TType;
	};

	//use a do()while(false)
#define LOOKASIDE_ALLOC_WITHT(type, la_t) skl::MatchTypeFinder<type, la_t>::TType::get()
#define LOOKASIDE_FREE_WITHT(ptr, la_t) skl::MatchTypeFinder<skl::remove_pointer<decltype(ptr)>::type, la_t>::TType::free(ptr)

}

//////////////////////////////////////////////////////////////////////////
//e.g.
// class Test;
// typedef skl::LookAside<int, bool, short, Test> LookAside_t;
// #define LOOKASIDE_ALLOC(type) skl::MatchTypeFinder<type, LookAside_t>::TType::get()
// #define LOOKASIDE_FREE(ptr) skl::MatchTypeFinder<skl::remove_pointer<decltype(ptr)>::type, LookAside_t>::TType::free(ptr)
// //definition of class Test, before LookAside_t::init_once
// void test(){
// 	LookAside_t::init_once(false); //driver entry
// 
// 	auto ptr = LOOKASIDE_ALLOC(Test);
// 	LOOKASIDE_FREE(ptr);
// 
// 	LookAside_t::init_once(true); //unload driver
// }
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

namespace skl{

	//////////////////////////////////////////////////////////////////////////
	//none static
	template<typename... A> struct LookAsidePool{};
	template<typename Head, typename... Tail>
	struct LookAsidePool<Head, Tail...> : public LookAsidePool<Tail...>
	{
		typedef Head Type;
		typedef LookAsidePool<Head, Tail...> ThisType;
		typedef LookAsidePool<Tail...> NextType;
		//static const int size = NextType::size + 1;

	protected:
		inline void* get(){ 
			KdPrint((PRINT_TAG"LookAsidePool::get(%d)(%p)", sizeof(Type), &_Lookaside));
			return ExAllocateFromNPagedLookasideList(&_Lookaside); 
		}
		inline void free(void* p){ 
			KdPrint((PRINT_TAG"LookAsidePool::free(%d)(%p)", sizeof(Type), &_Lookaside));
			ExFreeToNPagedLookasideList(&_Lookaside, p); 
		}

		NPAGED_LOOKASIDE_LIST _Lookaside;
	public:
		LookAsidePool(){
			KdPrint((PRINT_TAG"LookAsidePool(%d)", sizeof(Type)));
			ExInitializeNPagedLookasideList(&_Lookaside, NULL, NULL, 0, sizeof(Type), 'gbwf', 0);
		}
		~LookAsidePool(){
			KdPrint((PRINT_TAG"~LookAsidePool(%d)", sizeof(Type)));
			ExDeleteNPagedLookasideList(&_Lookaside);
		}
		template<typename T>
		inline T* New(){
			return (T*)MatchTypeFinder<T, ThisType>::TType::get();
		}
		template<typename T>
		inline void Delete(T* p){
			MatchTypeFinder<T, ThisType>::TType::free(p);
		}
	};
	template<> struct LookAsidePool<>{ //do a static assert
	};

	template<class T>struct MatchTypeFinder<T, LookAsidePool<>>{
		typedef void TType;
	};

}

//////////////////////////////////////////////////////////////////////////
//e.g.
// skl::LookAsidePool<int, bool, Test> poolLookAside;
// auto pint = poolLookAside.New<int>();
// poolLookAside.Delete(pint);
//////////////////////////////////////////////////////////////////////////