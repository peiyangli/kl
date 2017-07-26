#pragma once

namespace skl{
	template<class _Ty, unsigned _Size>
	class array
	{
		enum { _EEN_SIZE = _Size };	// helper for expression evaluator
		typedef array<_Ty, _Size> _Myt;
		typedef _Ty value_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef _Ty *pointer;
		typedef const _Ty *const_pointer;
		typedef _Ty& reference;
		typedef const _Ty& const_reference;

		_Ty _Elems[_Size == 0 ? 1 : _Size];
	public:

		_Ty *data() _NOEXCEPT
		{	// return pointer to mutable data array
			return (_Elems);
		}

		const _Ty *data() const _NOEXCEPT
		{	// return pointer to nonmutable data array
			return (_Elems);
		}
	};
}

//_Ty _Elems[_Size == 0 ? 1 : _Size];