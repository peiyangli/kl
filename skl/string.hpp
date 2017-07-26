/*////////////////////////////////////////////////////////
(c)22016
filename: string.hpp
author: pei
version: 0.0.1
date:
description: string
function/class list:
history:
////////////////////////////////////////////////////////*/

#pragma once

#include <ntifs.h>
//#include <string.h>
#include "util.hpp"
namespace skl{

// 	template<class _T>
// 	inline _T min(const _T& a, const T& b){
// 		return a < b ? a : b;
// 	}
// 	inline _T max(const _T& a, const T& b){
// 		return a > b ? a : b;
// 	}

	class string{
		UNICODE_STRING _unicodeStr;

		void Initialize(const UNICODE_STRING& us)
		{
			USHORT strLen = us.Length / 2;
			USHORT realLen = strLen * 2;
			realLen = realLen > us.MaximumLength ? us.MaximumLength : realLen;

			_unicodeStr.MaximumLength = (USHORT)((realLen + sizeof(WCHAR)));
			_unicodeStr.Buffer = new WCHAR[_unicodeStr.MaximumLength / 2];
			if (!_unicodeStr.Buffer){
				_unicodeStr.MaximumLength = 0;
				_unicodeStr.Length = 0;
				return;
			}
			_unicodeStr.Length = realLen;
			//RtlZeroMemory(m_unicodeStr.Buffer, m_unicodeStr.MaximumLength);
			if (_unicodeStr.Length){
				RtlCopyMemory(_unicodeStr.Buffer, us.Buffer, realLen);
			}
		}

		void Initialize(const wchar_t* buf, unsigned strLen)
		{
			USHORT realLen = (USHORT)strLen * 2;
			_unicodeStr.MaximumLength = (USHORT)((realLen + sizeof(WCHAR)));
			_unicodeStr.Buffer = new WCHAR[_unicodeStr.MaximumLength / 2];
			if (!_unicodeStr.Buffer){
				_unicodeStr.MaximumLength = 0;
				_unicodeStr.Length = 0;
				return;
			}
			_unicodeStr.Length = realLen;
			//RtlZeroMemory(m_unicodeStr.Buffer, m_unicodeStr.MaximumLength);
			if (_unicodeStr.Length){
				RtlCopyMemory(_unicodeStr.Buffer, buf, realLen);
			}
		}
		inline void emptyPtr(){
			if (_unicodeStr.Buffer) delete[] _unicodeStr.Buffer;
			_unicodeStr.Buffer = nullptr;
		}
	public:
		static unsigned const npos = (unsigned)-1;

		string()
		{
			_unicodeStr.Buffer = 0;
			_unicodeStr.MaximumLength = 0;
			_unicodeStr.Length = 0;
		}
		string(unsigned size)
		{
			_unicodeStr.Buffer = size > 0 ? new WCHAR[size] : nullptr;
			_unicodeStr.MaximumLength = (USHORT)(_unicodeStr.Buffer?size * 2:0);
			_unicodeStr.Length = 0;
		}
		string(const UNICODE_STRING& us)
		{
			Initialize(us);
		}

		static inline unsigned rawlen(const wchar_t* buf){
			unsigned strLen = 0;
			while (*buf++ != 0)strLen++;
			return strLen;
		}
		//take care of buf
		string(const wchar_t* buf, unsigned strLen = npos)
		{
			if (strLen == npos){
				strLen = rawlen(buf);
			}
			Initialize(buf, strLen);
		}
		string(const string& s){
			if (s.length() > 0){
				unsigned maxlen = s._unicodeStr.Length / 2 + 1;
				_unicodeStr.Buffer = new WCHAR[maxlen];
				if (_unicodeStr.Buffer){
					_unicodeStr.MaximumLength = (USHORT)maxlen * 2;
					_unicodeStr.Length = s._unicodeStr.Length;
					RtlCopyMemory(_unicodeStr.Buffer, s._unicodeStr.Buffer, s._unicodeStr.Length);
					return;
				}
			}
			//failed
			_unicodeStr.Buffer = 0;
			_unicodeStr.MaximumLength = 0;
			_unicodeStr.Length = 0;

		}
		string& operator = (const string& s){
			if (this == &s)return *this;
			clear();
			//reallocate
			if (s.length() > 0){
				if (s._unicodeStr.Length >= _unicodeStr.MaximumLength){
					emptyPtr();
					unsigned maxlen = s._unicodeStr.Length / 2 + 1;
					_unicodeStr.Buffer = new WCHAR[maxlen];
					if (_unicodeStr.Buffer)
						_unicodeStr.MaximumLength = (USHORT)maxlen * 2;
				}

				if (_unicodeStr.Buffer){
					_unicodeStr.Length = s._unicodeStr.Length;
					RtlCopyMemory(_unicodeStr.Buffer, s._unicodeStr.Buffer, s._unicodeStr.Length);
					return *this;
				}
			}
			return *this;
		}
		string& operator = (string&& s){
			s.swap(*this);
			return *this;
		}

		~string()
		{
			emptyPtr();
		}

		string(string&& s){
			_unicodeStr = s._unicodeStr;

			s._unicodeStr.Buffer = 0;
			s._unicodeStr.MaximumLength = 0;
			s._unicodeStr.Length = 0;
		}

		operator PUNICODE_STRING()
		{
			return &_unicodeStr;
		}

		string& swap(string& r){
			if (this == &r)return *this;
			auto tem = _unicodeStr;
			_unicodeStr = r._unicodeStr;
			r._unicodeStr = tem;
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		bool isValid()const{
			return _unicodeStr.Buffer != nullptr;
		}
		operator bool()const{
			return _unicodeStr.Buffer != nullptr;
		}

		inline unsigned length()const{
			return _unicodeStr.Buffer ? _unicodeStr.Length / 2 : 0;
		}
		WCHAR operator[](unsigned pos)const{
			if (!_unicodeStr.Buffer || pos >= (unsigned)_unicodeStr.Length / 2)return 0;
			return _unicodeStr.Buffer[pos];
		}
		WCHAR at(unsigned pos, bool* ok = nullptr){
			if (!_unicodeStr.Buffer || pos >= (unsigned)_unicodeStr.Length / 2){ if (ok)*ok = false; return 0; }
			return _unicodeStr.Buffer[pos];
		}
		//////////////////////////////////////////////////////////////////////////
		void clear(){
			_unicodeStr.Length = 0;
		}
// 		int compare(const string& str) const {
// 			if (this == &str )return 0;
// 			auto thislen = length();
// 			auto strlen = str.length();
// 			auto minlen = min(thislen, strlen);
// 			for (unsigned i = 0; i < minlen; ++i) {
// 				if (_unicodeStr.Buffer[i] < str._unicodeStr.Buffer[i])
// 					return -1;
// 				if (_unicodeStr.Buffer[i] > str._unicodeStr.Buffer[i])
// 					return 1;
// 			}
// 			return thislen == strlen ? 0 : (thislen<strlen?-1:1);
// 		}
// 
// 		int compare(const UNICODE_STRING& str) const {
// 			auto thislen = length();
// 			unsigned strlen = str.Buffer ? str.Length / 2 : 0;
// 			auto minlen = min(thislen, strlen);
// 			for (unsigned i = 0; i < minlen; ++i) {
// 				if (_unicodeStr.Buffer[i] < str.Buffer[i])
// 					return -1;
// 				if (_unicodeStr.Buffer[i] > str.Buffer[i])
// 					return 1;
// 			}
// 			return thislen == strlen ? 0 : (thislen < strlen ? -1 : 1);
// 		}

// 		int compare(const wchar_t* str, unsigned strlen = npos) const {
// 			if (strlen == npos){
// 				strlen = rawlen(str);
// 			}
// 
// 			auto thislen = length();
// 			auto minlen = min(thislen, strlen);
// 			for (unsigned i = 0; i < minlen; ++i) {
// 				if (_unicodeStr.Buffer[i] < str[i])
// 					return -1;
// 				if (_unicodeStr.Buffer[i] > str[i])
// 					return 1;
// 			}
// 			return thislen == strlen ? 0 : (thislen < strlen ? -1 : 1);
// 		}
		//////////////////////////////////////////////////////////////////////////
		int compare(const wchar_t* str, unsigned strlen) const {
			unsigned i = 0;
			auto thislen = length();
			for (; i < strlen && i < thislen; ++i)
			{
				if (_unicodeStr.Buffer[i] < str[i])
					return -1;
				if (_unicodeStr.Buffer[i] > str[i])
					return 1;
			}
			return strlen < thislen ? 1 : (thislen < strlen ? -1 : 0);
		}

		int compare(const wchar_t* str) const {
			unsigned i = 0;
			auto thislen = length();
			for (; str[i] && i < thislen; ++i)
			{
				if (_unicodeStr.Buffer[i] < str[i])
					return -1;
				if (_unicodeStr.Buffer[i] > str[i])
					return 1;
			}
			return i < thislen ? 1 : (str[i]?-1:0);
		}

		int compare(const string& str) const {
			return compare(str._unicodeStr.Buffer, str._unicodeStr.Length/2);
		}
		int compare(const UNICODE_STRING& str) const {
			return compare(str.Buffer, str.Length / 2);
		}

		bool operator<(const string& r)const{
			return compare(r) < 0;
		}

		static unsigned icomparemem(const wchar_t* str, const wchar_t* str2, unsigned strlen){
			unsigned i = 0;
			for (; i < strlen; ++i)
			{
				auto a = skl::tolower(str2[i]);
				auto b = skl::tolower(str[i]);
				if (a != b)
					break;
			}
			return i;
		}
		static unsigned icomparemem(const wchar_t* str, const wchar_t* str2, unsigned strlen, wchar_t skip2){
			unsigned i = 0;
			for (; i < strlen; ++i)
			{
				auto b = skl::tolower(str[i]);
				if (b == skip2)continue;
				auto a = skl::tolower(str2[i]);
				if (a != b)
					break;
			}
			return i;
		}
		static unsigned comparemem(const wchar_t* str, const wchar_t* str2, unsigned strlen){
			unsigned i = 0;
			for (; i < strlen; ++i)
			{
				if (str2[i] != str[i])
					break;
			}
			return i;
		}
		static unsigned comparemem(const wchar_t* str, const wchar_t* str2, unsigned strlen, wchar_t skip2){
			unsigned i = 0;
			for (; i < strlen; ++i)
			{
				if (skip2 == str2[i])continue;
				if (str2[i] != str[i])
					break;
			}
			return i;
		}


		int icompare(const wchar_t* str, unsigned strlen) const {
			unsigned i = 0;
			auto thislen = length();
			for (; i < strlen && i < thislen; ++i)
			{
				auto a = skl::tolower(_unicodeStr.Buffer[i]);
				auto b = skl::tolower(str[i]);
				if ( a< b)
					return -1;
				if (a > b)
					return 1;
			}
			return strlen < thislen ? 1 : (thislen < strlen ? -1 : 0);
		}

		int icompare(const wchar_t* str) const {
			unsigned i = 0;
			auto thislen = length();
			for (; str[i] && i < thislen; ++i)
			{
				auto a = skl::tolower(_unicodeStr.Buffer[i]);
				auto b = skl::tolower(str[i]);
				if (a < b)
					return -1;
				if (a > b)
					return 1;
			}
			return i < thislen ? 1 : (str[i] ? -1 : 0);
		}
		int icompare(const string& str) const {
			return icompare(str._unicodeStr.Buffer, str._unicodeStr.Length / 2);
		}
		int icompare(const UNICODE_STRING& str) const {
			return icompare(str.Buffer, str.Length / 2);
		}
		//////////////////////////////////////////////////////////////////////////

		string substr(unsigned pos = 0, unsigned count = npos) const{
			auto thislen = length();
			auto copylen = min(thislen > pos? thislen - pos:0, count);
			if (copylen < 1)return string();
			return string(_unicodeStr.Buffer + pos, copylen);
		}
		//////////////////////////////////////////////////////////////////////////
		static bool startwith(const wchar_t* str, unsigned strLen, const wchar_t* start, unsigned sLen){
			if (!str)return false;
			if (!start)return false;//may be true
			if (sLen > strLen)return false;
			unsigned i = 0;
			for (; i < sLen; ++i) {
				if (str[i] != start[i])
					return false;
			}
			return true;
		}
		friend bool startwith(const string& s, const string& start){
			return startwith(s._unicodeStr.Buffer, s._unicodeStr.Length / 2, start._unicodeStr.Buffer, start._unicodeStr.Length / 2);
		}
		friend bool startwith(const UNICODE_STRING&  s, const string& start){
			return startwith(s.Buffer, s.Length / 2, start._unicodeStr.Buffer, start._unicodeStr.Length / 2);
		}
		friend bool startwith(const wchar_t* s, const string& start){
			auto rawLen = rawlen(s);
			return startwith(s, rawLen, start._unicodeStr.Buffer, start._unicodeStr.Length / 2);
		}


		bool startwith(const wchar_t* start, unsigned strLen = npos)
		{
			if (!start)return false;

			unsigned melen = _unicodeStr.Length / 2;
			unsigned i = 0;
			for (; (i < strLen && i < melen && start[i]); ++i) {//while zero break, without care-about strLen, different from impliment below witch has been commented
				if (_unicodeStr.Buffer[i] != start[i])
					return false;
			}
			return (i == strLen) || (start[i] == 0);
// 			if (!start)return false;
// 			if (strLen == npos){
// 				strLen = rawlen(start);
// 			}
// 			if (strLen*2 > _unicodeStr.Length)return false;
// 			for (unsigned i = 0; i < strLen; ++i)
// 			{
// 				if (_unicodeStr.Buffer[i] != start[i])
// 					return false;
// 			}
// 			return true;
		}

		bool startwith(const UNICODE_STRING& start){
			if (start.Length > _unicodeStr.Length)return false; //
			return startwith(start.Buffer, start.Length / 2);
		}

		bool istartwith(const wchar_t* start, unsigned strLen = npos)
		{
			if (!start)return false;

			unsigned melen = _unicodeStr.Length / 2;
			unsigned i = 0;
			for (; (i < strLen && i<melen && start[i]); ++i) {
				if (skl::tolower(_unicodeStr.Buffer[i]) != skl::tolower(start[i]))
					return false;
			}
			return (i == strLen) || (start[i] == 0);
		}

		static bool istartwith(const wchar_t* str, unsigned strLen, const wchar_t* start, unsigned sLen){
			if (!str)return false;
			if (!start)return false;//may be true
			if (sLen > strLen)return false;
			unsigned i = 0;
			for (; i < sLen; ++i) {
				if (skl::tolower(str[i]) != skl::tolower(start[i]))
					return false;
			}
			return true;
		}

		//for no construct string
		friend bool istartwith(const string& s, const string& start){
			return istartwith(s._unicodeStr.Buffer, s._unicodeStr.Length / 2, start._unicodeStr.Buffer, start._unicodeStr.Length / 2);
		}
		friend bool istartwith(const UNICODE_STRING&  s, const string& start){
			return istartwith(s.Buffer, s.Length / 2, start._unicodeStr.Buffer, start._unicodeStr.Length / 2);
		}
		friend bool istartwith(const wchar_t* s, const string& start){
			auto rawLen = rawlen(s);
			return istartwith(s, rawLen, start._unicodeStr.Buffer, start._unicodeStr.Length / 2);
		}

		bool istartwith(const UNICODE_STRING& start){
			if (start.Length > _unicodeStr.Length)return false;
			return istartwith(start.Buffer, start.Length / 2);
		}

		//////////////////////////////////////////////////////////////////////////
		bool endwith(const wchar_t* s, unsigned strLen = npos)
		{
			if (!s)return false;
			unsigned melen = _unicodeStr.Length / 2;
			if (strLen == npos){
				strLen = rawlen(s);
			}
			if (strLen > melen)return false;
			for (unsigned i = strLen; i > 0; --i) {
				if (_unicodeStr.Buffer[melen - i] != s[strLen - i])
					return false;
			}
			return true;
		}
		bool endwith(const UNICODE_STRING& s){
			if (s.Length > _unicodeStr.Length)return false; //
			return endwith(s.Buffer, s.Length / 2);
		}

		static bool endwith(const wchar_t* str, unsigned strLen, const wchar_t* start, unsigned startLen)
		{
			if (!str || !start)return false;
			if (startLen > strLen)return false;
			for (unsigned i = startLen; i > 0; --i) {
				if (str[strLen - i] != start[startLen - i])
					return false;
			}
			return true;
		}

		friend bool endwith(const string& s, const string& start){
			return endwith(s._unicodeStr.Buffer, s._unicodeStr.Length / 2, start._unicodeStr.Buffer, start._unicodeStr.Length / 2);
		}
		friend bool endwith(const UNICODE_STRING&  s, const string& start){
			return endwith(s.Buffer, s.Length / 2, start._unicodeStr.Buffer, start._unicodeStr.Length / 2);
		}
		friend bool endwith(const wchar_t* s, const string& start){
			auto rawLen = rawlen(s);
			return endwith(s, rawLen, start._unicodeStr.Buffer, start._unicodeStr.Length / 2);
		}

		bool iendwith(const wchar_t* s, unsigned strLen = npos)
		{
			if (!s)return false;
			unsigned melen = _unicodeStr.Length / 2;
			if (strLen == npos){
				strLen = rawlen(s);
			}
			if (strLen > melen)return false;
			for (unsigned i = strLen; i > 0; --i) {
				if (skl::tolower(_unicodeStr.Buffer[melen - i]) != skl::tolower(s[strLen - i]))
					return false;
			}
			return true;
		}
		bool iendwith(const UNICODE_STRING& s){
			if (s.Length > _unicodeStr.Length)return false; //
			return iendwith(s.Buffer, s.Length / 2);
		}

		static bool iendwith(const wchar_t* str, unsigned strLen, const wchar_t* start, unsigned startLen)
		{
			if (!str || !start)return false;
			if (startLen > strLen)return false;
			for (unsigned i = startLen; i > 0; --i) {
				if (skl::tolower(str[strLen - i]) != skl::tolower(start[startLen - i]))
					return false;
			}
			return true;
		}

		friend bool iendwith(const string& s, const string& start){
			return iendwith(s._unicodeStr.Buffer, s._unicodeStr.Length / 2, start._unicodeStr.Buffer, start._unicodeStr.Length / 2);
		}
		friend bool iendwith(const UNICODE_STRING&  s, const string& start){
			return iendwith(s.Buffer, s.Length / 2, start._unicodeStr.Buffer, start._unicodeStr.Length / 2);
		}
		friend bool iendwith(const wchar_t* s, const string& start){
			auto rawLen = rawlen(s);
			return iendwith(s, rawLen, start._unicodeStr.Buffer, start._unicodeStr.Length / 2);
		}

		//////////////////////////////////////////////////////////////////////////
		unsigned find(const wchar_t* str, unsigned matchlen = npos, unsigned pos = 0){
			if (matchlen < 1)return npos;
			if (matchlen == npos){
				matchlen = rawlen(str);
			}
			unsigned memmatchLen = 2 * matchlen;
			auto thislen = length();
			for (; pos + matchlen <= thislen; ++pos) {
				if (RtlCompareMemory(_unicodeStr.Buffer + pos, str, memmatchLen) == memmatchLen)
					return pos;
			}
			return npos;
		}

		unsigned ifind(const wchar_t* str, unsigned matchlen = npos, unsigned pos = 0){
			if (matchlen < 1)return npos;
			if (matchlen == npos){
				matchlen = rawlen(str);
			}
			auto thislen = length();
			for (; pos + matchlen <= thislen; ++pos) {
				if (icomparemem(_unicodeStr.Buffer + pos, str, matchlen) == matchlen)
					return pos;
			}
			return npos;
		}


		unsigned find(wchar_t chr, unsigned pos = 0){
			auto thislen = length();
			for (; pos < thislen; ++pos) {
				if (chr == _unicodeStr.Buffer[pos])
					return pos;
			}
			return npos;
		}
		unsigned ifind(wchar_t chr, unsigned pos = 0){
			auto thislen = length();
			chr = skl::tolower(chr);
			for (; pos < thislen; ++pos) {
				if (chr == skl::tolower(_unicodeStr.Buffer[pos]))
					return pos;
			}
			return npos;
		}

		unsigned rfind(const wchar_t* str, unsigned matchlen = npos){

			if (matchlen == npos){
				matchlen = rawlen(str);
			}
			auto thislen = length();
			unsigned memmatchLen = 2 * matchlen;
			if (matchlen > thislen)return npos;
			for (unsigned pos = thislen - matchlen; pos > 0; --pos) {
				if (RtlCompareMemory(_unicodeStr.Buffer + pos, str, memmatchLen) == memmatchLen)
					return pos;
			}
			return npos;
		}

		unsigned rifind(const wchar_t* str, unsigned matchlen = npos){

			if (matchlen == npos){
				matchlen = rawlen(str);
			}
			auto thislen = length();
			if (matchlen > thislen)return npos;
			for (unsigned pos = thislen - matchlen; pos > 0; --pos) {
				if (icomparemem(_unicodeStr.Buffer + pos, str, matchlen) == matchlen)
					return pos;
			}
			return npos;
		}

		unsigned rfind(wchar_t chr){
			auto thislen = length();
			for (unsigned pos = thislen; pos > 0; --pos) {
				if (chr == _unicodeStr.Buffer[pos - 1])
					return pos - 1;
			}
			return npos;
		}
		unsigned rifind(wchar_t chr){
			auto thislen = length();
			chr = skl::tolower(chr);
			for (unsigned pos = thislen; pos > 0; --pos) {
				if (chr == skl::tolower(_unicodeStr.Buffer[pos - 1]))
					return pos - 1;
			}
			return npos;
		}

		string& replace(WCHAR old, WCHAR nc){
			unsigned len = 0;
			if ((len = length()) < 1)return *this; //false
			for (unsigned i = 0; i < len; ++i) {
				if (_unicodeStr.Buffer[i] == old)
					_unicodeStr.Buffer[i] = nc;
			}
			return *this;
		}


		static string repalce(const wchar_t* str, unsigned strLen, const wchar_t* old, const wchar_t* ns){
			if (!str||!old || !ns)return string();
			auto osl = rawlen(old);
			//auto nsl = rawlen(ns);
			unsigned repalce_count = 0;
			for (unsigned i = 0; i + osl < strLen; ++i) {
				if (osl == comparemem(str+i, old, osl)){
					repalce_count++;
					i += osl - 1;
					//this i should be recorded for replacing using, but here we have no proper container to do so
				}
			}
			if (repalce_count > 0){
				auto nsl = rawlen(ns);
				auto nm = nsl*sizeof(wchar_t);
				string rt(strLen + repalce_count*(nsl - osl));
				if (!rt)return rt;//this is an error
				//do replace
				unsigned i = 0;
				unsigned pre = 0;
				unsigned copy_len = 0;
				for (; i + osl < strLen; ++i) {
					if (osl == comparemem(str+i, old, osl)){
						//copy
						RtlCopyMemory(rt._unicodeStr.Buffer + copy_len, str + pre, (i - pre)*sizeof(wchar_t));
						copy_len += (i - pre);
						RtlCopyMemory(rt._unicodeStr.Buffer + copy_len, ns, nm);
						copy_len +=  nsl;

						pre = i + osl;
						i += osl - 1;
					}
				}
				if (pre < strLen){
					RtlCopyMemory(rt._unicodeStr.Buffer + copy_len, str + pre, (strLen - pre)*sizeof(wchar_t));
					copy_len += (strLen - pre);
				}
				rt._unicodeStr.Length = (USHORT)copy_len*sizeof(wchar_t);

				//return rt;
				return skl::move(rt);
			}
			else{
				return string(str, strLen);
			}
			
		}

		string repalce(const wchar_t* old, const wchar_t* ns){
			if (!old || !ns)return string();
			return repalce(_unicodeStr.Buffer, _unicodeStr.Length/2, old, ns);
		}


		static string irepalce(const wchar_t* str, unsigned strLen, const wchar_t* old, const wchar_t* ns){
			if (!str || !old || !ns)return string();
			auto osl = rawlen(old);
			//auto nsl = rawlen(ns);
			unsigned repalce_count = 0;
			for (unsigned i = 0; i + osl < strLen; ++i) {
				if (osl == icomparemem(str + i, old, osl)){
					repalce_count++;
					i += osl - 1;
					//this i should be recorded for replacing using, but here we have no proper container to do so
				}
			}
			if (repalce_count > 0){
				auto nsl = rawlen(ns);
				auto nm = nsl*sizeof(wchar_t);
				string rt(strLen + repalce_count*(nsl - osl));
				if (!rt)return rt;//this is an error
				//do replace
				unsigned i = 0;
				unsigned pre = 0;
				unsigned copy_len = 0;
				for (; i + osl < strLen; ++i) {
					if (osl == icomparemem(str + i, old, osl)){
						//copy
						RtlCopyMemory(rt._unicodeStr.Buffer + copy_len, str + pre, (i - pre)*sizeof(wchar_t));
						copy_len += (i - pre);
						RtlCopyMemory(rt._unicodeStr.Buffer + copy_len, ns, nm);
						copy_len += nsl;

						pre = i + osl;
						i += osl - 1;
					}
				}
				if (pre < strLen){
					RtlCopyMemory(rt._unicodeStr.Buffer + copy_len, str + pre, (strLen - pre)*sizeof(wchar_t));
					copy_len += (strLen - pre);
				}
				rt._unicodeStr.Length = (USHORT)copy_len*sizeof(wchar_t);

				//return rt;
				return skl::move(rt);
			}
			else{
				return string(str, strLen);
			}

		}

		string irepalce(const wchar_t* old, const wchar_t* ns){
			if (!old || !ns)return string();
			return irepalce(_unicodeStr.Buffer, _unicodeStr.Length / 2, old, ns);
		}

	};
}//namespace skl

#include "refholder.hpp"

namespace skl{
	template<>
	inline bool check_valid<string>(const string& v){
		return v.isValid();
	}
}