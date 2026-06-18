#ifndef PRIVATE_OS_MUTEX_H
#define PRIVATE_OS_MUTEX_H

#include <revolution/os/OSMutex.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

void __OSUnlockAllMutex(OSThread* thread);

int __OSCheckDeadLock(OSThread* thread);
int __OSCheckMutexes(OSThread* thread);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // PRIVATE_OS_MUTEX_H
