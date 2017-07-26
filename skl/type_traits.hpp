#pragma once


#define _STD_BEGIN namespace skl{
#define _STD_END }

#define _STD skl::
#define _XSTD

//#define _CRTIMP2_PURE

#pragma pack(push,_CRT_PACKING)
#pragma warning(push,3)
#pragma push_macro("new")
#undef new
#pragma warning(disable: 4180 4296)


namespace skl{

#define __nullptr  0
typedef decltype(__nullptr) nullptr_t;
typedef double max_align_t;

//#define _NATIVE_WCHAR_T_DEFINED 1

#define _NOEXCEPT

#define _LONGLONG	__int64
#define _ULONGLONG	unsigned __int64
#define _LLONG_MAX	0x7fffffffffffffff
#define _ULLONG_MAX	0xffffffffffffffff



//////////////////////////////////////////////////////////////////////////
//xtr1common
//////////////////////////////////////////////////////////////////////////
// TEMPLATE CLASS unary_function
template<class _T1,
class _Ret>
struct unary_function;

// TEMPLATE CLASS binary_function
template<class _T1,
class _T2,
class _Ret>
struct binary_function;

// STRUCT _Nil
struct _Nil
{	// empty struct, for unused argument types
};
static _Nil _Nil_obj;

// TEMPLATE CLASS integral_constant
template<class _Ty,
	_Ty _Val>
struct integral_constant
{	// convenient template for integral constant types
	static const _Ty value = _Val;

	typedef _Ty value_type;
	typedef integral_constant<_Ty, _Val> type;

	operator value_type() const
	{	// return stored value
		return (value);
	}
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

// TEMPLATE CLASS _Cat_base
template<bool>
struct _Cat_base
	: false_type
{	// base class for type predicates
};

template<>
struct _Cat_base<true>
	: true_type
{	// base class for type predicates
};

// TEMPLATE CLASS enable_if
template<bool _Test,
class _Ty = void>
struct enable_if
{	// type is undefined for assumed !_Test
};

template<class _Ty>
struct enable_if<true, _Ty>
{	// type is _Ty for _Test
	typedef _Ty type;
};

// TEMPLATE CLASS conditional
template<bool _Test,
class _Ty1,
class _Ty2>
struct conditional
{	// type is _Ty2 for assumed !_Test
	typedef _Ty2 type;
};

template<class _Ty1,
class _Ty2>
struct conditional<true, _Ty1, _Ty2>
{	// type is _Ty1 for _Test
	typedef _Ty1 type;
};

// TEMPLATE CLASS is_same
template<class _Ty1, class _Ty2>
struct is_same
	: false_type
{	// determine whether _Ty1 and _Ty2 are the same type
};

template<class _Ty1>
struct is_same<_Ty1, _Ty1>
	: true_type
{	// determine whether _Ty1 and _Ty2 are the same type
};

// TEMPLATE CLASS remove_const
template<class _Ty>
struct remove_const
{	// remove top level const qualifier
	typedef _Ty type;
};

template<class _Ty>
struct remove_const<const _Ty>
{	// remove top level const qualifier
	typedef _Ty type;
};

template<class _Ty>
struct remove_const<const _Ty[]>
{	// remove top level const qualifier
	typedef _Ty type[];
};

template<class _Ty, unsigned int _Nx>
struct remove_const<const _Ty[_Nx]>
{	// remove top level const qualifier
	typedef _Ty type[_Nx];
};

// TEMPLATE CLASS remove_volatile
template<class _Ty>
struct remove_volatile
{	// remove top level volatile qualifier
	typedef _Ty type;
};

template<class _Ty>
struct remove_volatile<volatile _Ty>
{	// remove top level volatile qualifier
	typedef _Ty type;
};

template<class _Ty>
struct remove_volatile<volatile _Ty[]>
{	// remove top level volatile qualifier
	typedef _Ty type[];
};

template<class _Ty, unsigned int _Nx>
struct remove_volatile<volatile _Ty[_Nx]>
{	// remove top level volatile qualifier
	typedef _Ty type[_Nx];
};

// TEMPLATE CLASS remove_cv
template<class _Ty>
struct remove_cv
{	// remove top level const and volatile qualifiers
	typedef typename remove_const<typename remove_volatile<_Ty>::type>::type
		type;
};

// TEMPLATE CLASS _Is_integral
template<class _Ty>
struct _Is_integral
	: false_type
{	// determine whether _Ty is integral
};

template<>
struct _Is_integral<bool>
	: true_type
{	// determine whether _Ty is integral
};

template<>
struct _Is_integral<char>
	: true_type
{	// determine whether _Ty is integral
};

template<>
struct _Is_integral<unsigned char>
	: true_type
{	// determine whether _Ty is integral
};

template<>
struct _Is_integral<signed char>
	: true_type
{	// determine whether _Ty is integral
};

#ifdef _NATIVE_WCHAR_T_DEFINED
template<>
struct _Is_integral<wchar_t>
	: true_type
{	// determine whether _Ty is integral
};
#endif /* _NATIVE_WCHAR_T_DEFINED */

template<>
struct _Is_integral<unsigned short>
	: true_type
{	// determine whether _Ty is integral
};

template<>
struct _Is_integral<signed short>
	: true_type
{	// determine whether _Ty is integral
};

template<>
struct _Is_integral<unsigned int>
	: true_type
{	// determine whether _Ty is integral
};

template<>
struct _Is_integral<signed int>
	: true_type
{	// determine whether _Ty is integral
};

template<>
struct _Is_integral<unsigned long>
	: true_type
{	// determine whether _Ty is integral
};

template<>
struct _Is_integral<signed long>
	: true_type
{	// determine whether _Ty is integral
};

#if _HAS_CHAR16_T_LANGUAGE_SUPPORT
template<>
struct _Is_integral<char16_t>
	: true_type
{	// determine whether _Ty is integral
};

template<>
struct _Is_integral<char32_t>
	: true_type
{	// determine whether _Ty is integral
};
#endif /* _HAS_CHAR16_T_LANGUAGE_SUPPORT */

#ifdef _LONGLONG
template<>
struct _Is_integral<_LONGLONG>
	: true_type
{	// determine whether _Ty is integral
};

template<>
struct _Is_integral<_ULONGLONG>
	: true_type
{	// determine whether _Ty is integral
};
#endif /* _LONGLONG */

// TEMPLATE CLASS is_integral
template<class _Ty>
struct is_integral
	: _Is_integral<typename remove_cv<_Ty>::type>
{	// determine whether _Ty is integral
};

// TEMPLATE CLASS _Is_floating_point
template<class _Ty>
struct _Is_floating_point
	: false_type
{	// determine whether _Ty is floating point
};

template<>
struct _Is_floating_point<float>
	: true_type
{	// determine whether _Ty is floating point
};

template<>
struct _Is_floating_point<double>
	: true_type
{	// determine whether _Ty is floating point
};

template<>
struct _Is_floating_point<long double>
	: true_type
{	// determine whether _Ty is floating point
};

// TEMPLATE CLASS is_floating_point
template<class _Ty>
struct is_floating_point
	: _Is_floating_point<typename remove_cv<_Ty>::type>
{	// determine whether _Ty is floating point
};

template<class _Ty>
struct _Is_numeric
	: _Cat_base<is_integral<_Ty>::value
	|| is_floating_point<_Ty>::value>
{	// determine whether _Ty is numeric
};

// TEMPLATE remove_reference
template<class _Ty>
struct remove_reference
{	// remove reference
	typedef _Ty type;
};

template<class _Ty>
struct remove_reference<_Ty&>
{	// remove reference
	typedef _Ty type;
};

template<class _Ty>
struct remove_reference<_Ty&&>
{	// remove rvalue reference
	typedef _Ty type;
};

// TEMPLATE STRUCT _Copy_cv
template<class _Tgt,
class _Src>
struct _Copy_cv
{	// plain version
	typedef typename remove_reference<_Tgt>::type _Tgtx;
	typedef _Tgtx& type;
};

template<class _Tgt,
class _Src>
struct _Copy_cv<_Tgt, const _Src>
{	// const version
	typedef typename remove_reference<_Tgt>::type _Tgtx;
	typedef const _Tgtx& type;
};

template<class _Tgt,
class _Src>
struct _Copy_cv<_Tgt, volatile _Src>
{	// volatile version
	typedef typename remove_reference<_Tgt>::type _Tgtx;
	typedef volatile _Tgtx& type;
};

template<class _Tgt,
class _Src>
struct _Copy_cv<_Tgt, const volatile _Src>
{	// const volatile version
	typedef typename remove_reference<_Tgt>::type _Tgtx;
	typedef const volatile _Tgtx& type;
};

template<class _Tgt,
class _Src>
struct _Copy_cv<_Tgt, _Src&>
{	// remove reference from second argument
	typedef typename _Copy_cv<_Tgt, _Src>::type type;
};

// TYPE TESTING MACROS
struct _Wrap_int
{	// wraps int so that int argument is favored over _Wrap_int
	_Wrap_int(int)
	{	// do nothing
	}
};

template<class _Ty>
struct _Identity
{	// map _Ty to type unchanged, without operator()
	typedef _Ty type;
};

#define _GET_TYPE_OR_DEFAULT(TYPE, DEFAULT) \
{ \
	template<class _Uty> \
	static auto _Fn(int) \
	-> _Identity<typename _Uty::TYPE>; \
	\
	template<class _Uty> \
	static auto _Fn(_Wrap_int) \
	-> _Identity<DEFAULT>; \
	\
	typedef decltype(_Fn<_Ty>(0)) _Decltype; \
	typedef typename _Decltype::type type; \
}

#define _HAS_TYPES(TYPE1, TYPE2, TYPE3) \
{ \
	template<class _Uty> \
	static auto _Fn(int, \
	_Identity<typename _Uty::TYPE1> * = 0, \
	_Identity<typename _Uty::TYPE2> * = 0, \
	_Identity<typename _Uty::TYPE3> * = 0) \
	-> true_type; \
	\
	template<class _Uty> \
	static auto _Fn(_Wrap_int) \
	-> false_type; \
	\
	typedef decltype(_Fn<_Ty>(0)) type; \
}

#define _HAS_ONE_TYPE(TYPE) \
	_HAS_TYPES(TYPE, TYPE, TYPE)

// TEMPLATE STRUCT _Has_result_type
template<class _Ty>
struct _Has_result_type
	_HAS_ONE_TYPE(result_type);
//////////////////////////////////////////////////////////////////////////
//xstddef
//////////////////////////////////////////////////////////////////////////

// MISCELLANEOUS MACROS
#define _EMPTY_ARGUMENT		/* for empty macro argument */

// BITMASK MACROS
#define _BITMASK(Enum, Ty)	typedef int Ty
#define _BITMASK_OPS(Ty)

// TEMPLATE FUNCTION addressof
template<class _Ty> inline
_Ty *addressof(_Ty& _Val) _NOEXCEPT
{	// return address of _Val
	return (reinterpret_cast<_Ty *>(
	(&const_cast<char&>(
	reinterpret_cast<const volatile char&>(_Val)))));
}

// TYPE DEFINITIONS

template<bool,
class _Ty1,
class _Ty2>
struct _If
{	// type is _Ty2 for assumed false
	typedef _Ty2 type;
};

template<class _Ty1,
class _Ty2>
struct _If<true, _Ty1, _Ty2>
{	// type is _Ty1 for assumed true
	typedef _Ty1 type;
};

template<class _Ty>
struct _Always_false
{	// false value that probably won't be optimized away
	static const bool value = false;
};

//	FUNCTIONAL STUFF (from <functional>)
// TEMPLATE STRUCT unary_function
template<class _Arg,
class _Result>
struct unary_function
{	// base class for unary functions
	typedef _Arg argument_type;
	typedef _Result result_type;
};

// TEMPLATE STRUCT binary_function
template<class _Arg1,
class _Arg2,
class _Result>
struct binary_function
{	// base class for binary functions
	typedef _Arg1 first_argument_type;
	typedef _Arg2 second_argument_type;
	typedef _Result result_type;
};

// TEMPLATE STRUCT plus
template<class _Ty = void>
struct plus
	: public binary_function<_Ty, _Ty, _Ty>
{	// functor for operator+
	_Ty operator()(const _Ty& _Left, const _Ty& _Right) const
	{	// apply operator+ to operands
		return (_Left + _Right);
	}
};

// TEMPLATE STRUCT minus
template<class _Ty = void>
struct minus
	: public binary_function<_Ty, _Ty, _Ty>
{	// functor for operator-
	_Ty operator()(const _Ty& _Left, const _Ty& _Right) const
	{	// apply operator- to operands
		return (_Left - _Right);
	}
};

// TEMPLATE STRUCT multiplies
template<class _Ty = void>
struct multiplies
	: public binary_function<_Ty, _Ty, _Ty>
{	// functor for operator*
	_Ty operator()(const _Ty& _Left, const _Ty& _Right) const
	{	// apply operator* to operands
		return (_Left * _Right);
	}
};

// TEMPLATE STRUCT equal_to
template<class _Ty = void>
struct equal_to
	: public binary_function<_Ty, _Ty, bool>
{	// functor for operator==
	bool operator()(const _Ty& _Left, const _Ty& _Right) const
	{	// apply operator== to operands
		return (_Left == _Right);
	}
};

// TEMPLATE STRUCT less
template<class _Ty = void>
struct less
	: public binary_function<_Ty, _Ty, bool>
{	// functor for operator<
	bool operator()(const _Ty& _Left, const _Ty& _Right) const
	{	// apply operator< to operands
		return (_Left < _Right);
	}
};

// TEMPLATE STRUCT SPECIALIZATION plus
template<>
struct plus<void>
{	// transparent functor for operator+
	template<class _Ty1,
	class _Ty2>
		auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(static_cast<_Ty1&&>(_Left)
		+static_cast<_Ty2&&>(_Right))
	{	// transparently apply operator+ to operands
			return (static_cast<_Ty1&&>(_Left)
				+static_cast<_Ty2&&>(_Right));
	}
};

// TEMPLATE STRUCT SPECIALIZATION minus
template<>
struct minus<void>
{	// transparent functor for operator-
	template<class _Ty1,
	class _Ty2>
		auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(static_cast<_Ty1&&>(_Left)
		-static_cast<_Ty2&&>(_Right))
	{	// transparently apply operator- to operands
			return (static_cast<_Ty1&&>(_Left)
				-static_cast<_Ty2&&>(_Right));
	}
};

// TEMPLATE STRUCT SPECIALIZATION multiplies
template<>
struct multiplies<void>
{	// transparent functor for operator*
	template<class _Ty1,
	class _Ty2>
		auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(static_cast<_Ty1&&>(_Left)
		* static_cast<_Ty2&&>(_Right))
	{	// transparently apply operator* to operands
			return (static_cast<_Ty1&&>(_Left)
				* static_cast<_Ty2&&>(_Right));
	}
};

// TEMPLATE STRUCT SPECIALIZATION equal_to
template<>
struct equal_to<void>
{	// transparent functor for operator==
	template<class _Ty1,
	class _Ty2>
		auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(static_cast<_Ty1&&>(_Left)
		== static_cast<_Ty2&&>(_Right))
	{	// transparently apply operator== to operands
			return (static_cast<_Ty1&&>(_Left)
				== static_cast<_Ty2&&>(_Right));
	}
};

// TEMPLATE STRUCT SPECIALIZATION less
template<>
struct less<void>
{	// transparent functor for operator<
	template<class _Ty1,
	class _Ty2>
		auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(static_cast<_Ty1&&>(_Left)
		< static_cast<_Ty2&&>(_Right))
	{	// transparently apply operator< to operands
			return (static_cast<_Ty1&&>(_Left)
				< static_cast<_Ty2&&>(_Right));
	}
};

#ifndef _HASH_SEQ_DEFINED
#define _HASH_SEQ_DEFINED

// FUNCTION _Hash_seq
inline size_t _Hash_seq(const unsigned char *_First, size_t _Count)
{	// FNV-1a hash function for bytes in [_First, _First+_Count)
#if defined(_M_X64) || defined(_LP64) || defined(__x86_64) || defined(_WIN64)
	static_assert(sizeof(size_t) == 8, "This code is for 64-bit size_t.");
	const size_t _FNV_offset_basis = 14695981039346656037ULL;
	const size_t _FNV_prime = 1099511628211ULL;

#else /* defined(_M_X64), etc. */
	static_assert(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
	const size_t _FNV_offset_basis = 2166136261U;
	const size_t _FNV_prime = 16777619U;
#endif /* defined(_M_X64), etc. */

	size_t _Val = _FNV_offset_basis;
	for (size_t _Next = 0; _Next < _Count; ++_Next)
	{	// fold in another byte
		_Val ^= (size_t)_First[_Next];
		_Val *= _FNV_prime;
	}

#if defined(_M_X64) || defined(_LP64) || defined(__x86_64) || defined(_WIN64)
	static_assert(sizeof(size_t) == 8, "This code is for 64-bit size_t.");
	_Val ^= _Val >> 32;

#else /* defined(_M_X64), etc. */
	static_assert(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
#endif /* defined(_M_X64), etc. */

	return (_Val);
}

// TEMPLATE STRUCT _Bitwise_hash
template<class _Kty>
struct _Bitwise_hash
	: public unary_function<_Kty, size_t>
{	// hash functor for plain old data
	size_t operator()(const _Kty& _Keyval) const
	{	// hash _Keyval to size_t value by pseudorandomizing transform
		return (_Hash_seq((const unsigned char *)&_Keyval, sizeof (_Kty)));
	}
};

// TEMPLATE STRUCT hash
template<class _Kty>
struct hash
	: public _Bitwise_hash<_Kty>
{	// hash functor for enums
	static const bool _Value = __is_enum(_Kty);
	static_assert(_Value,
		"The C++ Standard doesn't provide a hash for this type.");
};
template<>
struct hash<bool>
	: public _Bitwise_hash<bool>
{	// hash functor for bool
};

template<>
struct hash<char>
	: public _Bitwise_hash<char>
{	// hash functor for char
};

template<>
struct hash<signed char>
	: public _Bitwise_hash<signed char>
{	// hash functor for signed char
};

template<>
struct hash<unsigned char>
	: public _Bitwise_hash<unsigned char>
{	// hash functor for unsigned char
};

#if _HAS_CHAR16_T_LANGUAGE_SUPPORT
template<>
struct hash<char16_t>
	: public _Bitwise_hash<char16_t>
{	// hash functor for char16_t
};

template<>
struct hash<char32_t>
	: public _Bitwise_hash<char32_t>
{	// hash functor for char32_t
};
#endif /* _HAS_CHAR16_T_LANGUAGE_SUPPORT */

#ifdef _NATIVE_WCHAR_T_DEFINED
template<>
struct hash<wchar_t>
	: public _Bitwise_hash<wchar_t>
{	// hash functor for wchar_t
};
#endif /* _NATIVE_WCHAR_T_DEFINED */

template<>
struct hash<short>
	: public _Bitwise_hash<short>
{	// hash functor for short
};

template<>
struct hash<unsigned short>
	: public _Bitwise_hash<unsigned short>
{	// hash functor for unsigned short
};

template<>
struct hash<int>
	: public _Bitwise_hash<int>
{	// hash functor for int
};

template<>
struct hash<unsigned int>
	: public _Bitwise_hash<unsigned int>
{	// hash functor for unsigned int
};

template<>
struct hash<long>
	: public _Bitwise_hash<long>
{	// hash functor for long
};

template<>
struct hash<unsigned long>
	: public _Bitwise_hash<unsigned long>
{	// hash functor for unsigned long
};

template<>
struct hash<long long>
	: public _Bitwise_hash<long long>
{	// hash functor for long long
};

template<>
struct hash<unsigned long long>
	: public _Bitwise_hash<unsigned long long>
{	// hash functor for unsigned long long
};

template<>
struct hash<float>
	: public _Bitwise_hash<float>
{	// hash functor for float
	typedef float _Kty;
	typedef _Bitwise_hash<_Kty> _Mybase;

	size_t operator()(const _Kty& _Keyval) const
	{	// hash _Keyval to size_t value by pseudorandomizing transform
		return (_Mybase::operator()(
			_Keyval == 0 ? 0 : _Keyval)); // map -0 to 0
	}
};

template<>
struct hash<double>
	: public _Bitwise_hash<double>
{	// hash functor for double
	typedef double _Kty;
	typedef _Bitwise_hash<_Kty> _Mybase;

	size_t operator()(const _Kty& _Keyval) const
	{	// hash _Keyval to size_t value by pseudorandomizing transform
		return (_Mybase::operator()(
			_Keyval == 0 ? 0 : _Keyval)); // map -0 to 0
	}
};

template<>
struct hash<long double>
	: public _Bitwise_hash<long double>
{	// hash functor for long double
	typedef long double _Kty;
	typedef _Bitwise_hash<_Kty> _Mybase;

	size_t operator()(const _Kty& _Keyval) const
	{	// hash _Keyval to size_t value by pseudorandomizing transform
		return (_Mybase::operator()(
			_Keyval == 0 ? 0 : _Keyval)); // map -0 to 0
	}
};

template<class _Ty>
struct hash<_Ty *>
	: public _Bitwise_hash<_Ty *>
{	// hash functor for _Ty *
};

#endif /* _HASH_SEQ_DEFINED */

#if defined(MRTDLL) && defined(_CRTBLD)

#if !defined(_NATIVE_WCHAR_T_DEFINED) && defined(_M_CEE_PURE)
extern "C++"

#else /* !defined(_NATIVE_WCHAR_T_DEFINED) etc. */
extern "C"
#endif /* !defined(_NATIVE_WCHAR_T_DEFINED) etc. */

_CRTIMP __declspec(noreturn) void __cdecl _invoke_watson(
_In_opt_z_ const wchar_t *,
_In_opt_z_ const wchar_t *,
_In_opt_z_ const wchar_t *,
unsigned int,
uintptr_t);
#endif /* MRTDLL && _CRTBLD */

#ifdef _M_IX86

#ifdef _M_CEE
#define _NON_MEMBER_CALL(FUNC, CONST_OPT) \
	FUNC(__cdecl, CONST_OPT) \
	FUNC(__stdcall, CONST_OPT) \
	FUNC(__clrcall, CONST_OPT)
#define _MEMBER_CALL(FUNC, CONST_OPT, CV_OPT) \
	FUNC(__thiscall, CONST_OPT, CV_OPT) \
	FUNC(__cdecl, CONST_OPT, CV_OPT) \
	FUNC(__stdcall, CONST_OPT, CV_OPT) \
	FUNC(__clrcall, CONST_OPT, CV_OPT)

#else /* _M_CEE */
#define _NON_MEMBER_CALL(FUNC, CONST_OPT) \
	FUNC(__cdecl, CONST_OPT) \
	FUNC(__stdcall, CONST_OPT) \
	FUNC(__fastcall, CONST_OPT)
#define _MEMBER_CALL(FUNC, CONST_OPT, CV_OPT) \
	FUNC(__thiscall, CONST_OPT, CV_OPT) \
	FUNC(__cdecl, CONST_OPT, CV_OPT) \
	FUNC(__stdcall, CONST_OPT, CV_OPT) \
	FUNC(__fastcall, CONST_OPT, CV_OPT)
#endif /* _M_CEE */

#else /* _M_IX86 */

#ifdef _M_CEE
#define _NON_MEMBER_CALL(FUNC, CONST_OPT) \
	FUNC(__cdecl, CONST_OPT) \
	FUNC(__clrcall, CONST_OPT)
#define _MEMBER_CALL(FUNC, CONST_OPT, CV_OPT) \
	FUNC(__cdecl, CONST_OPT, CV_OPT) \
	FUNC(__clrcall, CONST_OPT, CV_OPT)

#else /* _M_CEE */
#define _NON_MEMBER_CALL(FUNC, CONST_OPT) \
	FUNC(__cdecl, CONST_OPT)
#define _MEMBER_CALL(FUNC, CONST_OPT, CV_OPT) \
	FUNC(__cdecl, CONST_OPT, CV_OPT)
#endif /* _M_CEE */
#endif /* _M_IX86 */

#define _NON_MEMBER_CALL_CONST(FUNC) \
	_NON_MEMBER_CALL(FUNC, ) \
	_NON_MEMBER_CALL(FUNC, const)

#define _MEMBER_CALL_CV(FUNC, CONST_OPT) \
	_MEMBER_CALL(FUNC, CONST_OPT, ) \
	_MEMBER_CALL(FUNC, CONST_OPT, const) \
	_MEMBER_CALL(FUNC, CONST_OPT, volatile) \
	_MEMBER_CALL(FUNC, CONST_OPT, const volatile)

#define _MEMBER_CALL_CONST_CV(FUNC) \
	_MEMBER_CALL_CV(FUNC, ) \
	_MEMBER_CALL_CV(FUNC, const)

#define _CLASS_DEFINE_CV(CLASS) \
	CLASS(_EMPTY_ARGUMENT) \
	CLASS(const) \
	CLASS(volatile) \
	CLASS(const volatile)

#define _COMMA	,	/* for immediate commas in macro parameters */

#define _Comma	,
#define _EX(...)	__VA_ARGS__




//////////////////////////////////////////////////////////////////////////
//type_traits
//////////////////////////////////////////////////////////////////////////
// COMPILER SUPPORT MACROS

// VC++ V14 SUPPORT
#define _IS_BASE_OF(_Base, _Der)	\
	: _Cat_base<__is_base_of(_Base, _Der)>
#define _IS_CONVERTIBLE(_From, _To)	\
	: _Cat_base<__is_convertible_to(_From, _To)>
#define _IS_UNION(_Ty)	\
	: _Cat_base<__is_union(_Ty)>
#define _IS_CLASS(_Ty)	\
	: _Cat_base<__is_class(_Ty)>
#define _IS_POD(_Ty)	\
	: _Cat_base<__is_pod(_Ty)>
#define _IS_EMPTY(_Ty)	\
	: _Cat_base<__is_empty(_Ty)>
#define _IS_POLYMORPHIC(_Ty)	\
	: _Cat_base<__is_polymorphic(_Ty)>
#define _IS_ABSTRACT(_Ty)	\
	: _Cat_base<__is_abstract(_Ty)>
#define _IS_STANDARD_LAYOUT(_Ty)	\
	: _Cat_base<__is_standard_layout(_Ty)>
#define _IS_TRIVIAL(_Ty)	\
	: _Cat_base<__is_trivial(_Ty)>
#define _HAS_TRIVIAL_DESTRUCTOR(_Ty)	\
	: _Cat_base<__has_trivial_destructor(_Ty)>
#define _HAS_VIRTUAL_DESTRUCTOR(_Ty)	\
	: _Cat_base<__has_virtual_destructor(_Ty)>
#define _UNDERLYING_TYPE(_Ty)	\
	__underlying_type(_Ty)
#define _IS_LITERAL_TYPE(_Ty)	\
	: _Cat_base<__is_literal_type(_Ty)>
#define _IS_ENUM(_Ty)	\
	: _Cat_base<__is_enum(_Ty)>
#define _IS_DESTRUCTIBLE(_Ty) \
	: _Cat_base<__is_destructible(_Ty)>
#define _IS_NOTHROW_ASSIGNABLE(_To, _From) \
	: _Cat_base<__is_nothrow_assignable(_To, _From)>
#define _IS_NOTHROW_DESTRUCTIBLE(_Ty) \
	: _Cat_base<__is_nothrow_destructible(_Ty)>
#define _IS_TRIVIALLY_ASSIGNABLE(_To, _From) \
	: _Cat_base<__is_trivially_assignable(_To, _From)>
#define _IS_CONSTRUCTIBLE \
	__is_constructible
#define _IS_NOTHROW_CONSTRUCTIBLE \
	__is_nothrow_constructible
#define _IS_TRIVIALLY_CONSTRUCTIBLE \
	__is_trivially_constructible

// ADDITIONAL INTRINSICS
// _ALIGN_OF(_Ty)
// _IS_ASSIGNABLE(_To, _From)

// TEMPLATE CLASS _Ptr_traits
template<class _Ty>
struct _Ptr_traits
{	// basic definition
};

template<class _Ty>
struct _Ptr_traits<_Ty *>
{	// pointer properties
	static const bool _Is_const = false;
	static const bool _Is_volatile = false;
};

template<class _Ty>
struct _Ptr_traits<const _Ty *>
{	// pointer to const properties
	static const bool _Is_const = true;
	static const bool _Is_volatile = false;
};

template<class _Ty>
struct _Ptr_traits<volatile _Ty *>
{	// pointer to volatile properties
	static const bool _Is_const = false;
	static const bool _Is_volatile = true;
};

template<class _Ty>
struct _Ptr_traits<const volatile _Ty *>
{	// pointer to const volatile properties
	static const bool _Is_const = true;
	static const bool _Is_volatile = true;
};

template<class _Ty>
struct _Is_funptr
	: false_type
{	// base class for function pointer predicates
};

template<class _Ty>
struct _Is_memfunptr
	: false_type
{	// base class for member function pointer predicates
};

#define _IS_FUNPTR(CALL_OPT, X1) \
	template<class _Ret, \
class... _Types> \
struct _Is_funptr<_Ret (CALL_OPT *)(_Types...)> \
	: true_type \
{	/* base class for function pointer predicates */ \
};

_NON_MEMBER_CALL(_IS_FUNPTR, )
#undef _IS_FUNPTR

#define _IS_MEMFUNPTR(CALL_OPT, X1, CV_OPT) \
	template<class _Ret, \
class _Arg0, \
class... _Types> \
struct _Is_memfunptr<_Ret (CALL_OPT _Arg0::*)(_Types...) CV_OPT> \
	: true_type \
{	/* base class for member function pointer predicates */ \
};

_MEMBER_CALL_CV(_IS_MEMFUNPTR, )
#undef _IS_MEMFUNPTR

template<class _Ret,
class... _Types>
struct _Is_funptr<_Ret(*)(_Types..., ...)>
	: true_type
{	// no calling conventions for ellipsis
};

#define _IS_MEMFUNPTR_ELLIPSIS(CV_OPT) \
	template<class _Ret, \
class _Arg0, \
class... _Types> \
struct _Is_memfunptr<_Ret (_Arg0::*)(_Types..., ...) CV_OPT> \
	: true_type \
{	/* no calling conventions for ellipsis */ \
};

_CLASS_DEFINE_CV(_IS_MEMFUNPTR_ELLIPSIS)
#undef _IS_MEMFUNPTR_ELLIPSIS

// Type modifiers
// TEMPLATE CLASS add_const
template<class _Ty>
struct add_const
{	// add top level const qualifier
	typedef const _Ty type;
};

// TEMPLATE CLASS add_volatile
template<class _Ty>
struct add_volatile
{	// add top level volatile qualifier
	typedef volatile _Ty type;
};

// TEMPLATE CLASS add_cv
template<class _Ty>
struct add_cv
{	// add top level const and volatile qualifiers
	typedef const volatile _Ty type;
};

// TEMPLATE CLASS add_reference -- retained
template<class _Ty>
struct add_reference
{	// add reference
	typedef _Ty& type;
};

#define _ADD_REFERENCE_VOID(CV_OPT) \
	template<> \
struct add_reference<CV_OPT void> \
{	/* add reference */ \
	typedef CV_OPT void type; \
};

_CLASS_DEFINE_CV(_ADD_REFERENCE_VOID)
#undef _ADD_REFERENCE_VOID

// TEMPLATE CLASS add_lvalue_reference
template<class _Ty>
struct add_lvalue_reference
{	// add lvalue reference
	typedef typename add_reference<_Ty>::type type;
};

// TEMPLATE CLASS add_rvalue_reference

template<class _Ty>
struct add_rvalue_reference
{	// add rvalue reference
	typedef _Ty&& type;
};

#define _ADD_RVALUE_REFERENCE_VOID(CV_OPT) \
	template<> \
struct add_rvalue_reference<CV_OPT void> \
{	/* add reference */ \
	typedef CV_OPT void type; \
};

_CLASS_DEFINE_CV(_ADD_RVALUE_REFERENCE_VOID)
#undef _ADD_RVALUE_REFERENCE_VOID

// TEMPLATE FUNCTION declval
template<class _Ty>
typename add_rvalue_reference<_Ty>::type
declval() _NOEXCEPT;

// TEMPLATE CLASS remove_extent
template<class _Ty>
struct remove_extent
{	// remove array extent
	typedef _Ty type;
};

template<class _Ty, unsigned int _Ix>
struct remove_extent<_Ty[_Ix]>
{	// remove array extent
	typedef _Ty type;
};

template<class _Ty>
struct remove_extent<_Ty[]>
{	// remove array extent
	typedef _Ty type;
};

// TEMPLATE CLASS remove_all_extents
template<class _Ty>
struct remove_all_extents
{	// remove all array extents
	typedef _Ty type;
};

template<class _Ty, unsigned int _Ix>
struct remove_all_extents<_Ty[_Ix]>
{	// remove all array extents
	typedef typename remove_all_extents<_Ty>::type type;
};

template<class _Ty>
struct remove_all_extents<_Ty[]>
{	// remove all array extents
	typedef typename remove_all_extents<_Ty>::type type;
};

// TEMPLATE CLASS remove_pointer
template<class _Ty>
struct remove_pointer
{	// remove pointer
	typedef _Ty type;
};

#define _REMOVE_POINTER(CV_OPT) \
	template<class _Ty> \
struct remove_pointer<_Ty *CV_OPT> \
{	/* remove pointer */ \
	typedef _Ty type; \
};

_CLASS_DEFINE_CV(_REMOVE_POINTER)
#undef _REMOVE_POINTER

// TEMPLATE CLASS add_pointer
template<class _Ty>
struct add_pointer
{	// add pointer
	typedef typename remove_reference<_Ty>::type *type;
};

// TYPE PREDICATES
// TEMPLATE CLASS is_void
template<class _Ty>
struct _Is_void
	: false_type
{	// determine whether _Ty is void
};

template<>
struct _Is_void<void>
	: true_type
{	// determine whether _Ty is void
};

template<class _Ty>
struct is_void
	: _Is_void<typename remove_cv<_Ty>::type>
{	// determine whether _Ty is void
};

// TEMPLATE CLASS is_array
template<class _Ty>
struct is_array
	: false_type
{	// determine whether _Ty is an array
};

template<class _Ty, size_t _Nx>
struct is_array<_Ty[_Nx]>
	: true_type
{	// determine whether _Ty is an array
};

template<class _Ty>
struct is_array<_Ty[]>
	: true_type
{	// determine whether _Ty is an array
};

// TEMPLATE CLASS is_lvalue_reference
template<class _Ty>
struct is_lvalue_reference
	: false_type
{	// determine whether _Ty is an lvalue reference
};

template<class _Ty>
struct is_lvalue_reference<_Ty&>
	: true_type
{	// determine whether _Ty is an lvalue reference
};

// TEMPLATE CLASS is_rvalue_reference
template<class _Ty>
struct is_rvalue_reference
	: false_type
{	// determine whether _Ty is an rvalue reference
};

template<class _Ty>
struct is_rvalue_reference<_Ty&&>
	: true_type
{	// determine whether _Ty is an rvalue reference
};

// TEMPLATE CLASS is_reference
template<class _Ty>
struct is_reference
	: _Cat_base<is_lvalue_reference<_Ty>::value
	|| is_rvalue_reference<_Ty>::value>
{	// determine whether _Ty is a reference
};


// TEMPLATE CLASS is_member_object_pointer
template<class _Ty>
struct _Is_member_object_pointer
	: false_type
{	// determine whether _Ty is a pointer to member object
};

template<class _Ty1, class _Ty2>
struct _Is_member_object_pointer<_Ty1 _Ty2::*>
	: _Cat_base<!_Is_memfunptr<_Ty1 _Ty2::*>::value>
{	// determine whether _Ty is a pointer to member object
};

template<class _Ty>
struct is_member_object_pointer
	: _Is_member_object_pointer<typename remove_cv<_Ty>::type>
{	// determine whether _Ty is a pointer to member object
};

// TEMPLATE CLASS is_member_function_pointer
template<class _Ty>
struct is_member_function_pointer
	: _Cat_base<_Is_memfunptr<typename remove_cv<_Ty>::type>::value>
{	// determine whether _Ty is a pointer to member function
};

// TEMPLATE CLASS is_pointer
template<class _Ty>
struct _Is_pointer
	: false_type
{	// determine whether _Ty is a pointer
};

template<class _Ty>
struct _Is_pointer<_Ty *>
	: _Cat_base<!is_member_object_pointer<_Ty *>::value
	&& !is_member_function_pointer<_Ty *>::value>
{	// determine whether _Ty is a pointer
};

template<class _Ty>
struct is_pointer
	: _Is_pointer<typename remove_cv<_Ty>::type>
{	// determine whether _Ty is a pointer
};

// TEMPLATE CLASS _Is_nullptr_t

template<class _Ty>
struct _Is_nullptr_t
	: _Cat_base<is_same<typename remove_cv<_Ty>::type, nullptr_t>::value>
{	// determine whether _Ty is nullptr_t
};

// TEMPLATE CLASS is_union
template<class _Ty>
struct is_union _IS_UNION(_Ty)
{	// determine whether _Ty is a union
};

// TEMPLATE CLASS is_class
template<class _Ty>
struct is_class _IS_CLASS(_Ty)
{	// determine whether _Ty is a class
};

// TEMPLATE CLASS is_function
template<class _Ty>
struct is_function
	: _Cat_base<_Is_funptr<typename remove_cv<_Ty>::type *>::value>
{	// determine whether _Ty is a function
};

template<class _Ty>
struct is_function<_Ty&>
	: false_type
{	// determine whether _Ty is a function
};

template<class _Ty>
struct is_function<_Ty&&>
	: false_type
{	// determine whether _Ty is a function
};

// TEMPLATE CLASS is_arithmetic
template<class _Ty>
struct is_arithmetic
	: _Cat_base<is_integral<_Ty>::value
	|| is_floating_point<_Ty>::value>
{	// determine whether _Ty is an arithmetic type
};

// TEMPLATE CLASS is_fundamental
template<class _Ty>
struct is_fundamental
	: _Cat_base<is_arithmetic<_Ty>::value
	|| is_void<_Ty>::value
	|| _Is_nullptr_t<_Ty>::value>
{	// determine whether _Ty is a fundamental type
};

// TEMPLATE CLASS is_object
template<class _Ty>
struct is_object
	: _Cat_base<!is_function<_Ty>::value
	&& !is_reference<_Ty>::value
	&& !is_void<_Ty>::value>
{	// determine whether _Ty is an object type
};

// TEMPLATE CLASS is_convertible

template<class _From, class _To>
struct is_convertible
	_IS_CONVERTIBLE(_From, _To)
{	// determine whether _From is convertible to _To
};

// TEMPLATE CLASS is_enum
template<class _Ty>
struct is_enum
	_IS_ENUM(_Ty)
{	// determine whether _Ty is an enumerated type
};

// TEMPLATE CLASS is_compound
template<class _Ty>
struct is_compound
	: _Cat_base<!is_fundamental<_Ty>::value>
{	// determine whether _Ty is a compound type
};

// TEMPLATE CLASS is_member_pointer
template<class _Ty>
struct is_member_pointer
	: _Cat_base<is_member_object_pointer<_Ty>::value
	|| is_member_function_pointer<_Ty>::value>
{	// determine whether _Ty is a pointer to member
};

// TEMPLATE CLASS is_scalar
template<class _Ty>
struct is_scalar
	: _Cat_base<is_arithmetic<_Ty>::value
	|| is_enum<_Ty>::value
	|| is_pointer<_Ty>::value
	|| is_member_pointer<_Ty>::value
	|| _Is_nullptr_t<_Ty>::value>
{	// determine whether _Ty is a scalar type
};

// TEMPLATE CLASS is_const
template<class _Ty>
struct is_const
	: _Cat_base<_Ptr_traits<_Ty *>::_Is_const
	&& !is_function<_Ty>::value>
{	// determine whether _Ty is const qualified
};

template<class _Ty, unsigned int _Nx>
struct is_const<_Ty[_Nx]>
	: false_type
{	// determine whether _Ty is const qualified
};

template<class _Ty, unsigned int _Nx>
struct is_const<const _Ty[_Nx]>
	: true_type
{	// determine whether _Ty is const qualified
};

template<class _Ty>
struct is_const<_Ty&>
	: false_type
{	// determine whether _Ty is const qualified
};

template<class _Ty>
struct is_const<_Ty&&>
	: false_type
{	// determine whether _Ty is const qualified
};

// TEMPLATE CLASS is_volatile
template<class _Ty>
struct is_volatile
	: _Cat_base<_Ptr_traits<_Ty *>::_Is_volatile
	&& !is_function<_Ty>::value>
{	// determine whether _Ty is volatile qualified
};

template<class _Ty>
struct is_volatile<_Ty&>
	: false_type
{	// determine whether _Ty is volatile qualified
};

template<class _Ty>
struct is_volatile<_Ty&&>
	: false_type
{	// determine whether _Ty is volatile qualified
};

// TEMPLATE CLASS is_pod
template<class _Ty>
struct is_pod _IS_POD(_Ty)
{	// determine whether _Ty is a POD type
};

// TEMPLATE CLASS is_empty
template<class _Ty>
struct is_empty _IS_EMPTY(_Ty)
{	// determine whether _Ty is an empty class
};

// TEMPLATE CLASS is_polymorphic
template<class _Ty>
struct is_polymorphic _IS_POLYMORPHIC(_Ty)
{	// determine whether _Ty is a polymorphic type
};

// TEMPLATE CLASS is_abstract
template<class _Ty>
struct is_abstract _IS_ABSTRACT(_Ty)
{	// determine whether _Ty is an abstract class
};

// TEMPLATE CLASS is_standard_layout
template<class _Ty>
struct is_standard_layout _IS_STANDARD_LAYOUT(_Ty)
{	// determine whether _Ty is standard layout
};

// TEMPLATE CLASS is_literal_type
template<class _Ty>
struct is_literal_type _IS_LITERAL_TYPE(_Ty)
{	// determine whether _Ty is a literal type
};

// TEMPLATE CLASS is_trivial
template<class _Ty>
struct is_trivial _IS_TRIVIAL(_Ty)
{	// determine whether _Ty is trivial
};

// TEMPLATE CLASS has_virtual_destructor
template<class _Ty>
struct has_virtual_destructor
	_HAS_VIRTUAL_DESTRUCTOR(_Ty)
{	// determine whether _Ty has a virtual destructor
};

// CONSTRUCTIBLE/ASSIGNABLE TRAITS
// TEMPLATE CLASS is_constructible


template<class _Ty,
class... _Args>
struct is_constructible
	: _Cat_base<_IS_CONSTRUCTIBLE(_Ty, _Args...)>
{	// determine whether _Ty(_Args...) is constructible
};


// TEMPLATE CLASS is_copy_constructible

template<class _Ty>
struct is_copy_constructible
	: is_constructible<
	_Ty,
	typename add_lvalue_reference<
	typename add_const<_Ty>::type
	>::type
	>::type
{	// determine whether _Ty has a copy constructor
};

// TEMPLATE CLASS has_copy_constructor -- retained
template<class _Ty>
struct has_copy_constructor
	: is_copy_constructible<_Ty>::type
{	// determine whether _Ty has a copy constructor
};

// TEMPLATE CLASS is_default_constructible
template<class _Ty>
struct is_default_constructible
	: is_constructible<_Ty>::type
{	// determine whether _Ty has a default constructor
};

// TEMPLATE CLASS has_default_constructor -- retained
template<class _Ty>
struct has_default_constructor
	: is_default_constructible<_Ty>::type
{	// determine whether _Ty has a default constructor
};

// TEMPLATE CLASS is_move_constructible
template<class _Ty>
struct is_move_constructible
	: is_constructible<

	_Ty,

	typename add_rvalue_reference<_Ty>::type
	>::type
{	// determine whether _Ty has a move constructor
};

// TEMPLATE CLASS has_move_constructor -- retained
template<class _Ty>
struct has_move_constructor
	: is_move_constructible<_Ty>::type
{	// determine whether _Ty has a move constructor
};

// TEMPLATE CLASS is_assignable

template<class _To,
class _From>
struct _Is_assignable
{	// tests for valid assign expression
	template<class _Dest,
	class _Src>
		static auto _Fn(int)
		-> decltype((_STD declval<_Dest>() = _STD declval<_Src>()),
		true_type());

	template<class _Dest,
	class _Src>
		static auto _Fn(_Wrap_int)
		->false_type;

	typedef decltype(_Fn<_To, _From>(0)) type;
};

template<class _To,
class _From>
struct is_assignable
	: _Is_assignable<_To, _From>::type
	//		_IS_ASSIGNABLE(_To, _From)
{	// determine whether _From can be assigned to _To
};

// TEMPLATE CLASS is_copy_assignable

template<class _Ty>
struct is_copy_assignable
	: is_assignable<
	typename add_lvalue_reference<_Ty>::type,
	typename add_lvalue_reference<
	typename add_const<_Ty>::type
	>::type
	>::type
{	// determine whether _Ty has a copy assignment operator
};

// TEMPLATE CLASS has_copy_assign -- retained
template<class _Ty>
struct has_copy_assign
	: is_copy_assignable<_Ty>::type
{	// determine whether _Ty has a copy assignment operator
};

// TEMPLATE CLASS is_move_assignable

template<class _Ty>
struct is_move_assignable
	: is_assignable<
	typename add_lvalue_reference<_Ty>::type,
	typename add_rvalue_reference<_Ty>::type
	>::type
{	// determine whether _Ty has a move assignment operator
};

template<class _Ty>
struct has_move_assign
	: is_move_assignable<_Ty>::type
{	// determine whether _Ty has a move assignment operator
};

// TEMPLATE CLASS is_destructible
template<class _Ty>
struct is_destructible
	_IS_DESTRUCTIBLE(_Ty)
{	// determine whether _Ty has a destructor
};

// TRIVIAL TRAITS
// TEMPLATE CLASS is_trivially_constructible


template<class _Ty,
class... _Args>
struct is_trivially_constructible
	: _Cat_base<_IS_TRIVIALLY_CONSTRUCTIBLE(_Ty, _Args...)>
{	// determine whether _Ty(_Args...) is trivially constructible
};


// TEMPLATE CLASS is_trivially_copy_constructible
template<class _Ty>
struct is_trivially_copy_constructible

	: is_trivially_constructible<
	_Ty,
	typename add_lvalue_reference<
	typename add_const<_Ty>::type
	>::type
	>::type

{	// determine whether _Ty has a trivial copy constructor
};

// TEMPLATE CLASS is_trivially_copyable
template<class _Ty>
struct is_trivially_copyable
	: is_trivially_copy_constructible<_Ty>::type
{	// determine whether _Ty has a trivial copy constructor
};

// TEMPLATE CLASS has_trivial_copy_constructor -- retained
template<class _Ty>
struct has_trivial_copy_constructor
	: is_trivially_copy_constructible<_Ty>::type
{	// determine whether _Ty has a trivial copy constructor
};

// TEMPLATE CLASS is_trivially_default_constructible
template<class _Ty>
struct is_trivially_default_constructible
	: is_trivially_constructible<_Ty>::type
{	// determine whether _Ty has a trivial default constructor
};

// TEMPLATE CLASS has_trivial_default_constructor -- retained
template<class _Ty>
struct has_trivial_default_constructor
	: is_trivially_default_constructible<_Ty>::type
{	// determine whether _Ty has a trivial constructor
};

// TEMPLATE CLASS is_trivially_move_constructible
template<class _Ty>
struct is_trivially_move_constructible

	: is_trivially_constructible<
	_Ty,
	typename add_rvalue_reference<_Ty>::type
	>::type

{	// determine whether _Ty has a trivial move constructor
};

// TEMPLATE CLASS has_trivial_move_constructor -- retained
template<class _Ty>
struct has_trivial_move_constructor
	: is_trivially_move_constructible<_Ty>::type
{	// determine whether _Ty has a trivial move constructor
};

// TEMPLATE CLASS is_trivially_assignable
template<class _To,
class _From>
struct is_trivially_assignable

	_IS_TRIVIALLY_ASSIGNABLE(_To, _From)

{	// determine whether _From can be assigned to _To, trivially
};

// TEMPLATE CLASS is_trivially_copy_assignable
template<class _Ty>
struct is_trivially_copy_assignable

	: is_trivially_assignable<
	typename add_lvalue_reference<_Ty>::type,
	typename add_lvalue_reference<
	typename add_const<_Ty>::type
	>::type
	>::type

{	// determine whether _Ty has a trivial copy assignment operator
};

// TEMPLATE CLASS has_trivial_copy_assign -- retained
template<class _Ty>
struct has_trivial_copy_assign
	: is_trivially_copy_assignable<_Ty>::type
{	// determine whether _Ty has a trivial copy assignment operator
};

// TEMPLATE CLASS is_trivially_move_assignable
template<class _Ty>
struct is_trivially_move_assignable

	: is_trivially_assignable<
	typename add_lvalue_reference<_Ty>::type,
	typename add_rvalue_reference<_Ty>::type
	>::type

{	// determine whether _Ty has a trivial move assignment operator
};

// TEMPLATE CLASS has_trivial_move_assign -- retained
template<class _Ty>
struct has_trivial_move_assign
	: is_trivially_move_assignable<_Ty>::type
{	// determine whether _Ty has a trivial move assignment operator
};

// TEMPLATE CLASS is_trivially_destructible
template<class _Ty>
struct is_trivially_destructible
	_HAS_TRIVIAL_DESTRUCTOR(_Ty)
{	// determine whether _Ty has a trivial destructor
};

// NOTHROW TRAITS
// TEMPLATE CLASS is_nothrow_constructible


template<class _Ty,
class... _Args>
struct is_nothrow_constructible
	: _Cat_base<_IS_NOTHROW_CONSTRUCTIBLE(_Ty, _Args...)>
{	// determine whether _Ty(_Args...) is nothrow constructible
};


// TEMPLATE CLASS is_nothrow_copy_constructible

template<class _Ty>
struct is_nothrow_copy_constructible
	: is_nothrow_constructible<
	_Ty,
	typename add_lvalue_reference<
	typename add_const<_Ty>::type
	>::type
	>::type
{	// determine whether _Ty has a nothrow copy constructor
};

// TEMPLATE CLASS has_nothrow_copy_constructor -- retained
template<class _Ty>
struct has_nothrow_copy_constructor

	: is_nothrow_copy_constructible<_Ty>::type

{	// determine whether _Ty has a nothrow copy constructor
};

// TEMPLATE CLASS is_nothrow_default_constructible
template<class _Ty>
struct is_nothrow_default_constructible
	: is_nothrow_constructible<_Ty>::type
{	// determine whether _Ty has a nothrow default constructor
};

// TEMPLATE CLASS has_nothrow_default_constructor -- retained
template<class _Ty>
struct has_nothrow_default_constructor
	: is_nothrow_default_constructible<_Ty>::type
{	// determine whether _Ty has a nothrow default constructor
};

// TEMPLATE CLASS is_nothrow_move_constructible

template<class _Ty>
struct is_nothrow_move_constructible
	: is_nothrow_constructible<
	_Ty,
	typename add_rvalue_reference<_Ty>::type
	>::type
{	// determine whether _Ty has a nothrow move constructor
};

// TEMPLATE CLASS has_nothrow_move_constructor -- retained
template<class _Ty>
struct has_nothrow_move_constructor
	: is_nothrow_move_constructible<_Ty>::type
{	// determine whether _Ty has a nothrow move constructor
};

// TEMPLATE CLASS is_nothrow_assignable
template<class _To,
class _From>
struct is_nothrow_assignable

	_IS_NOTHROW_ASSIGNABLE(_To, _From)

{	// determine whether _From can be assigned to _To, nothrow
};

// TEMPLATE CLASS is_nothrow_copy_assignable
template<class _Ty>
struct is_nothrow_copy_assignable

	: is_nothrow_assignable<
	typename add_lvalue_reference<_Ty>::type,
	typename add_lvalue_reference<
	typename add_const<_Ty>::type
	>::type
	>::type

{	// determine whether _Ty has a nothrow copy assignment operator
};

// TEMPLATE CLASS has_nothrow_copy_assign -- retained
template<class _Ty>
struct has_nothrow_copy_assign
	: is_nothrow_copy_assignable<_Ty>::type
{	// determine whether _Ty has a nothrow copy assignment operator
};

// TEMPLATE CLASS is_nothrow_move_assignable
template<class _Ty>
struct is_nothrow_move_assignable

	: is_nothrow_assignable<
	typename add_lvalue_reference<_Ty>::type,
	typename add_rvalue_reference<_Ty>::type
	>::type

{	// determine whether _Ty has a nothrow move assignment operator
};

// TEMPLATE CLASS has_nothrow_move_assign -- retained
template<class _Ty>
struct has_nothrow_move_assign
	: is_nothrow_move_assignable<_Ty>::type
{	// determine whether _Ty has a nothrow move assignment operator
};

// TEMPLATE CLASS is_nothrow_destructible
template<class _Ty>
struct is_nothrow_destructible

	_IS_NOTHROW_DESTRUCTIBLE(_Ty)

{	// determine whether _Ty has a nothrow destructor
};

// TEMPLATE CLASS has_trivial_constructor -- retained
template<class _Ty>
struct has_trivial_constructor

	: is_trivially_default_constructible<_Ty>::type

{	// determine whether _Ty has a trivial default constructor
};

// TEMPLATE CLASS has_trivial_copy -- retained
template<class _Ty>
struct has_trivial_copy

	: is_trivially_copy_constructible<_Ty>::type

{	// determine whether _Ty has a trivial copy constructor
};

// TEMPLATE CLASS has_trivial_assign -- retained
template<class _Ty>
struct has_trivial_assign

	: is_trivially_copy_assignable<_Ty>::type

{	// determine whether _Ty has a trivial assignment operator
};

// TEMPLATE CLASS has_trivial_destructor -- retained
template<class _Ty>
struct has_trivial_destructor

	: is_trivially_destructible<_Ty>::type

{	// determine whether _Ty has a trivial destructor
};

// TEMPLATE CLASS has_nothrow_constructor -- retained
template<class _Ty>
struct has_nothrow_constructor

	: is_nothrow_default_constructible<_Ty>::type

{	// determine whether _Ty has a nothrow default constructor
};

// TEMPLATE CLASS has_nothrow_copy -- retained
template<class _Ty>
struct has_nothrow_copy

	: is_nothrow_copy_constructible<_Ty>::type

{	// determine whether _Ty has a nothrow copy constructor
};

// TEMPLATE CLASS has_nothrow_assign -- retained
template<class _Ty>
struct has_nothrow_assign

	: is_nothrow_copy_assignable<_Ty>::type

{	// determine whether _Ty has a nothrow assignment operator
};

// TEMPLATE CLASS is_signed
template<class _Ty>
struct _Has_signed_vals
	: _Cat_base < (typename remove_cv<_Ty>::type)(-1)
	< (typename remove_cv<_Ty>::type)(0)>
{	// integral type can represent negative values
};

template<class _Ty>
struct is_signed
	: _Cat_base<is_floating_point<_Ty>::value || (is_integral<_Ty>::value
	&& _Has_signed_vals<
	typename _If<is_integral<_Ty>::value, _Ty, int>::type>::value)>
{	// determine whether _Ty is a signed type
};

// TEMPLATE CLASS is_unsigned
template<class _Ty>
struct is_unsigned
	: _Cat_base<is_integral<_Ty>::value
	&& !_Has_signed_vals<
	typename _If<is_integral<_Ty>::value, _Ty, int>::type>::value>
{	// determine whether _Ty is an unsigned type
};

// TEMPLATE CLASS _Change_sign
template<class _Ty>
struct _Change_sign
{	// signed/unsigned partners to _Ty
	static_assert(
		((is_integral<_Ty>::value || is_enum<_Ty>::value)
		&& !is_same<_Ty, bool>::value),
		"make_signed<T>/make_unsigned<T> require that T shall be a (possibly "
		"cv-qualified) integral type or enumeration but not a bool type.");

	typedef
		typename _If < is_same<_Ty, signed char>::value
		|| is_same<_Ty, unsigned char     >::value, signed char,
		typename _If < is_same<_Ty, short       >::value
		|| is_same<_Ty, unsigned short    >::value, short,
		typename _If < is_same<_Ty, int         >::value
		|| is_same<_Ty, unsigned int      >::value, int,
		typename _If<is_same<_Ty, long        >::value
		|| is_same<_Ty, unsigned long     >::value, long,
		typename _If<is_same<_Ty, long long   >::value
		|| is_same<_Ty, unsigned long long>::value, long long,
		typename _If<sizeof (_Ty) == sizeof (signed char), signed char,
		typename _If<sizeof (_Ty) == sizeof (short), short,
		typename _If<sizeof (_Ty) == sizeof (int), int,
		typename _If<sizeof (_Ty) == sizeof (long), long,
		long long
		>::type>::type>::type>::type>::type>::type > ::type > ::type > ::type
		_Signed;

	typedef
		typename _If<is_same<_Signed, signed char>::value, unsigned char,
		typename _If<is_same<_Signed, short      >::value, unsigned short,
		typename _If<is_same<_Signed, int        >::value, unsigned int,
		typename _If<is_same<_Signed, long       >::value, unsigned long,
		unsigned long long
		>::type>::type>::type>::type
		_Unsigned;
};

template<class _Ty>
struct _Change_sign<const _Ty>
{	// signed/unsigned partners to _Ty
	typedef const typename _Change_sign<_Ty>::_Signed _Signed;
	typedef const typename _Change_sign<_Ty>::_Unsigned _Unsigned;
};

template<class _Ty>
struct _Change_sign<volatile _Ty>
{	// signed/unsigned partners to _Ty
	typedef volatile typename _Change_sign<_Ty>::_Signed _Signed;
	typedef volatile typename _Change_sign<_Ty>::_Unsigned _Unsigned;
};

template<class _Ty>
struct _Change_sign<const volatile _Ty>
{	// signed/unsigned partners to _Ty
	typedef const volatile typename _Change_sign<_Ty>::_Signed _Signed;
	typedef const volatile typename _Change_sign<_Ty>::_Unsigned _Unsigned;
};

// TEMPLATE CLASS make_signed
template<class _Ty>
struct make_signed
{	// signed partner to _Ty
	typedef typename _Change_sign<_Ty>::_Signed type;
};

// TEMPLATE CLASS make_unsigned
template<class _Ty>
struct make_unsigned
{	// unsigned partner to _Ty
	typedef typename _Change_sign<_Ty>::_Unsigned type;
};

// TEMPLATE CLASS alignment_of
#ifndef _ALIGN_OF
template<class _Ty>
struct _Get_align
{	// struct used to determine alignment of _Ty
	_Ty _Elt0;
	char _Elt1;
	_Ty _Elt2;

	_Get_align();	// to quiet diagnostics
	~_Get_align();	// to quiet diagnostics
};

#define _ALIGN_OF(_Ty) (sizeof (_Get_align<_Ty>) - 2 * sizeof (_Ty))
#endif /* _ALIGN_OF */

template<class _Ty>
struct alignment_of
	: integral_constant<size_t, _ALIGN_OF(typename remove_reference<_Ty>::type)>
{	// determine alignment of _Ty
};

// TEMPLATE CLASS aligned_storage
#define _FITS(_Ty)	_Align <= alignment_of<_Ty>::value
#define _NEXT_ALIGN(_Ty)	\
	typedef typename _Aligned<_Len, _Align, _Ty, _FITS(_Ty)>::type type

template<class _Ty,
	size_t _Len>
union _Align_type
{	// union with size _Len bytes and alignment of _Ty
	_Ty _Val;
	char _Pad[_Len];
};

template<size_t _Len,
	size_t _Align,
class _Ty,
	bool _Ok>
struct _Aligned;

template<size_t _Len,
	size_t _Align,
class _Ty>
struct _Aligned<_Len, _Align, _Ty, true>
{	// define type with size _Len and alignment _Ty
	typedef _Align_type<_Ty, _Len> type;
};

template<size_t _Len,
	size_t _Align>
struct _Aligned<_Len, _Align, double, false>
{	// define type with size _Len and alignment _Ty
	typedef _Align_type<max_align_t, _Len> type;
};

template<size_t _Len,
	size_t _Align>
struct _Aligned<_Len, _Align, int, false>
{	// define type with size _Len and alignment _Ty
	_NEXT_ALIGN(double);
};

template<size_t _Len,
	size_t _Align>
struct _Aligned<_Len, _Align, short, false>
{	// define type with size _Len and alignment _Ty
	_NEXT_ALIGN(int);
};

template<size_t _Len,
	size_t _Align>
struct _Aligned<_Len, _Align, char, false>
{	// define type with size _Len and alignment _Ty
	_NEXT_ALIGN(short);
};

template<size_t _Len,
	size_t _Align = alignment_of<max_align_t>::value>
struct aligned_storage
{	// define type with size _Len and alignment _Align
	typedef typename _Aligned<_Len, _Align, char, _FITS(char)>::type type;
};

#undef _FITS
#undef _NEXT_ALIGN

// TEMPLATE CLASS aligned_union
template<size_t... _Vals>
struct _Maximum;

template<>
struct _Maximum<>
{	// maximum of nothing is 0
	static const size_t value = 0;
};

template<size_t _Val>
struct _Maximum<_Val>
{	// maximum of _Val is _Val
	static const size_t value = _Val;
};

template<size_t _First,
	size_t _Second,
	size_t... _Rest>
struct _Maximum<_First, _Second, _Rest...>
	: _Maximum < (_First < _Second ? _Second : _First), _Rest...>
{	// find maximum value in _First, _Second, _Rest...
};

template<size_t _Len,
class... _Types>
struct aligned_union
{	// define type with size at least _Len, for storing anything in _Types
	static const size_t _Max_len = _Maximum<
		_Len, sizeof(_Types)...>::value;	// NOT sizeof...(_Types)
	static const size_t alignment_value = _Maximum<
		alignment_of<_Types>::value...>::value;
	typedef typename aligned_storage<_Max_len, alignment_value>::type type;
};

// TEMPLATE CLASS underlying_type
template<class _Ty>
struct underlying_type
{	// determine type underlying type for enum
	typedef _UNDERLYING_TYPE(_Ty) type;
};

// TEMPLATE CLASS rank
template<class _Ty>
struct rank
	: integral_constant<size_t, 0>
{	// determine number of dimensions of array _Ty
};

template<class _Ty, unsigned int _Ix>
struct rank<_Ty[_Ix]>
	: integral_constant<size_t, rank<_Ty>::value + 1>
{	// determine number of dimensions of array _Ty
};

template<class _Ty>
struct rank<_Ty[]>
	: integral_constant<size_t, rank<_Ty>::value + 1>
{	// determine number of dimensions of array _Ty
};

// TEMPLATE CLASS extent
template<class _Ty, unsigned int _Nx>
struct _Extent
	: integral_constant<size_t, 0>
{	// determine extent of dimension _Nx of array _Ty
};

template<class _Ty, unsigned int _Ix>
struct _Extent<_Ty[_Ix], 0>
	: integral_constant<size_t, _Ix>
{	// determine extent of dimension _Nx of array _Ty
};

template<class _Ty, unsigned int _Nx, unsigned int _Ix>
struct _Extent<_Ty[_Ix], _Nx>
	: _Extent<_Ty, _Nx - 1>
{	// determine extent of dimension _Nx of array _Ty
};

template<class _Ty, unsigned int _Nx>
struct _Extent<_Ty[], _Nx>
	: _Extent<_Ty, _Nx - 1>
{	// determine extent of dimension _Nx of array _Ty
};

template<class _Ty, unsigned int _Nx = 0>
struct extent
	: _Extent<_Ty, _Nx>
{	// determine extent of dimension _Nx of array _Ty
};

// TEMPLATE CLASS is_base_of
template<class _Base, class _Der>
struct is_base_of _IS_BASE_OF(_Base, _Der)
{	// determine whether _Base is a base of or the same as _Der
};

// TEMPLATE CLASS decay
template<class _Ty>
struct decay
{	// determines decayed version of _Ty
	typedef typename remove_reference<_Ty>::type _Ty1;

	typedef typename _If<is_array<_Ty1>::value,
		typename remove_extent<_Ty1>::type *,
		typename _If<is_function<_Ty1>::value,
		typename add_pointer<_Ty1>::type,
		typename remove_cv<_Ty1>::type>::type>::type type;
};


// TEMPLATE STRUCT common_type
template<class... _Ty>
struct common_type;

template<class _Ty>
struct common_type<_Ty>
{	// type is typename decay<_Ty>::type for one argument
	typedef typename decay<_Ty>::type type;
};

template<class _Ty0,
class _Ty1>
struct common_type<_Ty0, _Ty1>
{	// type is common type of _Ty0 and _Ty1 for two arguments
	typedef typename decay<
		decltype(_Always_false<_Ty0>::value
		? _STD declval<_Ty0>()
		: _STD declval<_Ty1>())
	>::type type;
};

template<class _Ty0,
class _Ty1,
class... _Ty>
struct common_type<_Ty0, _Ty1, _Ty...>
{	// type is defined recursively for more than two arguments
	typedef typename common_type<
		typename common_type<_Ty0, _Ty1>::type, _Ty...
	>::type type;
};

// TEMPLATE STRUCT _Arg_idx
template<size_t... _Indexes>
struct _Arg_idx
{	// _Arg_idx<0, 1, 2...> for converting tuple to argument list
};

// CLASS _Unrefwrap
template<class _Ty>
class reference_wrapper;

template<class _Ty>
struct _Unrefwrap_helper
{	// leave unchanged if not a reference_wrapper
	typedef _Ty type;
};

template<class _Ty>
struct _Unrefwrap_helper<reference_wrapper<_Ty> >
{	// make a reference from a reference_wrapper
	typedef _Ty& type;
};

template<class _Ty>
struct _Unrefwrap
{	// decay, then unwrap a reference_wrapper
	typedef typename decay<_Ty>::type _Ty1;
	typedef typename _Unrefwrap_helper<_Ty1>::type type;
};

// TEMPLATE CLASS identity
template<class _Ty>
struct identity
{	// map _Ty to type unchanged
	typedef _Ty type;

	const _Ty& operator()(const _Ty& _Left) const
	{	// apply identity operator to operand
		return (_Left);
	}
};

// TEMPLATE FUNCTION forward
template<class _Ty> inline
_Ty&& forward(typename remove_reference<_Ty>::type& _Arg)
{	// forward an lvalue
	return (static_cast<_Ty&&>(_Arg));
}

template<class _Ty> inline
_Ty&& forward(typename remove_reference<_Ty>::type&& _Arg) _NOEXCEPT
{	// forward anything
	static_assert(!is_lvalue_reference<_Ty>::value, "bad forward call");
	return (static_cast<_Ty&&>(_Arg));
}

// TEMPLATE FUNCTION move
template<class _Ty> inline
typename remove_reference<_Ty>::type&&
move(_Ty&& _Arg) _NOEXCEPT
{	// forward _Arg as movable
	return ((typename remove_reference<_Ty>::type&&)_Arg);
}

// TEMPLATE FUNCTION _Move
template<class _Ty> inline
typename remove_reference<_Ty>::type&&
_Move(_Ty&& _Arg) _NOEXCEPT
{	// forward _Arg as movable
	return ((typename remove_reference<_Ty>::type&&)_Arg);
}

// TEMPLATE FUNCTION move_if_noexcept
template<class _Ty> inline
typename _If<!has_nothrow_move_constructor<_Ty>::value
&& has_copy_constructor<_Ty>::value,
const _Ty&, _Ty&&>::type
move_if_noexcept(_Ty& _Arg) _NOEXCEPT
{	// forward _Arg if moveable
	return (_STD move(_Arg));
}

// TEMPLATE FUNCTION _Decay_copy
template<class _Ty> inline
typename decay<_Ty>::type _Decay_copy(_Ty&& _Arg)
{	// forward _Arg as value of decayed type
	return (_STD forward<_Ty>(_Arg));
}

//////////////////////////////////////////////////////////////////////////
//xrefwrap
//////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 4180)

template<class _Ty>
class reference_wrapper;

// TEMPLATE CLASS _Fun_class_base
template<class _Ret,
class... _Types>
struct _Fun_class_base
{	// base if no arguments
};

template<class _Ret,
class _Farg0>
struct _Fun_class_base<_Ret, _Farg0>
	: public unary_function<_Farg0, _Ret>
{	// base if one argument
};

template<class _Ret,
class _Farg0,
class _Farg1>
struct _Fun_class_base<_Ret, _Farg0, _Farg1>
	: public binary_function<_Farg0, _Farg1, _Ret>
{	// base if two arguments
};

template<class _Ret,
class _Farg0,
class _Farg1,
class... _Types>
struct _Fun_class_base<_Ret, _Farg0, _Farg1, _Types...>
{	// base if more than two arguments
};


// IMPLEMENT result_of

template<class _Fty,
class... _Args>
struct _Result_of
{	// template to determine result of call operation
	typedef decltype(
		_STD declval<_Fty>()(_STD declval<_Args>()...)) type;
};

#define _RESULT_OF_PMF(CALL_OPT, CONST_OPT, CV_OPT) \
	template<class _Ret, \
class _Ty, \
class... _Mfargs, \
class _Obj, \
class... _Args> \
struct _Result_of<_Ret (CALL_OPT _Ty::* CONST_OPT)(_Mfargs...) CV_OPT, \
	_Obj, _Args...> \
{	/* template to determine result of call operation */ \
	/* on pointer to member function */ \
	typedef _Ret type; \
};

_MEMBER_CALL_CONST_CV(_RESULT_OF_PMF)
#undef _RESULT_OF_PMF

template<class _Ret,
class _Ty,
class _Obj,
class... _Args>
struct _Result_of<_Ret _Ty::*, _Obj, _Args...>
{	// template to determine result of call operation
	// on pointer to member data
	typedef typename _Copy_cv<_Ret,
		typename remove_pointer<
		typename remove_reference<_Obj>::type>::type>::type type;
};

template<class _Ret,
class _Ty,
class _Obj,
class... _Args>
struct _Result_of<_Ret _Ty::*const, _Obj, _Args...>
{	// template to determine result of call operation
	// on pointer to member data
	typedef typename _Copy_cv<_Ret,
		typename remove_pointer<
		typename remove_reference<_Obj>::type>::type>::type const type;
};

template<class _Fty>
struct result_of;	// not defined

#define _RESULT_OF(CALL_OPT, X1) \
	template<class _Fty, \
class... _Args> \
struct result_of<_Fty CALL_OPT (_Args...)> \
{	/* template to determine result of call operation */ \
	typedef typename _Result_of<_Fty, _Args...>::type type; \
}; \
	template<class _Fty, \
class... _Args> \
struct result_of<reference_wrapper<_Fty> CALL_OPT (_Args...)> \
{	/* template to determine result of call operation */ \
	typedef typename _Result_of<_Fty, _Args...>::type type; \
};

_NON_MEMBER_CALL(_RESULT_OF, )
#undef _RESULT_OF


// SUPPORT CLASSES FOR CALL WRAPPERS

// TEMPLATE STRUCT _Pmd_caller
template<class _Ret,
class _Arg0>
struct _Pmd_caller
{	// bind object and pointer to member data
	template<class _Pmd,
	class _Farg0>
		static _Ret& _Call_pmd(_Pmd _Pm, _Farg0&& _Fx0, true_type)
	{	// apply to object
			return ((_Ret&)(_Fx0.*_Pm));
	}

	template<class _Pmd,
	class _Farg0>
		static _Ret& _Call_pmd(_Pmd _Pm, _Farg0&& _Fx0, false_type)
	{	// apply to (possibly smart) pointer
			return ((_Ret&)((*_Fx0).*_Pm));
	}

	template<class _Pmd,
	class _Farg0>
		static _Ret& _Apply_pmd(_Pmd _Pm, _Farg0&& _Fx0)
	{	// apply to object
			typedef typename remove_cv<
				typename remove_reference<_Arg0>::type>::type _Arg0_bare;
			typedef typename remove_cv<
				typename remove_reference<_Farg0>::type>::type _Farg0_bare;
			typedef _Cat_base<is_same<_Arg0_bare, _Farg0_bare>::value
				|| (is_base_of<_Arg0_bare, _Farg0_bare>::value
				&& is_same<typename add_reference<_Farg0_bare>::type,
				_Farg0>::value)> _Is_obj;

			return (_Call_pmd<_Pmd, _Farg0>(_Pm,
				_STD forward<_Farg0>(_Fx0), _Is_obj()));
	}
};

// TEMPLATE STRUCT _Callable_base
template<class _Ty,
	bool _Indirect>
struct _Callable_base;

template<class _Ty>
struct _Callable_base<_Ty, false>
{	// base types for callable object wrappers
	enum { _EEN_INDIRECT = 0 };	// helper for expression evaluator
	typedef _Ty _MyTy;
	typedef const _Ty& _MyCnstTy;

	_Callable_base(const _Ty& _Val)
		: _Object(_Val)
	{	// construct from value
	}

	const _Ty& _Get() const
	{	// return reference to stored object
		return (_Object);
	}

	_Ty& _Get()
	{	// return reference to stored object
		return (_Object);
	}

	void _Reset(_Ty& _Val)
	{	// reseat reference (disallowed)
		static_assert(_Always_false<_Ty>::value,
			"can't assign to reference_wrapper<T&>");
	}

private:
	_Callable_base& operator=(const _Callable_base&);

	_Ty _Object;
};

template<class _Ty>
struct _Callable_base<_Ty, true>
{	// base types for callable object wrappers holding references
	// (used by reference_wrapper)
	enum { _EEN_INDIRECT = 1 };	// helper for expression evaluator
	typedef _Ty _MyTy;
	typedef _Ty& _MyCnstTy;

	_Callable_base(_Ty& _Val)
		: _Ptr(_STD addressof(_Val))
	{	// construct
	}

	_MyCnstTy _Get() const
	{	// return reference to stored object
		return (*_Ptr);
	}

	_Ty& _Get()
	{	// return reference to stored object
		return (*_Ptr);
	}

	void _Reset(_Ty& _Val)
	{	// reseat reference
		_Ptr = _STD addressof(_Val);
	}

private:
	_Ty *_Ptr;
};

// TEMPLATE STRUCT _Callable_pmd
template<class _Ty,
class _Memty,
	bool _Indirect = false>
struct _Callable_pmd
	: _Callable_base<_Ty, _Indirect>
{	// wrap pointer to member data
	_Callable_pmd(const _Callable_pmd& _Right)
		: _Callable_base<_Ty, _Indirect>(_Right._Get())
	{	// construct
	}

	_Callable_pmd(_Ty& _Val)
		: _Callable_base<_Ty, _Indirect>(_Val)
	{	// construct
	}

	template<class _Ret,
	class _Arg0>
		_Ret& _ApplyX(_Arg0&& _A0) const
	{	// apply
			return (_Pmd_caller<_Ret, _Memty>::
				_Apply_pmd(this->_Get(), _STD forward<_Arg0>(_A0)));
	}
};

// TEMPLATE STRUCT _Callable_obj
template<class _Ty,
	bool _Indirect = false>
struct _Callable_obj
	: _Callable_base<_Ty, _Indirect>
{	// wrap function object
	typedef _Callable_base<_Ty, _Indirect> _Mybase;

	template<class _Ty2>
	_Callable_obj(_Ty2&& _Val)
		: _Mybase(_STD forward<_Ty2>(_Val))
	{	// construct
	}

	template<class _Ret,
	class... _Types>
		_Ret _ApplyX(_Types&&... _Args) const
	{ // apply to UDT object
			return (this->_Get()(_STD forward<_Types>(_Args)...));
	}

	template<class _Ret,
	class... _Types>
		_Ret _ApplyX(_Types&&... _Args)
	{ // apply to UDT object
			return (this->_Get()(_STD forward<_Types>(_Args)...));
	}

};

// TEMPLATE STRUCT _Pmf_caller

template<class _Ret,
class _Arg0>
struct _Pmf_caller
{	// bind object and pointer to member function
	template<class _Pmf,
	class _Farg0,
	class... _Ftypes>
		static _Ret _Call_pmf(_Pmf _Pm, _Farg0&& _Fx0, true_type,
		_Ftypes&&... _Fargs)
	{	// apply to object
			typedef typename _Copy_cv<_Arg0, _Farg0>::type
				_Funobj_cv;
			return (((_Funobj_cv)_Fx0.*_Pm)(_STD forward<_Ftypes>(_Fargs)...));
	}

	template<class _Pmf,
	class _Farg0,
	class... _Ftypes>
		static _Ret _Call_pmf(_Pmf _Pm, _Farg0&& _Fx0, false_type,
		_Ftypes&&... _Fargs)
	{	// apply to (possibly smart) pointer
			return (((*_Fx0).*_Pm)(_STD forward<_Ftypes>(_Fargs)...));
	}

	template<class _Pmf,
	class _Farg0,
	class... _Ftypes>
		static _Ret _Apply_pmf(_Pmf _Pm, _Farg0&& _Fx0,
		_Ftypes&&... _Fargs)
	{	// apply to object
			typedef typename remove_reference<_Arg0>::type _Arg0_bare0;
			typedef typename remove_cv<_Arg0_bare0>::type _Arg0_bare;
			typedef typename remove_reference<_Farg0>::type _Farg0_bare;
			typedef _Cat_base<is_same<_Arg0_bare, _Farg0_bare>::value
				|| (is_base_of<_Arg0_bare, _Farg0_bare>::value
				&& is_same<typename add_reference<_Farg0_bare>::type,
				_Farg0>::value)> _Is_obj;

			return (_Call_pmf<_Pmf, _Farg0&&, _Ftypes&&...>(_Pm,
				_STD forward<_Farg0>(_Fx0), _Is_obj(),
				_STD forward<_Ftypes>(_Fargs)...));
	}
};


// TEMPLATE STRUCT _Callable_pmf
template<class _Ty,
class _Memty,
	bool _Indirect = false>
struct _Callable_pmf
	: _Callable_base<_Ty, _Indirect>
{	// wrap pointer to member function
	_Callable_pmf(const _Callable_pmf& _Right)
		: _Callable_base<_Ty, _Indirect>(_Right._Get())
	{	// construct
	}

	_Callable_pmf(_Ty& _Val)
		: _Callable_base<_Ty, _Indirect>(_Val)
	{	// construct
	}

	template<class _Ret,
	class... _Types>
		_Ret _ApplyX(_Types&&... _Args) const
	{ // call pointer to member function
			return (_Pmf_caller<_Ret, _Memty>::
				_Apply_pmf(this->_Get(), _STD forward<_Types>(_Args)...));
	}

};

// TEMPLATE STRUCT _Callable_fun
template<class _Ty,
	bool _Indirect = false>
struct _Callable_fun
	: _Callable_base<_Ty, _Indirect>
{	// wrap pointer to function
	_Callable_fun(const _Callable_fun& _Right)
		: _Callable_base<_Ty, _Indirect>(_Right._Get())
	{	// construct
	}

	_Callable_fun(_Ty& _Val)
		: _Callable_base<_Ty, _Indirect>(_Val)
	{	// construct
	}

	template<class _Ret,
	class... _Types>
		_Ret _ApplyX(_Types&&... _Args) const
	{ // call pointer to function
			return (this->_Get()(_STD forward<_Types>(_Args)...));
	}

};

// TEMPLATE STRUCT _Call_wrapper_base
template<class _Callable>
struct _Call_wrapper_base
{	// wrap callable object
	typedef typename _Callable::_MyTy _MyTy;
	typedef typename _Callable::_MyCnstTy _MyCnstTy;

	_Call_wrapper_base(_MyTy& _Val)
		: _Callee(_Val)
	{	// construct
	}

	void _Reset(_MyTy& _Val)
	{	// reset
		_Callee._Reset(_Val);
	}

	_MyCnstTy _Get() const
	{	// get
		return (_Callee._Get());
	}

	_MyCnstTy _Get()
	{	// get
		return (_Callee._Get());
	}

	_Callable _Callee;
};

// TEMPLATE STRUCT _Call_wrapper
template<class _Callable,
	bool _Is_abstract = false>
struct _Call_wrapper
	: _Call_wrapper_base<_Callable>
{	// wrap callable object
	typedef _Call_wrapper_base<_Callable> _Mybase;

	_Call_wrapper(typename _Call_wrapper_base<_Callable>::_MyTy& _Val)
		: _Call_wrapper_base<_Callable>(_Val)
	{	// construct
	}

	template<class... _Types>
	typename result_of<
		typename _Callable::_MyTy(_Types...)>::type
		operator()(_Types&&... _Args) const
	{ // call target object
			typedef typename result_of<
				typename _Callable::_MyTy(_Types...)>::type _Ret;
			return (this->_Callee.template _ApplyX<_Ret>(
				_STD forward<_Types>(_Args)...));
	}

};

template<class _Callable>
struct _Call_wrapper<_Callable, true>
	: _Call_wrapper_base<_Callable>
{	// wrap abstract callable object
	typedef _Call_wrapper_base<_Callable> _Mybase;

	_Call_wrapper(typename _Call_wrapper_base<_Callable>::_MyTy& _Val)
		: _Call_wrapper_base<_Callable>(_Val)
	{	// construct
	}
};

// TEMPLATE STRUCT _Has_result_and_arg_type
template<class _Ty>
struct _Has_result_and_arg_type
	_HAS_TYPES(argument_type, result_type, result_type);

// TEMPLATE STRUCT _Has_result_and_2arg_type
template<class _Ty>
struct _Has_result_and_2arg_type
	_HAS_TYPES(first_argument_type, second_argument_type, result_type);

// TEMPLATE CLASS _Refwrap_result0
template<class _Ty,
	bool>
struct _Refwrap_result0
{	// define result_type when target object defines it
	typedef typename _Ty::result_type result_type;
};

template<class _Ty>
struct _Refwrap_result0<_Ty, false>
{	// no result_type when not defined by target object
};

// TEMPLATE CLASS _Refwrap_result1_helper
template<class _Ty,
	bool>
struct _Refwrap_result1_helper
	: _Refwrap_result0<_Ty, _Has_result_type<_Ty>::type::value>
{	// select _Refwrap_result0 specialization
};

template<class _Ty>
struct _Refwrap_result1_helper<_Ty, true>
	: unary_function<typename _Ty::argument_type,
	typename _Ty::result_type>
{	// derive from unary_function
};

// TEMPLATE CLASS _Refwrap_result1
template<class _Ty,
	bool>
struct _Refwrap_result1
	: _Refwrap_result0<_Ty, _Has_result_type<_Ty>::type::value>
{	// select base for type without typedefs for result and one argument
};

template<class _Ty>
struct _Refwrap_result1<_Ty, true>
	: _Refwrap_result1_helper<_Ty,
	is_base_of<unary_function<
	typename _Ty::argument_type,
	typename _Ty::result_type>, _Ty>::value>
{	// select base for type with typedefs for result and one argument
};

// TEMPLATE CLASS _Refwrap_result2_helper
template<class _Ty,
	bool>
struct _Refwrap_result2_helper
	: _Refwrap_result1<_Ty, _Has_result_and_arg_type<_Ty>::type::value>
{	// select base
};

template<class _Ty>
struct _Refwrap_result2_helper<_Ty, true>
	: binary_function<typename _Ty::first_argument_type,
	typename _Ty::second_argument_type,
	typename _Ty::result_type>,
	_Refwrap_result1<_Ty, _Has_result_and_arg_type<_Ty>::type::value>
{	// base for type derived from binary_function
};

// TEMPLATE CLASS _Refwrap_result2
template<class _Ty,
	bool>
struct _Refwrap_result2
	: _Refwrap_result1<_Ty, _Has_result_and_arg_type<_Ty>::type::value>
{	// select base for type without typedefs for result and two arguments
};

template<class _Ty>
struct _Refwrap_result2<_Ty, true>
	: _Refwrap_result2_helper<_Ty,
	is_base_of<binary_function<
	typename _Ty::first_argument_type,
	typename _Ty::second_argument_type,
	typename _Ty::result_type>, _Ty>::value>
{	// select base for type with typedefs for result and two arguments
};

// TEMPLATE CLASS _Refwrap_impl
template<class _Ty>
struct _Refwrap_impl
	: _Call_wrapper<_Callable_obj<_Ty, true>,
	is_abstract<_Ty>::value>,
	_Refwrap_result2<_Ty, _Has_result_and_2arg_type<_Ty>::type::value>
{	// reference_wrapper implementation for UDT
	_Refwrap_impl(_Ty& _Val)
		: _Call_wrapper<_Callable_obj<_Ty, true>,
		is_abstract<_Ty>::value>(_Val)
	{	// construct
	}
};

template<class _Rx,
class _Arg0>
struct _Refwrap_impl<_Rx _Arg0::*>
	: _Call_wrapper<_Callable_pmd<_Rx _Arg0::*, _Arg0, false> >
{	// reference_wrapper implementation for pointer to member data
	typedef _Rx _Arg0::* _Fty;
	typedef _Rx result_type;

	_Refwrap_impl(_Fty& _Val)
		: _Call_wrapper<_Callable_pmd<_Fty, _Arg0, false> >(_Val)
	{	// construct
	}
};

template<class _Rx,
class _Arg0>
struct _Refwrap_impl<_Rx _Arg0::*const>
	: _Call_wrapper<_Callable_pmd<_Rx _Arg0::*const, _Arg0, false> >
{	// reference_wrapper implementation for const pointer to member data
	typedef _Rx _Arg0::*const _Fty;
	typedef _Rx result_type;
	_Refwrap_impl(_Fty& _Val)
		: _Call_wrapper<_Callable_pmd<_Fty, _Arg0, false> >(_Val)
	{	// construct
	}
};

// TEMPLATE CLASS _Refwrap_impl FOR FUNCTIONS
#define _REFWRAP_IMPL_FUN(CALL_OPT, X1) \
	template<class _Rx, \
class... _Types> \
struct _Refwrap_impl<_Rx CALL_OPT (_Types...)> \
	: _Call_wrapper<_Callable_fun< \
	_Rx(CALL_OPT *)(_Types...), false> >, \
	_Fun_class_base<_Rx, _Types...> \
{	/* implement for function */ \
	typedef _Rx(CALL_OPT *_Fty)(_Types...); \
	typedef _Rx result_type; \
	_Refwrap_impl(_Fty _Val) \
	: _Call_wrapper<_Callable_fun<_Fty, false> >(_Val) \
{	/* construct */ \
} \
};

_NON_MEMBER_CALL(_REFWRAP_IMPL_FUN, )
#undef _REFWRAP_IMPL_FUN

// TEMPLATE CLASS _Refwrap_impl FOR POINTERS TO FUNCTIONS
#define _REFWRAP_IMPL_PF(CALL_OPT, CONST_OPT) \
	template<class _Rx, \
class... _Types> \
struct _Refwrap_impl<_Rx(CALL_OPT *CONST_OPT)(_Types...)> \
	: _Call_wrapper<_Callable_fun< \
	_Rx(CALL_OPT *CONST_OPT)(_Types...), true> >, \
	_Fun_class_base<_Rx, _Types...> \
{	/* implement for CONST_OPT pointer to function */ \
	typedef _Rx(CALL_OPT *CONST_OPT _Fty)(_Types...); \
	typedef _Rx result_type; \
	_Refwrap_impl(_Fty& _Val) \
	: _Call_wrapper<_Callable_fun<_Fty, true> >(_Val) \
{	/* construct */ \
} \
};

_NON_MEMBER_CALL_CONST(_REFWRAP_IMPL_PF)
#undef _REFWRAP_IMPL_PF

// TEMPLATE CLASS _Refwrap_impl FOR POINTERS TO MEMBER FUNCTIONS
#define _REFWRAP_IMPL_PMF(CALL_OPT, CONST_OPT, CV_OPT) \
	template<class _Rx, \
class _Arg0, \
class... _Types> \
struct _Refwrap_impl<_Rx(CALL_OPT _Arg0::* CONST_OPT)(_Types...) CV_OPT> \
	: _Call_wrapper<_Callable_pmf< \
	_Rx(CALL_OPT _Arg0::* CONST_OPT)(_Types...) CV_OPT, \
	_Arg0, true> >, \
	_Fun_class_base<_Rx, CV_OPT _Arg0 *, _Types...> \
{	/* implement for pointer to member function */ \
	typedef _Rx(CALL_OPT _Arg0::* CONST_OPT _Fty)(_Types...) CV_OPT; \
	typedef _Rx result_type; \
	_Refwrap_impl(_Fty& _Val) \
	: _Call_wrapper<_Callable_pmf<_Fty, _Arg0, true> >(_Val) \
{	/* construct */ \
} \
};

_MEMBER_CALL_CONST_CV(_REFWRAP_IMPL_PMF)
#undef _REFWRAP_IMPL_PMF


// TEMPLATE CLASS reference_wrapper
template<class _Ty>
class reference_wrapper
	: public _Refwrap_impl<_Ty>
{	// stand-in for an assignable reference
public:
	typedef reference_wrapper<_Ty> _Myt;
	typedef _Refwrap_impl<_Ty> _Mybase;
	typedef _Ty type;

	reference_wrapper(_Ty& _Val) _NOEXCEPT
		: _Mybase(_Val)
	{	// construct
	}

	reference_wrapper(const _Myt& _Right) _NOEXCEPT
		: _Mybase(_Right.get())
	{	// construct by copying _Right
	}

	_Myt& operator=(const _Myt& _Right) _NOEXCEPT
	{	// assign _Right
		this->_Reset(_Right.get());
		return (*this);
	}

		operator _Ty&() const _NOEXCEPT
	{	// return reference
		return (this->_Get());
	}

		_Ty& get() const _NOEXCEPT
	{	// return reference
		return (this->_Get());
	}

	reference_wrapper(_Ty&&) = delete;
};

// TEMPLATE FUNCTIONS ref AND cref
template<class _Ty>
reference_wrapper<_Ty>
ref(_Ty& _Val) _NOEXCEPT
{	// create reference_wrapper<_Ty> object
	return (reference_wrapper<_Ty>(_Val));
}

template<class _Ty>
void ref(const _Ty&&) = delete;

template<class _Ty>
reference_wrapper<_Ty>
ref(reference_wrapper<_Ty> _Val) _NOEXCEPT
{	// create reference_wrapper<_Ty> object
	return (_Val);
}

template<class _Ty>
reference_wrapper<const _Ty>
cref(const _Ty& _Val) _NOEXCEPT
{	// create reference_wrapper<const _Ty> object
	return (reference_wrapper<const _Ty>(_Val));
}

template<class _Ty>
void cref(const _Ty&&) = delete;

template<class _Ty>
reference_wrapper<const _Ty>
cref(reference_wrapper<_Ty> _Val) _NOEXCEPT
{	// create reference_wrapper<const _Ty> object
	return (reference_wrapper<const _Ty>(_Val.get()));
}

// ALIAS TEMPLATES
template<class _Ty>
using remove_const_t = typename remove_const<_Ty>::type;

template<class _Ty>
using remove_volatile_t = typename remove_volatile<_Ty>::type;

template<class _Ty>
using remove_cv_t = typename remove_cv<_Ty>::type;

template<class _Ty>
using add_const_t = typename add_const<_Ty>::type;

template<class _Ty>
using add_volatile_t = typename add_volatile<_Ty>::type;

template<class _Ty>
using add_cv_t = typename add_cv<_Ty>::type;

template<class _Ty>
using remove_reference_t = typename remove_reference<_Ty>::type;

template<class _Ty>
using add_lvalue_reference_t = typename add_lvalue_reference<_Ty>::type;

template<class _Ty>
using add_rvalue_reference_t = typename add_rvalue_reference<_Ty>::type;

template<class _Ty>
using make_signed_t = typename make_signed<_Ty>::type;

template<class _Ty>
using make_unsigned_t = typename make_unsigned<_Ty>::type;

template<class _Ty>
using remove_extent_t = typename remove_extent<_Ty>::type;

template<class _Ty>
using remove_all_extents_t = typename remove_all_extents<_Ty>::type;

template<class _Ty>
using remove_pointer_t = typename remove_pointer<_Ty>::type;

template<class _Ty>
using add_pointer_t = typename add_pointer<_Ty>::type;

template<size_t _Len,
	size_t _Align = alignment_of<max_align_t>::value>
	using aligned_storage_t = typename aligned_storage<_Len, _Align>::type;

template<size_t _Len,
class... _Types>
	using aligned_union_t = typename aligned_union<_Len, _Types...>::type;

template<class _Ty>
using decay_t = typename decay<_Ty>::type;

template<bool _Test,
class _Ty = void>
	using enable_if_t = typename enable_if<_Test, _Ty>::type;

template<bool _Test,
class _Ty1,
class _Ty2>
	using conditional_t = typename conditional<_Test, _Ty1, _Ty2>::type;

template<class... _Ty>
using common_type_t = typename common_type<_Ty...>::type;

template<class _Ty>
using underlying_type_t = typename underlying_type<_Ty>::type;

template<class _Ty>
using result_of_t = typename result_of<_Ty>::type;

}//namespace skl{

#pragma pop_macro("new")
#pragma warning(pop)
#pragma pack(pop)