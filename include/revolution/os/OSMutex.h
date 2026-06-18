#ifndef REVOLUTION_OS_MUTEX_H
#define REVOLUTION_OS_MUTEX_H

#include <revolution/types.h>

#include <revolution/os/OSThread.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

struct OSMutex {
    OSThreadQueue queue;  // 0x00
    OSThread* thread;     // 0x08
    s32 count;            // 0x0C
    OSMutexLink link;     // 0x10
};

typedef struct OSCond {
    OSThreadQueue queue;
} OSCond;

void OSInitMutex(OSMutex* mutex);

void OSLockMutex(OSMutex* mutex);
void OSUnlockMutex(OSMutex* mutex);
BOOL OSTryLockMutex(OSMutex* mutex);

void OSInitCond(OSCond* cond);
void OSWaitCond(OSCond* cond, OSMutex* mutex);
void OSSignalCond(OSCond* cond);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // REVOLUTION_OS_MUTEX_H
