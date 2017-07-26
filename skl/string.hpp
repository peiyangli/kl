#pragma once

#include <ntifs.h>
//#include <string.h>
namespace skl{
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

		string& operator = (const string& s){
			if (this == &s)return *this;
			clear();
			//reallocate
			if (s.len() > 0){
				if (s._unicodeStr.Length >= _unicodeStr.MaximumLength){
					emptyPtr();
					auto maxlen = s._unicodeStr.Length / 2 + 1;
					_unicodeStr.Buffer = new WCHAR[maxlen];
					if (_unicodeStr.Buffer)
						_unicodeStr.MaximumLength = maxlen*2;
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
			clear();
			emptyPtr();
			_unicodeStr.Buffer = s._unicodeStr.Buffer;
			_unicodeStr.MaximumLength = s._unicodeStr.MaximumLength;
			_unicodeStr.Length = s._unicodeStr.Length;

			s._unicodeStr.Buffer = 0;
			s._unicodeStr.MaximumLength = 0;
			s._unicodeStr.Length = 0;

			return *this;
		}
		inline void emptyPtr(){
			if (_unicodeStr.Buffer) delete[] _unicodeStr.Buffer;
			_unicodeStr.Buffer = nullptr;
		}
	public:
		string()
		{
			_unicodeStr.Buffer = 0;
			_unicodeStr.MaximumLength = 0;
			_unicodeStr.Length = 0;
		}
		string(int size)
		{
			_unicodeStr.Buffer = size > 0 ? new WCHAR[size] : nullptr;
			_unicodeStr.MaximumLength = _unicodeStr.Buffer?size * 2:0;
			_unicodeStr.Length = 0;
		}
		string(const UNICODE_STRING& us)
		{
			Initialize(us);
		}
		string(const string& s){
			if (s.len() > 0){
				auto maxlen = s._unicodeStr.Length / 2 + 1;
				_unicodeStr.Buffer = new WCHAR[maxlen];
				if (_unicodeStr.Buffer){
					_unicodeStr.MaximumLength = maxlen * 2;
					_unicodeStr.Length = s._unicodeStr.Length;
					RtlCopyMemory(_unicodeStr.Buffer, s._unicodeStr.Buffer, s._unicodeStr.Length);
					return;
				}
			}
			_unicodeStr.Buffer = 0;
			_unicodeStr.MaximumLength = 0;
			_unicodeStr.Length = 0;

		}
		string(string&& s){
			_unicodeStr.Buffer = s._unicodeStr.Buffer;
			_unicodeStr.MaximumLength = s._unicodeStr.MaximumLength;
			_unicodeStr.Length = s._unicodeStr.Length;

			s._unicodeStr.Buffer = 0;
			s._unicodeStr.MaximumLength = 0;
			s._unicodeStr.Length = 0;
		}
		
		~string()
		{
			emptyPtr();
		}
		operator PUNICODE_STRING()
		{
			return &_unicodeStr;
		}

		//////////////////////////////////////////////////////////////////////////
		bool isValid(){
			return _unicodeStr.Buffer != nullptr;
		}

		int len()const{
			return _unicodeStr.Buffer ? _unicodeStr.Length / 2 : 0;
		}
		WCHAR operator[](int pos)const{
			if (!_unicodeStr.Buffer || pos < 0 || pos >= _unicodeStr.Length / 2)return 0;
			return _unicodeStr.Buffer[pos];
		}
		WCHAR at(int pos, bool* ok = nullptr){
			if (!_unicodeStr.Buffer || pos < 0 || pos >= _unicodeStr.Length / 2){ if (ok)*ok = false; return 0; }
			return _unicodeStr.Buffer[pos];
		}
		//////////////////////////////////////////////////////////////////////////
		void clear(){
			_unicodeStr.Length = 0;
		}
		string& replace(WCHAR old, WCHAR nc){
			if (len() < 1)return *this; //false
			auto len = _unicodeStr.Length / 2;
			for (int i = 0; i < len; ++i) {
				if (_unicodeStr.Buffer[i] == old)
					_unicodeStr.Buffer[i] =  nc;
			}
			return *this;
		}

		int compare(const string& str) const {
			if (this == &str)return 0;


			return -1;
		}

		string substr(int pos = 0, int count = -1) const{
			return *this;
		}

		int find(string&, int pos = 0){
			return -1;
		}
	};
}//namespace skl