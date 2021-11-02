FILE(
    GLOB_RECURSE
    BSP_METAL_SOURCES
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/src/*.c
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/src/*.S
)

FILE(
    GLOB
    BSP_FREERTOS_SOURCES
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/*.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/GCC/RISC-V/*.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/GCC/RISC-V/*.S
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/MemMang/heap_${RTOS_HEAP_ALLOC_SCHEME}.c
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/freertos/Bridge_Freedom-metal_FreeRTOS.c
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/freertos/printf.c
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

macro(bsp_post_build TARGET)
    get_property(WORKING_DIRECTORY TARGET ${TARGET} PROPERTY RUNTIME_OUTPUT_DIRECTORY)
    patch_image_header(${TARGET} "${TOOLCHAIN_PREFIX}" ${WORKING_DIRECTORY})
    add_custom_command(TARGET ${TARGET}
        POST_BUILD
        COMMAND ${CMAKE_OBJDUM%P} ${TARGET} --source --all-headers --demangle --line-numbers --wide > ${TARGET}.dump
        WORKING_DIRECTORY ${WORKING_DIRECTORY}
    )
    add_custom_command(TARGET ${TARGET}
            POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} ${TARGET} -O binary  ${TARGET}.bin
            WORKING_DIRECTORY ${WORKING_DIRECTORY}
    )
endmacro()
