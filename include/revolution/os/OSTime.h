#ifndef REVOLUTION_OS_TIME_H
#define REVOLUTION_OS_TIME_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef s64 OSTime;
typedef u32 OSTick;

typedef struct OSCalendarTime {
    int sec;   // 0x00
    int min;   // 0x04
    int hour;  // 0x08
    int mday;  // 0x0C
    int mon;   // 0x10
    int year;  // 0x14
    int wday;  // 0x18
    int yday;  // 0x1C
    int msec;  // 0x20
    int usec;  // 0x24
} OSCalendarTime;

#define USEC_MAX 1000
#define MSEC_MAX 1000
#define MONTH_MAX 12
#define WEEK_DAY_MAX 7
#define YEAR_DAY_MAX 365

#define SECS_IN_MIN 60
#define SECS_IN_HOUR (SECS_IN_MIN * 60)
#define SECS_IN_DAY (SECS_IN_HOUR * 24)
#define SECS_IN_YEAR (SECS_IN_DAY * 365)

#define BIAS 0xB2575

OSTick OSGetTick();
OSTime OSGetTime();

void OSTicksToCalendarTime(OSTime ticks, OSCalendarTime* td);
OSTime OSCalendarTimeToTicks(OSCalendarTime* td);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // REVOLUTION_OS_TIME_H
