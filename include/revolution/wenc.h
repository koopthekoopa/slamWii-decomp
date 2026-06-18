#ifndef REVOLUTION_WENC_H
#define REVOLUTION_WENC_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct {
    u8 data[32];  // 0x00
} WENCData;

s32 WENCGetEncodeData(WENCData* info, u32 flag, const s16* pbyPcmData, s32 nSampleNum, u8* pbyAdpcmData);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // REVOLUTION_WENC_H
