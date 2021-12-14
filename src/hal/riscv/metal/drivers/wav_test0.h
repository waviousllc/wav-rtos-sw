/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */
/* Modified by Wavious LLC. */

#ifndef METAL__DRIVERS__WAV_TEST0_H
#define METAL__DRIVERS__WAV_TEST0_H

#include <metal/compiler.h>
#include <metal/shutdown.h>

struct __metal_driver_vtable_wav_test0 {
    const struct __metal_shutdown_vtable shutdown;
};

__METAL_DECLARE_VTABLE(__metal_driver_vtable_wav_test0)

struct __metal_driver_wav_test0 {
    struct __metal_shutdown shutdown;
};

#endif /* METAL__DRIVERS__WAV_TEST0_H */
