// test.cpp : Defines the entry point for the console application.
//
#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers  
#include "windows.h"
//#include "stdafx.h"
#include <iostream>

#include"../sl/Registry.h"
#include"../Debug/dbgonly.hpp"

#define REG_PATH_THREADMSG_TID L"HKEY_CURRENT_USER\\Software\\Anvisoft\\XL"
#define REG_KEYNAME_THREADMSG_TID L"tid"

UINT g_msg = RegisterWindowMessage(L"XL_MSG_FOR_THREAD_COM");


void InitUIPIFilter()
{
	typedef BOOL(WINAPI *ChangeWindowMessageFilterProc)(UINT, DWORD);
	HMODULE hUser = LoadLibraryA("user32.dll");
	if (hUser)
	{
		ChangeWindowMessageFilterProc proc = (ChangeWindowMessageFilterProc)GetProcAddress(hUser, "ChangeWindowMessageFilter");
		if (proc)
		{
			proc(g_msg, 1);
		}
	}
}

bool PostMessageToWorkingThread(WPARAM msg, LPARAM lp){

	Registry reg(REG_PATH_THREADMSG_TID, true);
	auto votid = reg.value(REG_KEYNAME_THREADMSG_TID, (unsigned long long)0);
	bool ok = false;
	auto otid = votid.To<unsigned long long>(0, &ok);
	if (!ok || otid < 5)return false;

	return TRUE == ::PostThreadMessage(otid, g_msg, msg, lp);
}

#include<thread>

int main_test_Thread_msg(){

	std::thread([](){
		{
			unsigned long long Tid = GetCurrentThreadId();
			//set id
			Registry reg(REG_PATH_THREADMSG_TID, true);
			reg.setValue(REG_KEYNAME_THREADMSG_TID, Tid);
		}
		auto onHourTimer = SetTimer(NULL, 0x110, 10000, NULL);

		MSG msg;
		PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
		while (GetMessage(&msg, NULL, 0, 0))
		{
			switch (msg.message)
			{
			case WM_TIMER:
			{
				//onHourTimer == msg.wParam
				DbgPrtFileLineFunctionVar(msg.wParam);
			}break;
			default:
				break;
			}
			DbgPrtFileLineFunctionVar(msg.lParam);
		}

		DbgPrtFileLineFunction;
	}).detach();
	getchar();
	return 0;
}

#define BUF_LEN 512
static std::wstring WindowClassName(HWND hwnd){
	wchar_t buf[BUF_LEN] = {};
	if (0 == GetClassName(hwnd, buf, BUF_LEN - 1)){
		return L"";
	}
	return buf;
}
static std::wstring WindowTitle(HWND hwnd){
	wchar_t buf[BUF_LEN] = {};
	if (0 == InternalGetWindowText(hwnd, buf, BUF_LEN - 1)){
		return L"";
	}
	return buf;
}
#include "../dl/TestExport.h"
#include <array>

#include<locale>
#include <iostream>

template<class _Ty, int _TLen = 8>
void send_duff(_Ty* to, _Ty* from, int count)
{
	int n = (count + _TLen - 1) / _TLen;
	switch (count % _TLen) {
	case 0: do {
		*to++ = *from++;
	case 7:         *to++ = *from++;
	case 6:         *to++ = *from++;
	case 5:         *to++ = *from++;
	case 4:         *to++ = *from++;
	case 3:         *to++ = *from++;
	case 2:         *to++ = *from++;
	case 1:         *to++ = *from++;
	} while (--n > 0);
	}
}


void step(int n){
}

bool coro_fun(int& flag)
{
	bool brt = false;
	switch (flag)
	{
	case -1:
		if (flag)
		{
		terminate_core:
			flag = -1;
			goto bail_out_core;
		bail_out_core :
			break;
		}else
	case 0:
		step(1);
		for (flag = 1;;)
		{
			if (flag == 0)
			{
	case 1:
		break;
			}
			goto bail_out_core;
		}
		step(2);
		for (flag = 2;;)
		{
			if (flag == 0)
			{
	case 2:
		break;
			}
			goto bail_out_core;
		}
		step(3);
		brt = true;
	}
	return brt;
}

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <utility>

#include <boost/coroutine/all.hpp>

struct FinalEOL{
	~FinalEOL(){
		std::cout << std::endl;
	}
};

int main3(int argc, char* argv[]){
	std::vector<std::string> words{
		"peas", "porridge", "hot", "peas",
		"porridge", "cold", "peas", "porridge",
		"in", "the", "pot", "nine",
		"days", "old" };

	int num = 5, width = 15;
	boost::coroutines::asymmetric_coroutine<std::string>::push_type writer(
		[&](boost::coroutines::asymmetric_coroutine<std::string>::pull_type& in){
		// finish the last line when we leave by whatever means
		FinalEOL eol;
		// pull values from upstream, lay them out 'num' to a line
		for (;;){
			for (int i = 0; i < num; ++i){
				// when we exhaust the input, stop
				if (!in) return;
				std::cout << std::setw(width) << in.get();
				// now that we've handled this item, advance to next
				in();
			}
			// after 'num' items, line break
			std::cout << std::endl;
		}
	});

	std::copy(std::begin(words), std::end(words), boost::begin(writer));

	std::cout << "\nDone" << std::endl;

	return EXIT_SUCCESS;
}

int main2(int argc, char* argv[])
{
	
	std::array<int, 10> arr;
	std::vector<int> hell({1,2,3,4});
	std::initializer_list<int> l;
	int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0 };
	memmove(a+2, a, 9*sizeof(int));

	std::function<void(void)> name;
	std::shared_ptr<int> pint;
	if (argc > 1){
		InitUIPIFilter();
		STSetTID(999999);
		main_test_Thread_msg();
	}
	else{
		DbgPrtFileLineFunctionVar(STGetTID());
		PostMessageToWorkingThread(9999, 8888);
	}
	
	return 0;
}


#include <boost/asio.hpp>
int main4(int argc, char* argv[])
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;      //non inheritable handle, same as default
	sa.lpSecurityDescriptor = 0;
	auto handle = CreateNamedPipe(
		L"\\\\.\\pipe\\NAME_OF_ME_TEST_LA", // pipe name
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,       // read/write access
		PIPE_TYPE_BYTE |          // byte type pipe
		PIPE_READMODE_BYTE |      // byte-read mode
		PIPE_WAIT,                // blocking mode
		PIPE_UNLIMITED_INSTANCES, // max. instances
		1024,                  // output buffer size
		1024,                  // input buffer size
		3000,                     // client time-out
		&sa);
	boost::asio::io_service io_service;
	boost::asio::windows::stream_handle pipe(io_service, handle);

	return 0;
}


namespace tst{
	
}



#include <fstream>
#include <windows.h>



int main5()
{
	//std::locale loc(std::locale::classic(), new std::codecvt_utf8<char>);
	//ofs.imbue(loc);
	int max_num = 100000000; //100M memory
	std::unique_ptr<char[]> bytes = std::make_unique<char[]>(max_num);
	auto rbytes = bytes.get();


	auto tick = GetTickCount();

	for (int i = 2; i < max_num; ++i)
	{
		if (rbytes[i] == 1)continue;
		for (int j = i + i; j < max_num; j += i)
		{
			rbytes[j] = 1;
		}
	}

	tick = GetTickCount() - tick;

	int cnt = 0;
	for (int i = 2; i < max_num; ++i)
	{
		if (rbytes[i] == 0)
		{
			cnt++;
		}
	}


	std::ofstream ofs("primes.txt", std::ios_base::app);
	int one_line = 10;
	int count = 0;
	int lines = 0;
	for (int i = 2; i < max_num; ++i)
	{
		if (rbytes[i] == 0)
		{
			//this is a prime number
			count++;
			ofs << i << '\t';
			if (count % 10 == 0){
				lines++;
				ofs << std::endl;
			}	
		}
	}
	ofs.close();

	return 0;
}


//////////////////////////////////////////////////////////////////////////
unsigned int highest_bit(unsigned int n)
{
	if (!n)return 0;
	unsigned int pos = 0;
	for (unsigned int i = (n >> 1); i != 0; ++pos)
		i >>= 1;
	return pos + 1;
}
inline unsigned int roundup_pow_of_two(unsigned int n)
{
	return 1UL << highest_bit(n-1);
}
struct kfifo {
	unsigned char *buffer; /* the buffer holding the data */
	unsigned int size; /* the size of the allocated buffer */
	unsigned int in; /* data is added at offset (in % size) */
	unsigned int out; /* data is extracted from off. (out % size) */
};

/**
* * kfifo_alloc - allocates a new FIFO and its internal buffer
* * @size: the size of the internal buffer to be allocated.
* * @gfp_mask: get_free_pages mask, passed to kmalloc()
* * @lock: the lock to be used to protect the fifo buffer
* *
* * The size will be rounded-up to a power of 2.
* */
struct kfifo *kfifo_alloc(unsigned int size)
{
	unsigned char *buffer;
	struct kfifo *fifo;

	/*
	*       * round up to the next power of 2, since our 'let the indices
	*            * wrap' tachnique works only in this case.
	*                 */
	if (size & (size - 1)) {
		size = roundup_pow_of_two(size);
	}

	buffer = (unsigned char *)malloc(size);
	if (!buffer)
		return NULL;

	fifo = (struct kfifo*)malloc(sizeof(struct kfifo));

	if (!fifo)
	{
		free(buffer);
		return NULL;
	}

	fifo->buffer = buffer;
	fifo->size = size;
	fifo->in = fifo->out = 0;

	return fifo;
}

/**
* * kfifo_free - frees the FIFO
* * @fifo: the fifo to be freed.
* */
void kfifo_free(struct kfifo *fifo)
{
	free(fifo->buffer);
	free(fifo);
}

/**
* __kfifo_put - puts some data into the FIFO, no locking version
* @fifo: the fifo to be used.
* @buffer: the data to be added.
* @len: the length of the data to be added.
*
* This function copies at most @len bytes from the @buffer into
* the FIFO depending on the free space, and returns the number of
* bytes copied.
*
* Note that with only one concurrent reader and one concurrent
* writer, you don't need extra locking to use these functions.
*/
unsigned int __kfifo_put(struct kfifo *fifo,
	const unsigned char *buffer, unsigned int len)
{
	unsigned int l;

	len = min(len, fifo->size - fifo->in + fifo->out);

	/* first put the data starting from fifo->in to buffer end */
	l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));
	memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);

	/* then put the rest (if any) at the beginning of the buffer */
	memcpy(fifo->buffer, buffer + l, len - l);

	fifo->in += len;

	return len;
}

/**
* __kfifo_get - gets some data from the FIFO, no locking version
* @fifo: the fifo to be used.
* @buffer: where the data must be copied.
* @len: the size of the destination buffer.
*
* This function copies at most @len bytes from the FIFO into the
* @buffer and returns the number of copied bytes.
*
* Note that with only one concurrent reader and one concurrent
* writer, you don't need extra locking to use these functions.
*/
unsigned int __kfifo_get(struct kfifo *fifo,
	unsigned char *buffer, unsigned int len)
{
	unsigned int l;

	len = min(len, fifo->in - fifo->out);

	/* first get the data from fifo->out until the end of the buffer */
	l = min(len, fifo->size - (fifo->out & (fifo->size - 1)));
	memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);

	/* then get the rest (if any) from the beginning of the buffer */
	memcpy(buffer + l, fifo->buffer, len - l);

	fifo->out += len;

	return len;
}

/**
* __kfifo_reset - removes the entire FIFO contents, no locking version
* @fifo: the fifo to be emptied.
*/
static inline void __kfifo_reset(struct kfifo *fifo)
{
	fifo->in = fifo->out = 0;
}

/**
* __kfifo_len - returns the number of bytes available in the FIFO, no locking version
* @fifo: the fifo to be used.
*/
static inline unsigned int __kfifo_len(struct kfifo *fifo)
{
	return fifo->in - fifo->out;
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
class Fifo
{
	Fifo(const Fifo&);
	Fifo& operator = (const Fifo&);
private:
	kfifo* _kfifo;
public:
	void swap(Fifo& r){
		auto tmp = r._kfifo;
		r._kfifo = this->_kfifo;
		this->_kfifo = tmp;
	}

	Fifo(Fifo&& r):_kfifo(r._kfifo){
		r._kfifo = nullptr;
	}

	Fifo& operator = (Fifo&& r){
		r.swap(this);
	}
	Fifo() :_kfifo(nullptr)
	{
	}
	Fifo(unsigned int size)
	{
		_kfifo = kfifo_alloc(size*sizeof(T));
	}
	~Fifo()
	{
		if (_kfifo)
			kfifo_free(_kfifo);
	}
	bool push(T data);

	bool pop(T& data);

	bool empty()const{
		return _kfifo ? 0 == __kfifo_len(_kfifo) : true;
	}
	bool length()const{
		return _kfifo ? __kfifo_len(_kfifo) : 0;
	}
};

template <typename T>
bool Fifo<T>::push(T data)
{
	int len = 0;
	len = __kfifo_put(_kfifo, (const unsigned char *)&data, sizeof(T));
	if (len > 0)
		return true;
	else
		return false;
}

template <typename T>
bool Fifo<T>::pop(T& data)
{
	int len = __kfifo_get(_kfifo, (unsigned char *)&data, sizeof(T));
	if (len > 0)
		return true;
	else
		return false;
}

char cv[255] = 
{
//	0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	a,	b,	c,	d,	e,	f
//0 
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
//1 
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
//2 
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
//3 
	0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	0,	0,	0,	0,	0,	0,
//4 
	0,	0xa,0xb,0xc,0xd,0xe,0xf,0,	0,	0,	0,	0,	0,	0,	0,	0,
//5 
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
//4 
	0,	0xa,0xb,0xc,0xd,0xe,0xf,0,	0,	0,	0,	0,	0,	0,	0,	0
};

int main()
{

	Fifo<int> hello(10000);
	
	bool exit = false;
	std::thread at([&hello, &exit](){
		int o = 0;
		while (!exit)
		{
			while (!hello.empty())
			{
				hello.pop(o);
			}
		}
	});
	for (int i = 0; i < 100; ++i)
	{
		Sleep(100);
		hello.push(i);
	}

	exit = true;
	at.join();

	return 0;
}