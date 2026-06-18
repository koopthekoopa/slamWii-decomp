#ifndef PRIVATE_OS_EXCEPTION_H
#define PRIVATE_OS_EXCEPTION_H

#include <revolution/os/OSException.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

void __OSUnhandledException(__OSException exception, OSContext* context, u32 dsisr, u32 dar);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // PRIVATE_OS_EXCEPTION_H
