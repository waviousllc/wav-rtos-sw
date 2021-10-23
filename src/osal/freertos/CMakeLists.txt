################################################################################
##                         FreeRTOS Library
##
##  This library builds the FreeRTOS OSAL library. It should be linked against
##  the kernel library
###############################################################################/
FILE(
    GLOB
    FREERTOS_SOURCES
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/*.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/GCC/RISC-V/*.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/GCC/RISC-V/*.S
)

add_library(
    osal
    STATIC
    ${BSP_FREERTOS_SOURCES}
    ${FREERTOS_SOURCES}
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/MemMang/heap_${RTOS_HEAP_ALLOC_SCHEME}.c
    ${CMAKE_CURRENT_LIST_DIR}/freertos_bridge.c
    ${CMAKE_CURRENT_LIST_DIR}/time.c
)

target_include_directories(
    osal
    PUBLIC  #TODO: Should be private once osal exists
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/include
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/GCC/RISC-V/
    ${WAV_RTOS_TOP_LEVEL}/include/
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(
    osal
    PUBLIC
    hal
    bsp_freertos
)
