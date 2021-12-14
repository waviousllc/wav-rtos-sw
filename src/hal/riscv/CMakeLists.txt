# Make sure valid architecture is set
if (NOT CONFIG_SRC_ARCH MATCHES "riscv")
    message(FATAL_ERRROR "Building RISC-V HAL for invalid architecture")
endif()

################################################################################
##                         Metal Library
##
##  This library builds the SiFive Freedom Metal bare-metal framework that
##  serves as the guts of the hardware abstraction layer.
###############################################################################/
FILE(
    GLOB_RECURSE
    BSP_METAL_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/metal/drivers/riscv_cpu_min.c
    ${CMAKE_CURRENT_LIST_DIR}/metal/drivers/wav_test0.c
    ${CMAKE_CURRENT_LIST_DIR}/metal/drivers/wmcu_uart0.c
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/src/*.c
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/src/*.S
)

list(
    REMOVE_ITEM
    BSP_METAL_SOURCES
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/src/drivers/riscv_cpu.c
)

FILE(
    GLOB_RECURSE
    GLOSS_SRC
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/gloss/*.c
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/gloss/*.S
)

set(
    METAL_LIB_SRC
    ${BSP_METAL_SOURCES}
    ${GLOSS_SRC}
)

add_library(
    metal
    STATIC
    ${METAL_LIB_SRC}
)

target_include_directories(
    metal
    PUBLIC
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/
    ${CMAKE_CURRENT_LIST_DIR}/
)

target_compile_options(
    metal
    PRIVATE
    -Wno-array-bounds
)

target_link_libraries(
    metal
    "-Wl,--start-group"
    c
    gcc
    m
    "-Wl,--end-group"
)

################################################################################
##                        HAL Library
##
##  This library completes the implementation for the HAL. RISC-V HAL is
##  implemented by freedom-metal by SiFive. This library extends any abstract
##  interfaces required by the kernel such as CPU specific implementations of
##  certiain functions.
###############################################################################/
add_library(
    hal
    STATIC
    ${CMAKE_CURRENT_LIST_DIR}/irq.c
    ${CMAKE_CURRENT_LIST_DIR}/init.c
)

target_include_directories(
    hal
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${WAV_RTOS_TOP_LEVEL}/include/
)

target_link_libraries(
    hal
    PUBLIC
    board
    metal
    osal
)
