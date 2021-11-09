/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */
/* Modified by Wavious, LLC. */

#include <metal/machine/platform.h>

#ifdef METAL_WAV_TEST0

#include <metal/machine.h>

#include <stdint.h>

#include <metal/drivers/wav_test0.h>
#include <metal/io.h>

void __metal_driver_wav_test0_exit(const struct __metal_shutdown *sd,
                                      int code) __attribute__((noreturn));
void __metal_driver_wav_test0_exit(const struct __metal_shutdown *sd,
                                      int code) {
    long base = __metal_driver_wav_test0_base(sd);
    uint32_t out = (code << 16) + 0x1;
    while (1) {
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_WAV_TEST0_FINISHER_OFFSET)) = out;
    }
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_wav_test0) = {
    .shutdown.exit = &__metal_driver_wav_test0_exit,
};
#endif /* METAL_WAV_TEST0 */
