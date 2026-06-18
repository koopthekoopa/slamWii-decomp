#ifndef REVOLUTION_OS_IC_H
#define REVOLUTION_OS_IC_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void ICFlashInvalidate();
void ICEnable();
void ICDisable();
void ICFreeze();
void ICUnfreeze();
void ICBlockInvalidate(void* addr);
void ICSync();

#ifdef __cplusplus
}
#endif

#endif  // REVOLUTION_OS_IC_H
