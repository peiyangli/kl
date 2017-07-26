#include "kernel.hpp"



#define DEF_ALLOCAL_TAG 'IEPK'
#define DBG_TAG "[SKL]"

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

PVOID __cdecl operator new(size_t    iSize,
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

PVOID __cdecl operator new(size_t      iSize,
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

void __cdecl operator delete(PVOID pVoid)
{
	ExFreePool(pVoid);
}