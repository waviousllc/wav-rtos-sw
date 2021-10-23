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
    irq.c
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
    metal
)
