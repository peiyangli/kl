#pragma once


#include "mutex.hpp"
#include "list.hpp"
#include "event.hpp"
namespace skl{

	enum E_DEQUE_RT{
		E_EXIT,
		E_OK,
		E_FALSE
	};

	template<class __QueueType>
	class WorkQueue {
		queue<__QueueType> _q;
		mutex _lock;
		skl::event _e;
		volatile bool _exit;
	public:
		WorkQueue() : _exit(false){}
		~WorkQueue() {
		}
		bool goExit(){ auto rt = _exit; _exit = true; /* _e.notify();*/ return rt; }

		E_DEQUE_RT deque(__QueueType& o) { //only working thread dequeue
			_lock.lock();
			while (_q.empty()) {
				_lock.unlock();
				//wait event here
				_e.wait_for(100000);//a while and check the flag of exit
				//_e.wait();
				//while timeout?
				if (_exit){ return E_EXIT; }
				_lock.lock();
			}
			auto v = _q.pop_front(o);
			_lock.unlock();
			return v ? E_OK : E_FALSE; //no copy if needed
		}
		bool enque(const __QueueType& v) {
			bool empty = false;
			bool ok = false;
			{
				skl::auto_locker<skl::mutex> locker(_lock);
				empty = _q.empty();
				ok = _q.push_back(v);
			}
			if (empty && ok)
				_e.notify();
				empty;
			return ok;
		}

		bool emptyEnque(const __QueueType& v) {
			skl::queue<__QueueType> ept;
			bool ok = false;
			{
				skl::auto_locker<skl::mutex> locker(_lock);
				_q.swap(ept);
				ok = _q.push_back(v);
			}
			//set event here
			if (ok)
				_e.notify();
			return ok;
		}

		void clear() {
			skl::queue<__QueueType> ept;
			{
				skl::auto_locker<skl::mutex> locker(_lock);
				_q.swap(ept);
			}
			//set event here
			_e.notify();
			return;
		}
	};
}

#pragma warning(push)
#pragma warning(disable: 4127) // while (false);

namespace skl{

	class base_a_call_ref_item : public ref_item{
		friend class just_a_call;
	public:
		base_a_call_ref_item(long n = 0) :ref_item(n){};
	};
	template<class _KeyT>
	class a_call_ref_item : public base_a_call_ref_item{
		friend class just_a_call;
	public:
		a_call_ref_item(){}
		a_call_ref_item(const _KeyT& k) : first(k), base_a_call_ref_item(1){
		}
		_KeyT first;
	};

	//a functor like class, but only works with none-param(rt(void)) simple functions, like lambda or raw function.
	class just_a_call{

		typedef VOID (*pProc)(_In_ PVOID StartContext, _Out_ ULONG* o);

		pProc _routing;
		base_a_call_ref_item* _param;

		template<class _Fn>
		static VOID Proc(
			_In_ PVOID StartContext, _Out_ ULONG* o
			){
			if (StartContext){
				auto holder = (shared_ref_item<shared_ptr<_Fn>>*)StartContext;
				auto rt = (*(holder->first))();
				if (o)*o = rt;
			}
		}
	public:
		just_a_call() : _routing(NULL), _param(NULL){
		}
		template<class _Fn>
		just_a_call(_Fn&& _Fx): _routing(NULL), _param(NULL){
			do
			{
				auto ptr = helper::make_shared<_Fn>(_Fx);
				if (!ptr)
					break;
				auto holder = new a_call_ref_item<shared_ptr<_Fn>>(ptr);
				if (!holder)
					break;
				_param = holder;
				_routing = Proc<_Fn>;
			} while (false);
		}

		just_a_call(const just_a_call& r) :_routing(r._routing),
		_param(r._param){
			if (_param)
				_param->retain();
		}
		just_a_call(just_a_call&& r) :_routing(r._routing),
			_param(r._param){
			r._routing = NULL;
			r._param = NULL;
			if (_param)
				_param->retain();
		}
		just_a_call& operator=(const just_a_call& r){
			return just_a_call(r).swap(*this);
		}
		just_a_call& operator=(just_a_call&& r){
			return r.swap(*this);
		}

		~just_a_call(){
			if (_param)
				_param->release();
		}

		just_a_call& swap(just_a_call& r){
			if (this != &r){
				auto tf = r._routing;
				auto tp = r._param;

				r._routing = _routing;
				r._param = _param;

				_routing = tf;
				_param = tp;
			}
			return *this;
		}

		void operator()(ULONG* o = NULL){
			if (_routing)
				_routing(_param, o);
		}
		operator bool(){
			return _routing != NULL;
		}
		bool isValid()const{
			return _routing != NULL;
		}
	};

	template<>
	inline bool check_valid<just_a_call>(const just_a_call& v){
		return v.isValid();
	}



	
}

#include "shared_ptr.hpp"
#include "thread.hpp"
namespace skl{
	class simple_worker{
		skl::shared_ptr<skl::WorkQueue<skl::just_a_call>> _queue;
		skl::thread _t;

		bool init(){

			bool ok = false;
			do 
			{
				auto wq = skl::helper::make_shared<skl::WorkQueue<skl::just_a_call>>();
				if (!wq)break;

				skl::thread t([wq](){
					skl::just_a_call o;
					E_DEQUE_RT exit = E_OK;
					while (true)
					{
						exit = wq->deque(o);
						if (exit == E_OK){
							o();
						}
						else if (exit == E_EXIT){
							break;
						}
						else{
							//dbgprint('some error occurr');
						}
					}
				});
				if (!t)break;

				_t.swap(t);
				_queue = wq;
				ok = true;
			} while (false);
			return ok;
		}

		friend class skl::helper; //ugly
		simple_worker(){
		}
		simple_worker(const simple_worker&) = delete;
		simple_worker(simple_worker&&) = delete;
		simple_worker& operator=(const simple_worker&) = delete;
		simple_worker& operator=(simple_worker&&) = delete;
	public:
		~simple_worker(){
			stop();//no detach
		}
		static skl::shared_ptr<simple_worker> create(){
			auto rt = skl::helper::make_shared<simple_worker>();
			if (!rt)return rt;
			bool ok = rt->init();
			if (!ok)return shared_ptr<simple_worker>();
			return rt;
		}

		//warning: use lambda to capture some raw ptr is not save(memory leak), and hard to free.(must free when return false, and must then never call clear)
		//recommend to use shared_ptr
		bool push_a_async_call(just_a_call&& c){
			if (!c)return false;
			return _queue->enque(c);
		}
		bool stop(){ //return old status
			return _queue->goExit();
		}

		//clear tasks in queue, when raw ptr is captured by instance of some just_a_call/lambda, you must not call this for voiding of memory leak
		void clear(){
			_queue->clear();
		}

		//stop and wait the thread to exit, all work in queue will be done
		void detach(){//wait to stop;
			stop();
			_t.detach();
		}
	};
}




namespace skl{
	template<class _TInterface>
	class interface_worker{
		skl::shared_ptr<skl::WorkQueue<skl::shared_ptr<_TInterface>>> _queue;
		skl::thread _t;

		bool init(){

			bool ok = false;
			do
			{
				auto wq = skl::helper::make_shared<skl::WorkQueue<skl::shared_ptr<_TInterface>>>();
				if (!wq)break;

				skl::thread t([wq](){
					skl::shared_ptr<_TInterface> o;
					E_DEQUE_RT exit = E_OK;
					while (true)
					{
						exit = wq->deque(o);
						if (exit == E_OK){
							if (o)
								(*o)();
						}
						else if (exit == E_EXIT){
							break;
						}
						else{
							//dbgprint('some error occurr');
						}
					}
				});
				if (!t)break;

				_t.swap(t);
				_queue = wq;
				ok = true;
			} while (false);
			return ok;
		}

		friend class skl::helper; //ugly
		interface_worker(){
		}
		interface_worker(const interface_worker&) = delete;
		interface_worker(interface_worker&&) = delete;
		interface_worker& operator=(const interface_worker&) = delete;
		interface_worker& operator=(interface_worker&&) = delete;
	public:
		~interface_worker(){
			stop();//no detach
		}
		static skl::shared_ptr<interface_worker> create(){
			auto rt = skl::helper::make_shared<interface_worker>();
			if (!rt)return rt;
			bool ok = rt->init();
			if (!ok)return shared_ptr<interface_worker>();
			return rt;
		}

		bool push(skl::shared_ptr<_TInterface>&& c){
			if (!c)return false;
			return _queue->enque(c);
		}
		bool stop(){ //return old status
			return _queue->goExit();
		}

		void clear(){
			_queue->clear();
		}

		//stop and wait the thread to exit, all work in queue will be done
		void detach(){//wait to stop;
			stop();
			_t.detach();
		}
	};
}

#pragma warning(pop)