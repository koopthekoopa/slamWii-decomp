#ifndef PRIVATE_MIDI_H
#define PRIVATE_MIDI_H

#include <revolution/midi.h>

#include <revolution/os/OSThread.h>
#include <revolution/os/OSTime.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef void (*__MIDITransferCallback)(s32);

// there is a lot of unused junk.
// what are they doing?

typedef struct {
    u8 unk_0x00;
    u8 unk_0x01;
    u8 unk_0x02;
    u8 unk_0x03;
    u8 unk_0x04;
} __MIDIStruct0;

typedef struct {
    __MIDIStruct0 transferOutput;  // 0x00
    __MIDIStruct0 transferInput;   // 0x05
    u8 unk_0x0A;
    u8 unk_0x0B;
    u32 transferOutputLen;      // 0x0C
    u32 transferInputLen;       // 0x10
    u8* outStatus;              // 0x14
    __MIDIStruct0* outBuffer;   // 0x18
    MIDICallback callback;      // 0x1C
    u32 result;                 // 0x20
    OSThreadQueue threadQueue;  // 0x24
    u32 unk_0x2C;
    OSTime delayTime;                         // 0x30
    __MIDITransferCallback transferCallback;  // 0x38
    u8 unused_0x3C[0x6C];
    u32* param;  // 0xA8
    u32 unused_0xAC;
} __MIDIControl;

extern __MIDIControl __MIDI[MIDI_MAX_CHAN];
extern BOOL __MIDIReset;

void __MIDISyncCallback(s32 type, s32 arg);
s32 __MIDISync(s32 chan);

s32 __MIDITransfer(s32 chan, u32 outLen, u32 inLen, __MIDITransferCallback transferCallback);

OSTime __MIDISetDelay(s32 chan, OSTime delay);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // PRIVATE_MIDI_H
