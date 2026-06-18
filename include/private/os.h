#ifndef PRIVATE_OS_H
#define PRIVATE_OS_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <private/os/OSAlarm.h>
#include <private/os/OSBootInfo.h>
#include <private/os/OSException.h>
#include <private/os/OSMutex.h>
#include <private/os/OSThread.h>
#include <private/os/OSTime.h>

u32 __OSPhysicalMem1Size AT_ADDRESS(OS_BASE_CACHED | 0x3100);
u32 __OSSimulatedMem1Size AT_ADDRESS(OS_BASE_CACHED | 0x3104);

u32 __OSPhysicalMem2Size AT_ADDRESS(OS_BASE_CACHED | 0x3118);
u32 __OSSimulatedMem2Size AT_ADDRESS(OS_BASE_CACHED | 0x311C);

extern BOOL __OSInIPL;

extern char* __OSExceptionNames[17];

void __OSPSInit();
void __OSFPRInit();
void __OSCacheInit();

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // PRIVATE_OS_H
