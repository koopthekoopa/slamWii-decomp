#ifndef REVOLUTION_OS_DC_H
#define REVOLUTION_OS_DC_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void DCFlashInvalidate();
void DCEnable();
void DCDisable();
void DCFreeze();
void DCUnfreeze();
void DCTouchLoad(void* addr);
void DCBlockZero(void* addr);
void DCBlockStore(void* addr);
void DCBlockFlush(void* addr);
void DCBlockInvalidate(void* addr);

#ifdef __cplusplus
}
#endif

#endif  // REVOLUTION_OS_DC_H
