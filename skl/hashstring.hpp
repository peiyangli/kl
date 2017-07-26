/*////////////////////////////////////////////////////////
(c)22016
filename: hashstring.hpp
author: pei
version: 0.0.1
date:
description: string hash calculate, more hash to do
function/class list:
history:
////////////////////////////////////////////////////////*/

#pragma once

#include "kernel.h"

#include "util.hpp"

namespace skl {
	namespace runtime {
		unsigned long long BKDRHash(const UNICODE_STRING& str, bool lowercase)
		{
			register unsigned long long hash = 0;
			auto pbuf = (UCHAR*)str.Buffer;
			auto wlen = str.Length / 2;
			for (size_t i = 0; i < wlen; i++) {
				auto v = lowercase ? skl::tolower(str.Buffer[i]) : str.Buffer[i];
				hash = hash * 131 + (unsigned long long)v;
			}
			return hash;
		}

		unsigned long long BKDRHash(const wchar_t* str, size_t wlen, bool lowercase)
		{
			register unsigned long long hash = 0;
			auto pbuf = (UCHAR*)str;
			for (size_t i = 0; i < wlen; i++) {
				auto v = lowercase?skl::tolower(str[i]): str[i];
				hash = hash * 131 + (unsigned long long)v;
			}
			return hash;
		}

	}//namespace runtime
}//namespace skl