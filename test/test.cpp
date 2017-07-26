// test.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
//////////////////////////////////////////////////////////////////////////
template<typename T1, typename T2> class same_type { public: enum { ret = false }; };
template<typename T> class same_type<T, T> { public: enum { ret = true }; };

template<bool c, typename Then, typename Else> class IF_ { };
template<typename Then, typename Else>
class IF_<true, Then, Else> { public: typedef Then reType; };
template<typename Then, typename Else>
class IF_<false, Then, Else> { public: typedef Else reType; };

//////////////////////////////////////////////////////////////////////////
template<typename... A> struct ValAlloc{};
template<typename Head, typename... Tail>
struct ValAlloc<Head, Tail...> : public ValAlloc<Tail...>
{
	typedef Head Type;
	typedef ValAlloc<Head, Tail...> ThisType;
	typedef ValAlloc<Tail...> NextType;
	static const int size = NextType::size + 1;

	static int get(){ return ki; }
	//typedef typename IF_<same_type<T, typename VaT::Type>::ret, typename VaT::ThisType, typename tnextFinder::TType>::reType TType;

	static int ki;
	static void init(int c = 0){
 		ki = size + c;
 		NextType::init(c);
	}

	static void print(){
		std::cout <<"size: " << size << "\tki: " << ki << std::endl;
		NextType::print();
	}

	int mget()const{ return size; }
	template<typename _TT>
	int mgetv()const{
		return Finder<_TT, ThisType>::TType::mget();
	}
};
template<> struct ValAlloc<>{ //do a static assert
	static const int size = 0;
	static void init(int c){}

	static void print(){
		std::cout << "===empty===" << std::endl;
	}

	//int mget()const{ return 0; }
};

template<typename Head, typename... Tail>
int ValAlloc<Head, Tail...>::ki = 0;

template<class T, class VaT>struct Finder
{
	typedef Finder tFinder;
	typedef Finder<T, typename VaT::NextType> tnextFinder;
	typedef typename IF_<same_type<T, typename VaT::Type>::ret, typename VaT::ThisType, typename tnextFinder::TType>::reType TType;
};

template<class T>struct Finder<T, ValAlloc<>>
{
	typedef void TType;
};


typedef unsigned long long UINT64;
class temp{
public:
	static int get(){ return 11; }
	//static const UINT64 f = (UINT64)get;
};

//////////////////////////////////////////////////////////////////////////
template<typename T>
bool eq(const T& a, const T& b){ return !(a < b) && !(b < a); }

template<typename T = int>
struct bignum
{
	T _v;
	bignum(T v) :_v(v){}
	bool operator <(const bignum& r)const{ return _v < r._v; }
};

template<typename T, int N>
struct Vec
{
	T _v[N];
};
template<>
struct Vec<float, 3>
{
	float _v[3];
}; 
template<int N>
struct Vec<bool, N>{
	char _v[(N + sizeof(char)-1) / sizeof(char)];
};


template<unsigned long N>
struct binary
{
	static unsigned const value = binary<N / 10>::value << 1 | N % 10;
};
template<>
struct binary<0>
{
	static unsigned const value = 0;
};


template<class T> struct itr_traits;

template<class T> struct itr_traits<T*>{
	typedef T value_type;
};

template<class Itr>
struct iter_traits
{
	typedef typename Itr::value_type value_type;
};

template<class T> struct t_traits;
template<class T> struct t_traits<T*>
{
	typedef T value_type;
};

template<class FWItr1, class FWItr2>
void iter_swap(FWItr1 i1, FWItr2 i2){
	//typename itr_traits<FWItr1>::value_type tmp = *i1;
	typename iter_traits<t_traits<FWItr1>>::value_type tmp = *i1;
	//auto tmp = *i1;
	*i1 = *i2;
	*i2 = tmp;
}

void funs(int* a, int* b){
	iter_swap(a, b);
}


template<class T, typename F, typename G>
T apply_fg(T x, F f, G g){
	return f(g(x));
}


#include <functional>


void main_test(){
	auto one = binary<1>::value;
	auto three = binary<11>::value;
	auto five = binary<101>::value;

	int aaa = 1;
	int bbb = 2;
	funs(&aaa, &bbb);


	auto x = apply_fg(0.5f, std::negate<float>(), log2f);

}

template <unsigned int N>
struct Mask {
	enum { Value = (1 << N) };
};


#include "list.hpp"


#include <codecvt>


std::wstring utf8toUnicode(std::string utf8)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.from_bytes(utf8);
}

std::string utf8toMultibyte(std::string utf8)
{
	return std::string();
}

std::wstring multibytetoUnicode(std::string utf8)
{
	using wcmb = std::codecvt_byname < wchar_t, char, std::mbstate_t >;
	static std::wstring_convert<wcmb> conv(new wcmb(""));
	return conv.from_bytes(utf8);
}

std::string unicodeToMultibyte(std::wstring utf8)
{
	using wcmb = std::codecvt_byname < wchar_t, char, std::mbstate_t >;
	static std::wstring_convert<wcmb> conv(new wcmb(""));
	return conv.to_bytes(utf8);
}
#include<thread>
class testt {
	int m = 0;
public:
	void test() { m++; }
	void go() {
		std::thread([this] (){
			m++;
			test();
		});
	}
};

int main(int argc, char* argv[])
{
	return 0;
}

