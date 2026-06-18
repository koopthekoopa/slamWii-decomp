#ifndef REVOLUTION_ARC_H
#define REVOLUTION_ARC_H

#include <revolution/types.h>

#define ARC_MAX_PATH 128

typedef struct {
    void* archiveStartAddr;  // 0x00
    void* FSTStart;          // 0x04
    void* fileStart;         // 0x08
    u32 entryNum;            // 0x0C
    char* FSTStringStart;    // 0x10
    u32 FSTLength;           // 0x14
    u32 currDir;             // 0x18
} ARCHandle;

typedef struct {
    unsigned int magic;  // 0x00
    int fstStart;        // 0x04
    int fstSize;         // 0x08
    int fileStart;       // 0x0C
    int reserve[4];      // 0x10
} ARCHeader;

typedef struct {
    ARCHandle* handle;  // 0x00
    u32 startOffset;    // 0x04
    u32 length;         // 0x08
} ARCFileInfo;

typedef struct {
    ARCHandle* handle;  // 0x00
    u32 entryNum;       // 0x04
    u32 location;       // 0x08
    u32 next;           // 0x0C
} ARCDir;

typedef struct {
    ARCHandle* handle;  // 0x00
    u32 entryNum;       // 0x04
    BOOL isDir;         // 0x08
    char* name;         // 0x0C
} ARCDirEntry;

BOOL ARCInitHandle(void* arcStart, ARCHandle* handle);

BOOL ARCOpen(ARCHandle* handle, char* fileName, ARCFileInfo* af);
BOOL ARCFastOpen(ARCHandle* handle, s32 entrynum, ARCFileInfo* af);

s32 ARCConvertPathToEntrynum(ARCHandle* handle, char* pathPtr);
BOOL ARCEntrynumIsDir(ARCHandle* handle, s32 entrynum);

BOOL ARCGetCurrentDir(ARCHandle* handle, char* path, u32 maxlen);

void* ARCGetStartAddrInMem(ARCFileInfo* af);
u32 ARCGetStartOffset(ARCFileInfo* af);

u32 ARCGetLength(ARCFileInfo* af);

BOOL ARCClose();

BOOL ARCChangeDir(ARCHandle* handle, char* dirName);
BOOL ARCOpenDir(ARCHandle* handle, char* dirName, ARCDir* dir);
BOOL ARCReadDir(ARCDir* dir, ARCDirEntry* dirent);
BOOL ARCCloseDir();

#endif  // REVOLUTION_ARC_H
