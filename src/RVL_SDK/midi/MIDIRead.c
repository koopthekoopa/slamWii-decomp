#include <revolution/midi.h>

#include <private/midi.h>

#include <string.h>

void ReadProc(s32 chan) {
    __MIDIControl* midi = &__MIDI[chan];

    if (midi->result == MIDI_RESULT_OK) {
        memcpy(midi->outBuffer, &midi->transferInput, sizeof(midi->transferInput) - 1);
        *midi->outStatus = (u8)(midi->transferInput.unk_0x04 & 0x3A);
    }
}

s32 MIDIReadAsync(s32 chan, u8* buffer, u8* status, MIDICallback callback) {
    __MIDIControl* midi = &__MIDI[chan];

    if (midi->callback) {
        return MIDI_RESULT_2;
    }
    midi->transferOutput.unk_0x00 = 20;
    midi->outBuffer = (__MIDIStruct0*)buffer;
    midi->outStatus = (u8*)status;
    midi->callback = callback;
    return __MIDITransfer(chan, 1, 5, ReadProc);
}

s32 MIDIRead(s32 chan, u8* buffer, u8* status) {
    __MIDIControl* midi = &__MIDI[chan];

    s32 result = MIDIReadAsync(chan, buffer, status, __MIDISyncCallback);

    if (result != MIDI_RESULT_OK) {
        return result;
    }
    return __MIDISync(chan);
}
