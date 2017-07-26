
/*////////////////////////////////////////////////////////
(c)22016
filename: utility.h
author: pei/from vs
version: 0.0.1
date:
description: utility
function/class list:
history:
////////////////////////////////////////////////////////*/

#pragma once

#include "type_traits.hpp"

namespace skl{


#pragma warning(push)
#pragma warning(disable: 4244) // while (false);

#define _NOEXCEPT_OP(x)
	// TEMPLATE FUNCTION iter_swap (from <xutility>)
	template<class _Ty> inline
		void swap(_Ty&, _Ty&)
		_NOEXCEPT_OP(is_nothrow_move_constructible<_Ty>::value
		&& is_nothrow_move_assignable<_Ty>::value);

	template<class _FwdIt1,
	class _FwdIt2> inline
		void iter_swap(_FwdIt1 _Left, _FwdIt2 _Right)
	{	// swap *_Left and *_Right
		swap(*_Left, *_Right);
	}

	// TEMPLATE FUNCTION swap
	template<class _Ty,
		size_t _Size> inline
		void swap(_Ty(&_Left)[_Size], _Ty(&_Right)[_Size])
		_NOEXCEPT_OP(_NOEXCEPT_OP(swap(*_Left, *_Right)))
	{	// exchange arrays stored at _Left and _Right
		if (&_Left != &_Right)
		{	// worth swapping, swap ranges
			_Ty *_First1 = _Left;
			_Ty *_Last1 = _First1 + _Size;
			_Ty *_First2 = _Right;
			for (; _First1 != _Last1; ++_First1, ++_First2)
				_STD iter_swap(_First1, _First2);
		}
	}

	template<class _Ty> inline
		void swap(_Ty& _Left, _Ty& _Right)
		_NOEXCEPT_OP(is_nothrow_move_constructible<_Ty>::value
		&& is_nothrow_move_assignable<_Ty>::value)
	{	// exchange values stored at _Left and _Right
		_Ty _Tmp = _Move(_Left);
		_Left = _Move(_Right);
		_Right = _Move(_Tmp);
	}

	// TEMPLATE FUNCTION _Swap_adl
	template<class _Ty> inline
		void _Swap_adl(_Ty& _Left, _Ty& _Right)
	{	// exchange values stored at _Left and _Right, using ADL
		swap(_Left, _Right);
	}

	template<class _Ty1,
	class _Ty2>
	struct pair
	{	// store a pair of values
		typedef pair<_Ty1, _Ty2> _Myt;
		typedef _Ty1 first_type;
		typedef _Ty2 second_type;

		pair()
			: first(), second()
		{	// default construct
		}

		pair(const _Ty1& _Val1, const _Ty2& _Val2)
			: first(_Val1), second(_Val2)
		{	// construct from specified values
		}

		template<class _Other1,
		class _Other2,
		class = typename enable_if<is_convertible<const _Other1&, _Ty1>::value
			&& is_convertible<const _Other2&, _Ty2>::value,
			void>::type>
			pair(const pair<_Other1, _Other2>& _Right)
			: first(_Right.first), second(_Right.second)
		{	// construct from compatible pair
		}

		template<class _Other1,
		class _Other2>
			_Myt& operator=(const pair<_Other1, _Other2>& _Right)
		{	// assign from compatible pair
			first = _Right.first;
			second = _Right.second;
			return (*this);
		}

// 
// 		template<class _Tuple1,
// 		class _Tuple2,
// 			size_t... _Indexes1,
// 			size_t... _Indexes2> inline
// 			pair(_Tuple1& _Val1,
// 			_Tuple2& _Val2,
// 			_Arg_idx<_Indexes1...>,
// 			_Arg_idx<_Indexes2...>);
// 
// 		template<class... _Types1,
// 		class... _Types2> inline
// 			pair(piecewise_construct_t,
// 			tuple<_Types1...> _Val1,
// 			tuple<_Types2...> _Val2)
// 			_NOEXCEPT_OP((is_nothrow_constructible<_Ty1, _Types1&&...>::value
// 			&& is_nothrow_constructible<_Ty2, _Types2&&...>::value));


		template<class _Other1,
		class _Other2,
		class = typename enable_if<is_convertible<_Other1, _Ty1>::value
			&& is_convertible<_Other2, _Ty2>::value,
			void>::type>
			pair(_Other1&& _Val1, _Other2&& _Val2)
			_NOEXCEPT_OP((is_nothrow_constructible<_Ty1, _Other1&&>::value
			&& is_nothrow_constructible<_Ty2, _Other2&&>::value))
			: first(_STD forward<_Other1>(_Val1)),
			second(_STD forward<_Other2>(_Val2))
		{	// construct from moved values
		}

		template<class _Other1,
		class _Other2,
		class = typename enable_if<is_convertible<_Other1, _Ty1>::value
			&& is_convertible<_Other2, _Ty2>::value,
			void>::type>
			pair(pair<_Other1, _Other2>&& _Right)
			_NOEXCEPT_OP((is_nothrow_constructible<_Ty1, _Other1&&>::value
			&& is_nothrow_constructible<_Ty2, _Other2&&>::value))
			: first(_STD forward<_Other1>(_Right.first)),
			second(_STD forward<_Other2>(_Right.second))
		{	// construct from moved compatible pair
		}

		template<class _Other1,
		class _Other2>
			_Myt& operator=(pair<_Other1, _Other2>&& _Right)
			_NOEXCEPT_OP((is_nothrow_assignable<_Ty1, _Other1&&>::value
			&& is_nothrow_assignable<_Ty2, _Other2&&>::value))
		{	// assign from moved compatible pair
			first = _STD forward<_Other1>(_Right.first);
			second = _STD forward<_Other2>(_Right.second);
			return (*this);
		}

		_Myt& operator=(_Myt&& _Right)
			_NOEXCEPT_OP((is_nothrow_move_assignable<_Ty1>::value
			&& is_nothrow_move_assignable<_Ty2>::value))
		{	// assign from moved pair
			first = _STD forward<_Ty1>(_Right.first);
			second = _STD forward<_Ty2>(_Right.second);
			return (*this);
		}

		void swap(_Myt& _Right)
			_NOEXCEPT_OP(_NOEXCEPT_OP(_Swap_adl(this->first, _Right.first))
			&& _NOEXCEPT_OP(_Swap_adl(this->second, _Right.second)))
		{	// exchange contents with _Right
			if (this != &_Right)
			{	// different, worth swapping
				_Swap_adl(first, _Right.first);
				_Swap_adl(second, _Right.second);
			}
		}

		_Myt& operator=(const _Myt& _Right)
		{	// assign from copied pair
			first = _Right.first;
			second = _Right.second;
			return (*this);
		}

		_Ty1 first;	// the first stored value
		_Ty2 second;	// the second stored value
	};


#pragma warning(pop)
}//namespace skl