#include <revolution/wenc.h>

#include <string.h>

typedef struct {
    s32 nXN;        // 0x00
    s32 nDL;        // 0x04
    s32 nQN;        // 0x08
    s32 nDN;        // 0x0C
    s32 nDLH;       // 0x10
    s32 nDLQ;       // 0x14
    u8 padding[8];  // 0x18
} WENCDataInteral;

s32 WENCGetEncodeData(WENCData* info, u32 flag, const s16* pbyPcmData, s32 nSampleNum, u8* pbyAdpcmData) {
    f64 dTable[8] = {0.8984375, 0.8984375, 0.8984375, 0.8984375, 1.19921875, 1.59765625, 2, 2.3984375};
    u8 by;
    u8* pDst;
    s16* pSrc;
    s32 ii;
    s32 nDA;
    s32 nL3, nL2, nL1, nL0;
    s32 nDLX;
    s32 nXNC;
    s32 nOffset;
    s32 nIndex;
    s32 nEncodeSize;
    s32 nXN, nDL, nQN, nDN;
    s32 nDLH, nDLQ;
    WENCDataInteral* block = (WENCDataInteral*)info;

    nEncodeSize = (nSampleNum + 1) / 2;
    memset(pbyAdpcmData, 0, nEncodeSize);

    pSrc = (s16*)pbyPcmData;
    pDst = (u8*)pbyAdpcmData;

    if (((u32)flag & 1) == 0) {
        nXN = 0;
        nDL = 127;
        nQN = 0;
        nDN = 0;
        nDLH = 0;
        nDLQ = 0;
    } else {
        nXN = block->nXN;
        nDL = block->nDL;
        nQN = block->nQN;
        nDN = block->nDN;
        nDLH = block->nDLH;
        nDLQ = block->nDLQ;
    }

    for (ii = 0; ii < nSampleNum; ii++) {
        nL3 = nL2 = nL1 = nL0 = 0;

        nDA = *pSrc++;
        if (nDA < nXN) {
            nL3 = 1;
        }

        nDN = __abs(nDA - nXN);
        if (nDN >= nDL) {
            nL2 = 1;
            nDN -= nDL;
        }

        nDLH = nDL / 2;
        if (nDN >= nDLH) {
            nL1 = 1;
            nDN -= nDLH;
        }

        nDLQ = nDLH / 2;
        if (nDN >= nDLQ) {
            nL0 = 1;
            nDN -= nDLQ;
        }

        nDLX = nDLQ / 2;
        nQN = (1 - (nL3 * 2)) * ((nDL * nL2) + (nDLH * nL1) + (nDLQ * nL0) + nDLX);
        if (nQN > 0xFFFF) {
            nQN = 0xFFFF;
        }
        if (nQN < -0x10000) {
            nQN = -0x10000;
        }

        nXNC = nXN + nQN;
        if (nXNC > 0x7FFF) {
            nXNC = 0x7FFF;
        }
        if (nXNC < -0x8000) {
            nXNC = -0x8000;
        }
        nXN = nXNC;

        nOffset = (ii & 1) == 0 ? 4 : 0;
        by = (nL3 * 8) + (nL2 * 4) + (nL1 * 2) + nL0;
        pDst[ii / 2] |= by << nOffset;

        nIndex = (nL2 * 4) + (nL1 * 2) + nL0;

        nDL *= dTable[nIndex];
        if (nDL <= 0x7F) {
            nDL = 0x7F;
        }
        if (nDL >= 0x6000) {
            nDL = 0x6000;
        }
    }

    block->nXN = nXN;
    block->nDL = nDL;
    block->nQN = nQN;
    block->nDN = nDN;
    block->nDLH = nDLH;
    block->nDLQ = nDLQ;

    return nSampleNum;
}
