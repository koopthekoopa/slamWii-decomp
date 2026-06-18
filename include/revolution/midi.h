#ifndef REVOLUTION_MIDI_H
#define REVOLUTION_MIDI_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define MIDI_MAX_CHAN 4

typedef void (*MIDICallback)(s32, s32);

enum {
    MIDI_RESULT_OK = 0,
    MIDI_RESULT_1,
    MIDI_RESULT_2,
};

void MIDIInit();

s32 MIDIReadAsync(s32 chan, u8* buffer, u8* status, MIDICallback callback);
s32 MIDIRead(s32 chan, u8* buffer, u8* status);

s32 MIDIGetStatusAsync(s32 chan, u8* status, MIDICallback callback);
s32 MIDIGetStatus(s32 chan, u8* status);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // REVOLUTION_MIDI_H
