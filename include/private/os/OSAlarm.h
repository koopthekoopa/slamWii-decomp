#ifndef PRIVATE_OS_ALARM_H
#define PRIVATE_OS_ALARM_H

#include <revolution/os/OSAlarm.h>
#include <revolution/os/OSThread.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

void __OSSetInternalAlarmUserData(OSAlarm* alarm, void* userdata);
void __OSCancelInternalAlarms(OSThread* thread);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // PRIVATE_OS_ALARM_H
