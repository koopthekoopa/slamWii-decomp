#ifndef PRIVATE_HOLLYWOOD_FLIPPER_H
#define PRIVATE_HOLLYWOOD_FLIPPER_H

#include <revolution/types.h>

#ifdef __MWERKS__
vu16 __VIRegs[59] AT_ADDRESS(0xCC002000);
vu32 __PIRegs[12] AT_ADDRESS(0xCC003000);
vu16 __MEMRegs[64] AT_ADDRESS(0xCC004000);
vu16 __DSPRegs[] AT_ADDRESS(0xCC005000);
vu32 __DIRegs[] AT_ADDRESS(0xCD006000);
vu32 __SIRegs[0x100] AT_ADDRESS(0xCC006400);
vu32 __EXIRegs[0x40] AT_ADDRESS(0xCD006800);
vu32 __AIRegs[8] AT_ADDRESS(0xCD006C00);
vu32 __ACRRegs[256] AT_ADDRESS(0xCD000000);
vu32 __ACRIOPRegs[256] AT_ADDRESS(0xCD800000);
#else
#define __VIRegs ((vu16*)0xCC002000)
#define __PIRegs ((vu32*)0xCC003000)
#define __MEMRegs ((vu16*)0xCC004000)
#define __DSPRegs ((vu16*)0xCC005000)
#define __DIRegs ((vu32*)0xCD006000)
#define __SIRegs ((vu32*)0xCC006400)
#define __EXIRegs ((vu32*)0xCD006800)
#define __AIRegs ((vu32*)0xCD006C00)
#define __ACRRegs ((vu32*)0xCD000000)
#define __ACRIOPRegs ((vu32*)0xCD800000)
#endif

#endif  // PRIVATE_HOLLYWOOD_FLIPPER_H
