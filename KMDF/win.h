#pragma once

#include<ntifs.h>
NTSTATUS RetrieveNameByPID(HANDLE pid, PVOID ProcessImageName, ULONG ProcessImageNameLen, PULONG ProcessImageNameLenRt);
NTSTATUS RetrieveNameByEPROCESS(PEPROCESS eProcess, PVOID ProcessImageName, ULONG ProcessImageNameLen, PULONG ProcessImageNameLenRt);