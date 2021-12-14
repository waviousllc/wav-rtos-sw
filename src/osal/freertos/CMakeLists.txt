################################################################################
##                         FreeRTOS Library
##
##  This library builds the FreeRTOS OSAL library. It should be linked against
##  the kernel library
###############################################################################/
add_library(
    osal
    STATIC
    ${BSP_FREERTOS_SOURCES}
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/croutine.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/event_groups.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/list.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/queue.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/stream_buffer.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/tasks.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/timers.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/MemMang/heap_${RTOS_HEAP_ALLOC_SCHEME}.c
    ${CMAKE_CURRENT_LIST_DIR}/freertos_bridge.c
    ${CMAKE_CURRENT_LIST_DIR}/time.c
)

target_include_directories(
    osal
    PUBLIC  #TODO: Should be private once osal exists
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/include
    ${WAV_RTOS_TOP_LEVEL}/include/
    ${CMAKE_CURRENT_LIST_DIR}
)

# Set based on which architecture is set
if (${CONFIG_SRC_ARCH} MATCHES riscv)
    FILE(
        GLOB
        FREERTOS_SOURCES
        ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/GCC/RISC-V/*.c
        ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/GCC/RISC-V/*.S
    )

    target_include_directories(
        osal
        PUBLIC
        ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/GCC/RISC-V/
    )

# POSIX is default
else()
    FILE(
        GLOB
        FREERTOS_SOURCES
        ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/*.c
        ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/utils/*.c
    )

    target_include_directories(
        osal
        PUBLIC
        ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/
        ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/utils/
    )
endif()

target_sources(
    osal
    PRIVATE
    ${FREERTOS_SOURCES}
)

target_link_libraries(
    osal
    PUBLIC
    hal
    bsp_freertos
)
