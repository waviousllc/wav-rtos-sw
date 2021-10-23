set(CONFIG_TARGET_HAL posix)

add_compile_options(
    -std=gnu99
)

FILE(
    GLOB
    BSP_FREERTOS_SOURCES
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/*.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/*.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/utils/*.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/MemMang/heap_${RTOS_HEAP_ALLOC_SCHEME}.c
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
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/utils
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/freertos
)

macro(bsp_post_build TARGET)
    get_property(WORKING_DIRECTORY TARGET ${TARGET} PROPERTY RUNTIME_OUTPUT_DIRECTORY)
    add_custom_command(TARGET ${TARGET}
        POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} ${TARGET} --source --all-headers --demangle --line-numbers --wide > ${TARGET}.dump
        WORKING_DIRECTORY ${WORKING_DIRECTORY}
    )
endmacro()
