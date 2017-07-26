#pragma once

#include <ntifs.h>

#include "globle_runtime.h"

namespace skl {

	namespace runtime{
		struct Uint64Item
		{
			UINT64 _64;
		};
		typedef Uint64Item *PUint64Item;

		struct Uint64ItemAVL
		{
			_RTL_BALANCED_LINKS reserved1;
			Uint64Item _reserved2;
		};
		typedef Uint64ItemAVL *PUint64ItemAVL;
	}

	class Uint64GenericTable
	{
		RTL_AVL_TABLE _root;

		static RTL_GENERIC_COMPARE_RESULTS
			NTAPI
			Compare(
				__in struct _RTL_AVL_TABLE *Table,
				__in PVOID FirstStruct,
				__in PVOID SecondStruct // 已存在的项
			)
		{
			UNREFERENCED_PARAMETER(Table);
			auto f = (runtime::PUint64Item)FirstStruct;
			auto s = (runtime::PUint64Item)SecondStruct;
			//GenericEqual GenericGreaterThan GenericLessThan
			return (f->_64 == s->_64) ? GenericEqual : ((f->_64 > s->_64) ? GenericGreaterThan : GenericLessThan);
		}
		static PVOID
			NTAPI
			Allocate(
				__in struct _RTL_AVL_TABLE *Table,
				__in CLONG ByteSize
			)
		{
			UNREFERENCED_PARAMETER(Table);

			//ByteSize == sizeof(_RTL_BALANCED_LINKS) + sizeof(runtime::Uint64Item);

			ASSERT(ByteSize == sizeof(runtime::Uint64ItemAVL));

			return runtime::gInstanceGlobleRuntime->poolLookAside.Malloc<runtime::Uint64ItemAVL>();
			//PVOID ptr = ExAllocatePoolWithTag(NonPagedPool, ByteSize, 'avla');
			//return ptr;
		}
		static void
			NTAPI
			Free(
				__in struct _RTL_AVL_TABLE *Table,
				__in __drv_freesMem(Mem) __post_invalid PVOID Buffer
			)
		{
			UNREFERENCED_PARAMETER(Table);
			//ExFreePool(Buffer);
			RTL_BALANCED_LINKS;
			runtime::gInstanceGlobleRuntime->poolLookAside.Free<runtime::Uint64ItemAVL>((runtime::Uint64ItemAVL*)Buffer);
		}

		Uint64GenericTable(const Uint64GenericTable&);
		Uint64GenericTable& operator = (const Uint64GenericTable&);
	public:
		Uint64GenericTable() {
			RtlInitializeGenericTableAvl(&_root, &Uint64GenericTable::Compare, &Uint64GenericTable::Allocate, &Uint64GenericTable::Free, NULL);
		}
		~Uint64GenericTable() {
			clear();
		}

		void clear() {
			for (runtime::PUint64Item p = (runtime::PUint64Item)RtlEnumerateGenericTableAvl(&_root, TRUE);
				p != NULL;
				p = (runtime::PUint64Item)RtlEnumerateGenericTableAvl(&_root, FALSE))
			{
				RtlDeleteElementGenericTableAvl(&_root, p); // 会调用Compare！
			}
		}

		runtime::PUint64Item find(UINT64 val) {
			runtime::Uint64Item temp;
			temp._64 = val;
			return (runtime::PUint64Item)RtlLookupElementGenericTableAvl(&_root, (PVOID)(&temp));
		}

		ULONG count() {
			return RtlNumberGenericTableElementsAvl(&_root);
		}
		BOOLEAN empty() {
			return RtlIsGenericTableEmptyAvl(&_root);
		}
		enum eInsertResult {
			IR_Error = -1,
			IR_Ok = 0,
			IR_Existed
		};
		eInsertResult insert(UINT64 val) {
			runtime::Uint64Item pt;
			pt._64 = val;
			BOOLEAN newElement = FALSE;
			runtime::PUint64Item pItem = (runtime::PUint64Item)RtlInsertElementGenericTableAvl(&_root,
				&pt,
				sizeof(runtime::Uint64Item),
				&newElement);
			if (pItem) {
				if (newElement) {
					return IR_Ok;
				}
				return IR_Existed;
			}
			return IR_Error;
		}

		BOOLEAN remove(UINT64 val) {
			runtime::Uint64Item temp;
			temp._64 = val;
			auto rt = RtlDeleteElementGenericTableAvl(&_root, (PVOID)(&temp));

			return rt;
		}

		//Should be set to NULL if the enumeration is to start at the first element in the table.
		runtime::PUint64Item Enum(__inout PVOID *RestartKey) {
			return (runtime::PUint64Item)RtlEnumerateGenericTableWithoutSplayingAvl(&_root, RestartKey);
		}
	};



}//namespace skl


//////////////////////////////////////////////////////////////////////////