################################################################################
##                        HAL Library
##
##  This library completes the implementation for the HAL. This library extends
##  any abstract interfaces required by the kernel such as CPU specific
##  implementations of certiain functions.
###############################################################################/
add_library(
    hal
    STATIC
    irq.c
    init.c
)

target_include_directories(
    hal
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${WAV_RTOS_TOP_LEVEL}/include/
)

# Find pthread library; required for this HAL
find_package (Threads REQUIRED)

target_link_libraries(
    hal
    ${CMAKE_THREAD_LIBS_INIT}
)
