#ifndef REVOLUTION_OS_LC_H
#define REVOLUTION_OS_LC_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void LCAllocOneTag(BOOL invalidate, void* tag);
void LCAllocTags(BOOL invalidate, void* startTag, u32 numBlocks);
void LCAlloc(void* addr, u32 nBytes);
void LCAllocNoInvalidate(void* addr, u32 nBytes);

#ifdef __cplusplus
}
#endif

#endif  // REVOLUTION_OS_LC_H
