# MCU Interrupt Configuration
if (NOT DEFINED ${CONFIG_WAV_MCU_IRQ_SYNC_CFG})
    set(CONFIG_WAV_MCU_IRQ_SYNC_CFG 0x00000000)
endif()
if (NOT DEFINED ${CONFIG_WAV_MCU_IRQ_EDGE_CFG})
    set(CONFIG_WAV_MCU_IRQ_EDGE_CFG 0x00000000)
endif()

FILE(
    GLOB_RECURSE
    BSP_METAL_SOURCES
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/metal/riscv_cpu_min.c
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/metal/wav_test0.c
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/src/*.c
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/src/*.S
)

list(
    REMOVE_ITEM
    BSP_METAL_SOURCES
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/src/drivers/riscv_cpu.c
)

FILE(
    GLOB
    BSP_FREERTOS_SOURCES
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/*.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/GCC/RISC-V/*.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/GCC/RISC-V/*.S
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/MemMang/heap_${RTOS_HEAP_ALLOC_SCHEME}.c
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/freertos/Bridge_Freedom-metal_FreeRTOS.c
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/freertos/vector.S
)

#-------------------------------#
#     FreeRTOS Library          #
#-------------------------------#
add_library(
    bsp_freertos
    INTERFACE
)

target_include_directories(
    bsp_freertos
    INTERFACE
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/include
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/GCC/RISC-V/
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/freertos
)

target_compile_options(
    bsp_freertos
    INTERFACE
    -DWAV_MCU_IRQ_SYNC_CFG=${CONFIG_WAV_MCU_IRQ_SYNC_CFG}
    -DWAV_MCU_IRQ_EDGE_CFG=${CONFIG_WAV_MCU_IRQ_EDGE_CFG}
)

macro(bsp_post_build TARGET)
    get_property(WORKING_DIRECTORY TARGET ${TARGET} PROPERTY RUNTIME_OUTPUT_DIRECTORY)
    patch_image_header(${TARGET} "${TOOLCHAIN_PREFIX}" ${WORKING_DIRECTORY})
    add_custom_command(TARGET ${TARGET}
        POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} ${TARGET} --source --all-headers --demangle --line-numbers --wide > ${TARGET}.dump
        WORKING_DIRECTORY ${WORKING_DIRECTORY}
    )
    add_custom_command(TARGET ${TARGET}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} ${TARGET} -O binary ${TARGET}_itcm.bin -j .text -j .init
        COMMAND ${CMAKE_OBJCOPY} ${TARGET} -O binary ${TARGET}_dtcm.bin -j .preinit_array -j .init_array -j .fini_array -j .rodata -j .data -j .tdata -j .tbss -j .bss
        WORKING_DIRECTORY ${WORKING_DIRECTORY}
    )
endmacro()
