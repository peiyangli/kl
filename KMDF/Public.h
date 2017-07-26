/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that app can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_KMDF,
    0x0a0d7dbf,0xc6c1,0x45a5,0xa3,0x4b,0x99,0x3d,0x2e,0xea,0xe4,0xf2);
// {0a0d7dbf-c6c1-45a5-a34b-993d2eeae4f2}


#ifndef __cplusplus
#define ALLOC_TEXT(x, y) alloc_text(x, y)
#else
#define ALLOC_TEXT(x, y)  
#endif // __cplusplus
