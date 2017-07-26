#pragma once

//make them inline other than in cpp

//#pragma warning(disable: 4201)
//#pragma warning(disable: 4100)

//#include <wdm.h>

#include<ntifs.h>

#define DEF_ALLOCAL_TAG 'IEPK'
#define DBG_TAG "[SKL]"
/*****************************************************************************
* operator new()
*****************************************************************************
* Overload new to allocate from NonePagedPool, with default pooltag.
*/
inline
void* __cdecl operator new(size_t iSize)
{
	PVOID result = ExAllocatePoolWithTag(NonPagedPool, iSize, DEF_ALLOCAL_TAG); //PagedPool
	if (result) {
		RtlZeroMemory(result, iSize);
	}
#if DBG
	else {
		KdPrint((DBG_TAG"Couldn't new NonPagedPool: %I64d bytes\n", iSize));
	}
#endif // DBG
	return result;
}

inline
void* __cdecl operator new[](size_t iSize)
{
	PVOID result = ExAllocatePoolWithTag(NonPagedPool, iSize, DEF_ALLOCAL_TAG); //PagedPool
if (result) {
	RtlZeroMemory(result, iSize);
}
#if DBG
else {
	KdPrint((DBG_TAG"Couldn't new NonPagedPool: %I64d bytes\n", iSize));
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
inline
PVOID operator new(size_t    iSize,
POOL_TYPE poolType)
{
	PVOID result = ExAllocatePoolWithTag(poolType, iSize, DEF_ALLOCAL_TAG);
	if (result) {
		RtlZeroMemory(result, iSize);
	}
#if DBG
	else {
		KdPrint((DBG_TAG"Couldn't new poolType(%d): %I64d bytes\n", (ULONG)poolType, iSize));
	}
#endif // DBG

	return result;
}


inline
PVOID operator new[](size_t    iSize,
	POOL_TYPE poolType)
{
	PVOID result = ExAllocatePoolWithTag(poolType, iSize, DEF_ALLOCAL_TAG);
if (result) {
	RtlZeroMemory(result, iSize);
}
#if DBG
else {
	KdPrint((DBG_TAG"Couldn't new poolType(%d): %I64d bytes\n", (ULONG)poolType, iSize));
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
inline
PVOID operator new(size_t      iSize,
POOL_TYPE   poolType,
ULONG       tag)
{
	PVOID result = ExAllocatePoolWithTag(poolType, iSize, tag);
	if (result) {
		RtlZeroMemory(result, iSize);
	}
#if DBG
	else {
		KdPrint((DBG_TAG"Couldn't new tagged poolType(%d): %I64d bytes\n", (ULONG)poolType, iSize));
	}
#endif // DBG
	return result;
}

inline
PVOID operator new[](size_t      iSize,
	POOL_TYPE   poolType,
	ULONG       tag)
{
	PVOID result = ExAllocatePoolWithTag(poolType, iSize, tag);
if (result) {
	RtlZeroMemory(result, iSize);
}
#if DBG
else {
	KdPrint((DBG_TAG"Couldn't new tagged poolType(%d): %I64d bytes\n", (ULONG)poolType, iSize));
}
#endif // DBG
return result;
}

/*****************************************************************************
* operator delete()
*****************************************************************************
* Delete function.
*/
inline
void __cdecl operator delete(PVOID pVoid)
{
	ExFreePool(pVoid);
}

//vs 15
inline
void __cdecl operator delete(PVOID pVoid, size_t)
{
	ExFreePool(pVoid);
}


inline 
void __cdecl operator delete[](void * pVoid)
{
	ExFreePool(pVoid);
}

inline
void __cdecl operator delete[](PVOID pVoid, size_t)
{
	ExFreePool(pVoid);
}

//placement
inline
void*__cdecl operator new(size_t, void *_Where)
{	// construct array with placement at _Where
	return (_Where);
}
inline
void __cdecl operator delete(void *, void *)
{	// delete if placement new fails
}

///
namespace skl{

	namespace runtime{
		//call just in driverentry
		NTSTATUS RunTimeInit();

		//call when exit/unload driver
		NTSTATUS RunTimeUnInit();
	}
}

