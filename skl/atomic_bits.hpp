#pragma once

#include "atomic.hpp"

namespace skl{
	template<unsigned _Count, unsigned _Size>
	class atomic_bits{
		skl::atomic_uint64 _pidProtect[_Count];
		enum {
			one_count = sizeof(unsigned long long) / _Size,
			mem_size = sizeof(unsigned long long)
		};
	public:
		unsigned long long setValue(int index, unsigned long long value){
			//index < _Count/one_count
			auto pidindex = (index-1) / one_count;
			//pidindex < _Count
			auto pidbit = ((index - 1) % one_count) * _Size;
		}
		unsigned long long value(int index){
		}
	};
}
