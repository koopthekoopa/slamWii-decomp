#ifndef PRIVATE_OS_THREAD_H
#define PRIVATE_OS_THREAD_H

#include <revolution/os/OSThread.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

OSThreadQueue __OSActiveThreadQueue AT_ADDRESS(OS_BASE_CACHED | 0x00DC);
OSThread* __OSCurrentThread AT_ADDRESS(OS_BASE_CACHED | 0x00E4);
OSContext* __OSFPRContext AT_ADDRESS(OS_BASE_CACHED | 0x00D8);

void __OSThreadInit();
s32 __OSGetEffectivePriority(OSThread* thread);
void __OSPromoteThread(OSThread* thread, s32 priority);
void __OSReschedule();

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // PRIVATE_OS_THREAD_H
