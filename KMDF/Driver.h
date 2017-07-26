/*++

Module Name:

    driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#define INITGUID

#include <ntddk.h>
#include <wdf.h>

#include "device.h"
#include "queue.h"

//
// WDFDRIVER Events
//

extern "C" DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD KMDFEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP KMDFEvtDriverContextCleanup;

