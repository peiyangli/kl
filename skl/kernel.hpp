#pragma once



#ifndef _NEW_DELETE_OPERATORS_
#define _NEW_DELETE_OPERATORS_

//make them inline other than in cpp

//#pragma warning(disable: 4201)
//#pragma warning(disable: 4100)

#include <wdm.h>

#ifndef DEF_ALLOCAL_TAG
#define DEF_ALLOCAL_TAG 'IEPK'
#endif // !DEF_ALLOCAL_TAG

#ifndef DBG_TAG
#define DBG_TAG "[SKL]"
#endif // !DBG_TAG




/*****************************************************************************
* operator new()
*****************************************************************************
* Overload new to allocate from NonePagedPool, with default pooltag.
*/
//inline
void* __cdecl operator new(size_t iSize)
{
	PVOID result = ExAllocatePoolWithTag(NonPagedPool, iSize, DEF_ALLOCAL_TAG); //PagedPool
if (result) {
	RtlZeroMemory(result, iSize);
}
#if DBG
else {
	KdPrint((DBG_TAG"Couldn't new NonPagedPool: %I64d bytes", iSize));
}
#endif // DBG
return result;
}



/*****************************************************************************
* operator new
*****************************************************************************
* Overload new to allocate with default pooltag.
* Allocates from PagedPool or NonPagedPool, as specified.
*/
PVOID __cdecl operator new(size_t    iSize,
_When_((poolType & NonPagedPoolMustSucceed) != 0,
__drv_reportError("Must succeed pool allocations are forbidden. Allocation failures cause a system crash"))
POOL_TYPE poolType)
{
	PVOID result = ExAllocatePoolWithTag(poolType, iSize, DEF_ALLOCAL_TAG);
if (result) {
	RtlZeroMemory(result, iSize);
}
#if DBG
else {
	KdPrint((DBG_TAG"Couldn't new poolType(%d): %I64d bytes", (ULONG)poolType, iSize));
}
#endif // DBG

return result;
}



/*****************************************************************************
* operator new()
*****************************************************************************
* Overload new to allocate with a specified allocation tag.
* Allocates from PagedPool or NonPagedPool, as specified.
*/
PVOID __cdecl operator new(size_t      iSize,
_When_((poolType & NonPagedPoolMustSucceed) != 0,
__drv_reportError("Must succeed pool allocations are forbidden. Allocation failures cause a system crash"))
POOL_TYPE   poolType,
ULONG       tag)
{
	PVOID result = ExAllocatePoolWithTag(poolType, iSize, tag);
if (result) {
	RtlZeroMemory(result, iSize);
}
#if DBG
else {
	KdPrint((DBG_TAG"Couldn't new tagged poolType(%d): %I64d bytes", (ULONG)poolType, iSize));
}
#endif // DBG
return result;
}



/*****************************************************************************
* operator delete()
*****************************************************************************
* Delete function.
*/

#if _MSC_VER < 1900
void __cdecl operator delete(PVOID pVoid)
#else
void __cdecl operator delete(PVOID pVoid, size_t)
#endif
{
	ExFreePool(pVoid);
}

#endif //!_NEW_DELETE_OPERATORS_

