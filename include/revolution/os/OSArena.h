#ifndef REVOLUTION_OS_ARENA_H
#define REVOLUTION_OS_ARENA_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/* MEM1 */

void* OSGetMEM1ArenaHi();
void* OSGetMEM1ArenaLo();
void OSSetMEM1ArenaHi(void* newHi);
void OSSetMEM1ArenaLo(void* newLo);
void* OSAllocFromMEM1ArenaLo(u32 size, u32 align);
void* OSAllocFromMEM1ArenaHi(u32 size, u32 align);

/* MEM2 */

void* OSGetMEM2ArenaHi();
void* OSGetMEM2ArenaLo();
void OSSetMEM2ArenaHi(void* newHi);
void OSSetMEM2ArenaLo(void* newLo);
void* OSAllocFromMEM2ArenaLo(u32 size, u32 align);
void* OSAllocFromMEM2ArenaHi(u32 size, u32 align);

/* Dolphin SDK compatibility (MEM1) */

void* OSGetArenaHi();
void* OSGetArenaLo();
void OSSetArenaHi(void* newHi);
void OSSetArenaLo(void* newLo);
#define OSAllocFromArenaLo OSAllocFromMEM1ArenaLo
#define OSAllocFromArenaHi OSAllocFromMEM1ArenaHi

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // REVOLUTION_OS_ARENA_H
