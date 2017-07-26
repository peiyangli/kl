#pragma once

#include "type_traits.hpp"

#pragma pack(push,_CRT_PACKING)
#pragma warning(push,3)
#pragma push_macro("new")
#undef new

namespace std{

		// TEMPLATE CLASS initializer_list
	template<class _Elem>
	class initializer_list
	{	// list of pointers to elements
	public:
		typedef _Elem value_type;
		typedef const _Elem& reference;
		typedef const _Elem& const_reference;
		typedef size_t size_type;

		typedef const _Elem* iterator;
		typedef const _Elem* const_iterator;

		initializer_list() _NOEXCEPT
			: _First(0), _Last(0)
		{	// empty list
		}

		initializer_list(const _Elem *_First_arg,
			const _Elem *_Last_arg) _NOEXCEPT
			: _First(_First_arg), _Last(_Last_arg)
		{	// construct with pointers
		}

		const _Elem *begin() const _NOEXCEPT
		{	// get beginning of list
			return (_First);
		}

			const _Elem *end() const _NOEXCEPT
		{	// get end of list
			return (_Last);
		}

			size_t size() const _NOEXCEPT
		{	// get length of list
			return ((size_t)(_Last - _First));
		}

	private:
		const _Elem *_First;
		const _Elem *_Last;
	};

	// TEMPLATE FUNCTION begin
	template<class _Elem> inline
		const _Elem *begin(initializer_list<_Elem> _Ilist) _NOEXCEPT
	{	// get beginning of sequence
		return (_Ilist.begin());
	}

		// TEMPLATE FUNCTION end
		template<class _Elem> inline
		const _Elem *end(initializer_list<_Elem> _Ilist) _NOEXCEPT
	{	// get end of sequence
		return (_Ilist.end());
	}
}

#pragma pop_macro("new")
#pragma warning(pop)
#pragma pack(pop)