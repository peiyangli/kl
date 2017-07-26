#pragma once
//#include <winnt.h>
namespace skl{

	

	template<typename T>
	class list_item{
		//LIST_ENTRY e;
	public:
		static void* operator new(size_t size);
		static void operator delete(void *ptr, size_t size);
	};

	template<typename T>
	void* operator list_item::new(size_t size){
		auto ts = sizeof(T);
		return 0;
	}
	template<typename T>
	void operator list_item::delete(void *ptr, size_t size){
		auto ts = sizeof(T);
		return 0;
	}

	class list_item_one : public list_item<list_item_one>{
	public:
		int v;
		int o;
		int t;
	};

	template<typename T>
	class list{
	public:
		list();
		~list();
		int size();
		bool push();
		T pop();
	};
}