#include "globle_runtime.h"

#include"lookaside_atomic.hpp"

#include "unit64table.hpp"
#include "list.hpp"

namespace skl{
	namespace runtime{

//#pragma warning(disable: 4316) // while (false);

		GlobleRuntime::GlobleRuntime() //: poolLookAside(new lookaside_t) //pagedPoolLookAside(new pagedlookaside_t),
		{
		}

		GlobleRuntime::~GlobleRuntime()
		{
		}
	}
}
