
#include "kernel.h"

namespace skl{

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
		return 1UL << highest_bit(n - 1);
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
	template<POOL_TYPE poolType>
	kfifo *kfifo_alloc(unsigned int size)
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

		buffer = (unsigned char *)ExAllocatePoolWithTag(poolType, size, DEF_ALLOCAL_TAG);
		if (!buffer)
			return NULL;

		fifo = (struct kfifo*)ExAllocatePoolWithTag(poolType, sizeof(struct kfifo), DEF_ALLOCAL_TAG);

		if (!fifo) {
			ExFreePool(buffer);
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
		ExFreePool(fifo->buffer);
		ExFreePool(fifo);
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

	template <typename T, int size = 10, POOL_TYPE poolType = NonPagedPool>
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

		Fifo(Fifo&& r) :_kfifo(r._kfifo){
			r._kfifo = nullptr;
		}

		Fifo& operator = (Fifo&& r){
			r.swap(this);
		}
		Fifo() :_kfifo(kfifo_alloc<poolType>(size*sizeof(T)))
		{
		}
// 		Fifo(unsigned int size)
// 		{
// 			
// 		}
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

	template <typename T, int size, POOL_TYPE poolType>
	bool Fifo<T, size, poolType>::push(T data)
	{
		if (!_kfifo)return false;
		int len = 0;
		len = __kfifo_put(_kfifo, (const unsigned char *)&data, sizeof(T));
		if (len > 0)
			return true;
		else
			return false;
	}

	template <typename T, int size, POOL_TYPE poolType>
	bool Fifo<T, size, poolType>::pop(T& data)
	{
		if (!_kfifo)return false;
		int len = __kfifo_get(_kfifo, (unsigned char *)&data, sizeof(T));
		if (len > 0)
			return true;
		else
			return false;
	}

}//namespace skl