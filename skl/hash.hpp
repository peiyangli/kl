/*////////////////////////////////////////////////////////
(c)22016
filename: hash.hpp
author: pei
version: 0.0.1
date:
description: hash calculate, more hash to do
function/class list:
history:
////////////////////////////////////////////////////////*/
#pragma once

namespace skl {

	namespace hash {
		unsigned long long BKDRHash(const unsigned char *str, size_t len)
		{
			register unsigned long long hash = 0;
			for (size_t i = 0; i < len; ++i) //31 131 1313 13131 131313 etc...
				hash = hash * 131 + (unsigned long long)str[i];
			return hash;
		}
	}//namespace hash
}//namespace skl