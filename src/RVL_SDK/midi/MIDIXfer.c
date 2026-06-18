#include <revolution/midi.h>

#include <private/midi.h>

#include <revolution/os.h>
#include <revolution/si.h>

#include <private/os.h>

static void __MIDIHandler(s32 type, u32 sr, OSContext* context) {
    __MIDIControl* midi = &__MIDI[type];

    if (!__MIDIReset) {
        if ((sr & 0xF) != 0) {
            midi->result = MIDI_RESULT_1;
        } else {
            midi->result = MIDI_RESULT_OK;
        }
        if (midi->transferCallback) {
            __MIDITransferCallback transferCallback;
            transferCallback = midi->transferCallback;
            midi->transferCallback = NULL;
            transferCallback(type);
        }
        if (midi->callback) {
            OSContext newContext;
            MIDICallback callback;
            OSClearContext(&newContext);
            OSSetCurrentContext(&newContext);
            callback = midi->callback;
            midi->callback = 0;
            callback(type, midi->result);
            OSClearContext(&newContext);
            OSSetCurrentContext(context);
        }
    }
}

void __MIDISyncCallback(s32 type, s32 arg) {
    __MIDIControl* midi = &__MIDI[type];

    OSWakeupThread(&midi->threadQueue);
}

s32 __MIDISync(s32 chan) {
    __MIDIControl* midi = &__MIDI[chan];
    s32 result;

    BOOL enabled = OSDisableInterrupts();

    while (midi->callback) {
        OSSleepThread(&midi->threadQueue);
    }
    result = midi->result;
    OSRestoreInterrupts(enabled);
    return result;
}

static void TypeAndStatusCallback(s32 chan, u32 type) {
    __MIDIControl* midi = &__MIDI[chan];

    if (!__MIDIReset) {
        ASSERTLINE(109, !(type & SI_ERROR_BUSY));
        if ((type & 0x000000FF) != 0 || (type & 0xFFFF0000) != 0x40000) {
            midi->result = MIDI_RESULT_1;
            goto block_9;
        }
        if (!SITransfer(chan, &midi->transferOutput, midi->transferOutputLen, &midi->transferInput, midi->transferInputLen, __MIDIHandler,
                        midi->delayTime)) {
            midi->result = MIDI_RESULT_2;
        block_9:
            if (midi->transferCallback) {
                __MIDITransferCallback transferCallback = midi->transferCallback;
                midi->transferCallback = NULL;
                transferCallback(chan);
            }
            if (midi->callback) {
                MIDICallback callback;
                OSContext* currentContext = OSGetCurrentContext();
                OSContext newContext;

                OSClearContext(&newContext);
                OSSetCurrentContext(&newContext);
                callback = midi->callback;
                midi->callback = NULL;
                callback(chan, midi->result);
                OSClearContext(&newContext);
                OSSetCurrentContext(currentContext);
                __OSReschedule();
            }
        }
    }
}

s32 __MIDITransfer(s32 chan, u32 outLen, u32 inLen, __MIDITransferCallback transferCallback) {
    BOOL enabled;
    __MIDIControl* midi = &__MIDI[chan];

    enabled = OSDisableInterrupts();
    midi->transferCallback = transferCallback;
    midi->transferOutputLen = outLen;
    midi->transferInputLen = inLen;
    SIGetTypeAsync(chan, TypeAndStatusCallback);
    OSRestoreInterrupts(enabled);
    return MIDI_RESULT_OK;
}

OSTime __MIDISetDelay(s32 chan, OSTime delay) {
    __MIDIControl* midi = &__MIDI[chan];

    OSTime oldDelay = midi->delayTime;
    midi->delayTime = delay;
    return oldDelay;
}
