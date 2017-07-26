/*////////////////////////////////////////////////////////
(c)22016
filename: util.hpp
author: pei
version: 0.0.1
date:
description: 
function/class list:
history:
////////////////////////////////////////////////////////*/

#pragma once


namespace skl {

	template<typename CharT>
	inline CharT tolower(CharT c) {
		return (c >= 'A' && c <= 'Z') ? (c + 32) : c;
	}

	template<typename CharT>
	inline CharT toupper(CharT c) {
		return (c >= 'a' && c <= 'z') ? (c - 32) : c;
	}
}//namespace skl