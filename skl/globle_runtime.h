/*////////////////////////////////////////////////////////
(c)22016
filename: globle_runtime.h
author: pei
version: 0.0.1
date:
description: support for global runtime init and uninit
function/class list:
history:
////////////////////////////////////////////////////////*/

#pragma once

#include"lookaside.hpp"

namespace skl{

	namespace runtime{
		//lookaside
		//shared_ptr ref-count
		//shared-list item-wraper
		//uint64table ALV tree element
		class GlobleRuntime
		{
			GlobleRuntime(const GlobleRuntime&);
			GlobleRuntime(GlobleRuntime&&);
			GlobleRuntime& operator=(const GlobleRuntime&);
			GlobleRuntime& operator=(GlobleRuntime&&);
		public:
			GlobleRuntime();
			~GlobleRuntime();
		public:
			//sizeof(none_paged::LookasideAtomic32<unsigned>), sizeof(item_type_avl), sizeof(Uint64ItemAVL)
#ifdef _WIN64
			typedef skl::LookAsideSizePool<NonPagedPool, 4, 36, 40> lookaside_t;
#else
			typedef skl::LookAsideSizePool<NonPagedPool, 4, 20, 24> lookaside_t;
#endif // _WIN64
			//lookaside_t* const poolLookAside;

			lookaside_t poolLookAside;
		private:
		};
		extern GlobleRuntime* gInstanceGlobleRuntime;
	}
}

