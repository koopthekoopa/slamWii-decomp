#include <revolution/arc.h>

#include <revolution/os.h>

#include <ctype.h>

#define ARC_MAGIC 0x55AA382D

typedef struct FSTEntry {
    unsigned int isDirAndStringOff;  // 0x00
    unsigned int parentOrPosition;   // 0x04
    unsigned int nextEntryOrLength;  // 0x08
} FSTEntry;

#define FST_ENTRIES ((u8*)arcStart + arcHeader->fstStart)
#define FILE_START ((u8*)arcStart + arcHeader->fileStart)

#define FST_IS_DIR(fstEntry) (!((fstEntry).isDirAndStringOff & 0xFF000000) ? FALSE : TRUE)
#define FST_STRING_OFF(fstEntry) ((fstEntry).isDirAndStringOff & 0x00FFFFFF)

#define FST_PARENT(fstEntry) ((fstEntry).parentOrPosition)
#define FST_POSITION(fstEntry) ((fstEntry).parentOrPosition)

#define FST_NEXT_ENTRY(fstEntry) ((fstEntry).nextEntryOrLength)
#define FST_LENGTH(fstEntry) ((fstEntry).nextEntryOrLength)

BOOL ARCInitHandle(void* arcStart, ARCHandle* handle) {
    FSTEntry* FSTEntries;
    ARCHeader* arcHeader = (ARCHeader*)arcStart;

    if (arcHeader->magic != ARC_MAGIC) {
        OSHaltLine(74, "ARCInitHandle: bad archive format");
    }

    handle->archiveStartAddr = arcStart;

    FSTEntries = (FSTEntry*)FST_ENTRIES;
    handle->FSTStart = FSTEntries;
    handle->fileStart = FILE_START;
    ASSERTMSGLINE(81, FSTEntries != NULL, "ARCInitHandle: bad archive format");  // ok then, debug assert

    handle->entryNum = FSTEntries->nextEntryOrLength;

    handle->FSTStringStart = (char*)&FSTEntries[handle->entryNum];
    handle->FSTLength = arcHeader->fstSize;

    handle->currDir = 0;

    return TRUE;
}

BOOL ARCOpen(ARCHandle* handle, char* fileName, ARCFileInfo* af) {
    s32 entry;
    char currentDir[ARC_MAX_PATH];
    FSTEntry* FSTEntries;

    ASSERTMSGLINE(97, handle, "ARCOpen(): NULL pointer is specified to ARCHandle structure");
    ASSERTMSGLINE(98, fileName, "ARCOpen(): NULL pointer is specified to fileName");
    ASSERTMSGLINE(99, af, "ARCOpen(): NULL pointer is specified to ARCFileInfo structure");

    FSTEntries = (FSTEntry*)handle->FSTStart;
    entry = ARCConvertPathToEntrynum(handle, fileName);

    if (entry < 0) {
        ARCGetCurrentDir(handle, currentDir, sizeof(currentDir));
        OSReport("Warning: ARCOpen(): file '%s' was not found under %s in the archive.\n", fileName, &currentDir);
        return FALSE;
    }

    ASSERTMSGLINE(115, !FST_IS_DIR(FSTEntries[entry]), "ARCOpen(): %s is a directory", fileName);

    if (entry < 0 || FST_IS_DIR(FSTEntries[entry])) {
        return FALSE;
    }

    af->handle = handle;
    af->startOffset = FST_POSITION(FSTEntries[entry]);
    af->length = FST_LENGTH(FSTEntries[entry]);

    return TRUE;
}

BOOL ARCFastOpen(ARCHandle* handle, s32 entrynum, ARCFileInfo* af) {
    FSTEntry* FSTEntries;

    ASSERTMSGLINE(133, handle, "ARCFastOpen(): null pointer is specified to ARCHandle address  ");
    ASSERTMSGLINE(135, af, "ARCFastOpen(): null pointer is specified to ARCFileInfo address  ");
    ASSERTMSGLINE(138, entrynum >= 0 && entrynum < handle->entryNum, "ARCFastOpen(): specified entry number '%d' is out of range  ", entrynum);

    FSTEntries = (FSTEntry*)handle->FSTStart;

    ASSERTMSGLINE(144, !FST_IS_DIR(FSTEntries[entrynum]), "ARCFastOpen(): entry number '%d' is assigned to a directory  ", entrynum);

    if (entrynum < 0 || entrynum >= handle->entryNum || FST_IS_DIR(FSTEntries[entrynum])) {
        return FALSE;
    }

    af->handle = handle;
    af->startOffset = FST_POSITION(FSTEntries[entrynum]);
    af->length = FST_LENGTH(FSTEntries[entrynum]);

    return TRUE;
}

static BOOL isSame(char* path, char* string) {
    while (*string != 0) {
        if (tolower(*path++) != tolower(*string++)) {
            return FALSE;
        }
    }

    if (*path == '/' || *path == 0) {
        return TRUE;
    }
    return FALSE;
}

s32 ARCConvertPathToEntrynum(ARCHandle* handle, char* pathPtr) {
    char* ptr;
    char* stringPtr;
    BOOL isDir;
    s32 length;
    u32 dirLookAt;
    u32 i;
    char* origPathPtr = pathPtr;  // unused
    FSTEntry* FSTEntries;

    ASSERTMSGLINE(202, handle, "ARCConvertPathToEntrynum(): null pointer is specified to ARCHandle structure");
    ASSERTMSGLINE(203, pathPtr, "ARCConvertPathToEntrynum(): null pointer is specified to file name");

    dirLookAt = handle->currDir;
    FSTEntries = handle->FSTStart;

    while (TRUE) {
        if (*pathPtr == 0) {
            return dirLookAt;
        }
        if (*pathPtr == '/') {
            dirLookAt = 0;
            pathPtr++;
            continue;
        }
        if (*pathPtr == '.') {
            if (pathPtr[1] == '.') {
                if (pathPtr[2] == '/') {
                    dirLookAt = FST_PARENT(FSTEntries[dirLookAt]);
                    pathPtr += 3;
                    continue;
                }
                if (pathPtr[2] == 0) {
                    return FST_PARENT(FSTEntries[dirLookAt]);
                }
            } else {
                if (pathPtr[1] == '/') {
                    pathPtr += 2;
                    continue;
                }
                if (pathPtr[1] == 0) {
                    return dirLookAt;
                }
            }
        }

        ptr = pathPtr;
        while (*ptr != 0 && *ptr != '/') {
            ptr++;
        }

        isDir = *ptr == 0 ? FALSE : TRUE;
        length = ptr - pathPtr;
        ptr = pathPtr;

        for (i = dirLookAt + 1; i < FST_LENGTH(FSTEntries[dirLookAt]); i = FST_IS_DIR(FSTEntries[i]) ? FST_NEXT_ENTRY(FSTEntries[i]) : i + 1) {
            while (TRUE) {
                if (!FST_IS_DIR(FSTEntries[i]) && isDir == TRUE) {
                    break;
                }
                stringPtr = &handle->FSTStringStart[FST_STRING_OFF(FSTEntries[i])];
                if (stringPtr[0] == '.' && stringPtr[1] == 0) {
                    i++;
                    continue;
                }
                if (isSame(ptr, stringPtr) == TRUE) {
                    goto found;
                }

                break;
            }
        }
        return -1;

    found:
        if (isDir == FALSE) {
            return i;
        } else {
            dirLookAt = i;
            pathPtr += length + 1;
        }
    }

    return -1;
}

BOOL ARCEntrynumIsDir(ARCHandle* handle, s32 entrynum) {
    FSTEntry* FSTEntries;

    ASSERTMSGLINE(307, handle, "ARCEntrynumIsDir(): null pointer is specified to ARCHandle structure");
    ASSERTMSGLINE(308, entrynum >= 0, "ARCEntrynumIsDir(): no file/directory is specified to entrynum");

    FSTEntries = handle->FSTStart;

    return FST_IS_DIR(FSTEntries[entrynum]);
}

static u32 myStrncpy(char* dest, char* src, u32 maxlen) {
    u32 i = maxlen;

    while (i != 0 && *src != 0) {
        *dest++ = *src++;
        i--;
    }
    return maxlen - i;
}

static u32 entryToPath(ARCHandle* handle, u32 entry, char* path, u32 maxlen) {
    char* name;
    u32 loc;
    FSTEntry* FSTEntries = handle->FSTStart;

    if (entry == 0) {
        return 0;
    }

    name = &handle->FSTStringStart[FST_STRING_OFF(FSTEntries[entry])];
    loc = entryToPath(handle, FST_PARENT(FSTEntries[entry]), path, maxlen);
    if (loc == maxlen) {
        return loc;
    }

    path[loc++] = '/';
    loc += myStrncpy(&path[loc], name, maxlen - loc);
    return loc;
}

static BOOL ARCConvertEntrynumToPath(ARCHandle* handle, s32 entrynum, char* path, u32 maxlen) {
    u32 loc;
    FSTEntry* FSTEntries;

    FSTEntries = handle->FSTStart;

    ASSERTMSGLINE(404, entrynum >= 0 && entrynum < handle->entryNum, "ARCConvertEntrynumToPath: specified entrynum(%d) is out of range  ", entrynum);
    ASSERTMSGLINE(406, maxlen > 1, "ARCConvertEntrynumToPath: maxlen should be more than 1 (%d is specified)", maxlen);
    ASSERTMSGLINE(411, FST_IS_DIR(FSTEntries[entrynum]), "ARCConvertEntrynumToPath: cannot convert an entry num for a file to path  ");

    loc = entryToPath(handle, (u32)entrynum, path, maxlen);
    if (loc == maxlen) {
        path[maxlen - 1] = 0;
        return FALSE;
    }

    if (FST_IS_DIR(FSTEntries[entrynum])) {
        if (loc == (u32)(maxlen - 1)) {
            path[loc] = 0;
            return FALSE;
        }
        path[loc++] = '/';
    }
    path[loc] = 0;

    return TRUE;
}

BOOL ARCGetCurrentDir(ARCHandle* handle, char* path, u32 maxlen) {
    ASSERTMSGLINE(453, maxlen > 1, "ARCGetCurrentDir: maxlen should be more than 1 (%d is specified)", maxlen);

    return ARCConvertEntrynumToPath(handle, handle->currDir, path, maxlen);
}

void* ARCGetStartAddrInMem(ARCFileInfo* af) {
    ARCHandle* handle = af->handle;

    // "ARCGetFileAddr"
    ASSERTMSGLINE(464, handle, "ARCGetFileAddr(): af->handle is null pointer. Maybe it's not initialized properly");
    ASSERTMSGLINE(465, af, "ARCGetFileAddr(): null pointer is specified to ARCFileInfo structure");

    return ((u8*)handle->archiveStartAddr + af->startOffset);
}

u32 ARCGetStartOffset(ARCFileInfo* af) {
    return af->startOffset;
}

u32 ARCGetLength(ARCFileInfo* af) {
    return af->length;
}

BOOL ARCClose() {
    // we aren't closing anything soo
    return TRUE;
}

BOOL ARCChangeDir(ARCHandle* handle, char* dirName) {
    s32 entry;
    char currentDir[ARC_MAX_PATH];
    FSTEntry* FSTEntries;

    ASSERTMSGLINE(496, handle, "ARCChangeDir(): null pointer is specified to ARCHandle");
    ASSERTMSGLINE(497, dirName, "ARCChangeDir(): null pointer is specified to dirname");

    entry = ARCConvertPathToEntrynum(handle, dirName);
    FSTEntries = handle->FSTStart;

#ifdef DEBUG
    if (entry < 0) {
        ARCGetCurrentDir(handle, currentDir, sizeof(currentDir));
        OSPanic(__FILE__, 506, "ARCOpendir(): directory '%s' is not found under %s  ", dirName, currentDir);
    }
#endif

    ASSERTMSGLINE(510, FST_IS_DIR(FSTEntries[entry]), "ARCChangeDir(): %s is not a directory", dirName);

    if (entry < 0 || !FST_IS_DIR(FSTEntries[entry])) {
        return FALSE;
    }

    handle->currDir = entry;
    return TRUE;
}

BOOL ARCOpenDir(ARCHandle* handle, char* dirName, ARCDir* dir) {
    s32 entry;
    char currentDir[ARC_MAX_PATH];
    FSTEntry* FSTEntries;

    ASSERTMSGLINE(531, handle, "ARCOpenDir(): null pointer is specified to ARCHandle");
    ASSERTMSGLINE(532, dirName, "ARCOpenDir(): null pointer is specified to ARCDir");  // @typo: its dirName
    // in fact, there's no assert for ARCDir

    entry = ARCConvertPathToEntrynum(handle, dirName);
    FSTEntries = handle->FSTStart;

#ifdef DEBUG
    if (entry < 0) {
        ARCGetCurrentDir(handle, currentDir, sizeof(currentDir));
        OSPanic(__FILE__, 541, "ARCOpendir(): directory '%s' is not found under %s  ", dirName, currentDir);
    }
#endif

    ASSERTMSGLINE(548, FST_IS_DIR(FSTEntries[entry]), "ARCOpenDir(): %s is a regular file", dirName);

    if (entry < 0 || !FST_IS_DIR(FSTEntries[entry])) {
        return FALSE;
    }

    dir->handle = handle;
    dir->entryNum = entry;
    dir->location = entry + 1;
    dir->next = FST_NEXT_ENTRY(FSTEntries[entry]);
    return TRUE;
}

BOOL ARCReadDir(ARCDir* dir, ARCDirEntry* dirent) {
    u32 loc;
    FSTEntry* FSTEntries;
    ARCHandle* handle;

    handle = dir->handle;
    ASSERTMSGLINE(569, handle, "ARCReadDir: dir->handle is null pointer. Maybe it's not initialized properly");

    FSTEntries = handle->FSTStart;

    loc = dir->location;

    while (TRUE) {
        if (loc <= dir->entryNum || dir->next <= loc) {
            return FALSE;
        }
        dirent->handle = handle;
        dirent->entryNum = loc;
        dirent->isDir = FST_IS_DIR(FSTEntries[loc]);
        dirent->name = &handle->FSTStringStart[FST_STRING_OFF(FSTEntries[loc])];
        if (dirent->name[0] == '.' && dirent->name[1] == 0) {
            loc++;
            continue;
        }
        break;
    }

    dir->location = FST_IS_DIR(FSTEntries[loc]) ? FSTEntries[loc].nextEntryOrLength : loc + 1;
    return TRUE;
}

BOOL ARCCloseDir() {
    return TRUE;
}
