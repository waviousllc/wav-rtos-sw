if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project("WAV-RTOS-SW" C ASM)
    cmake_minimum_required(VERSION 3.13)

    # Set the output folder where your program will be created
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    # Make sure everything is built for correct architecture
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
    include(${CMAKE_CURRENT_LIST_DIR}/CMake.in)

    # Setup default compile options 
    add_compile_options(
        -O1
        -g
        -Wall
        -Wextra
        -Werror
        -Wno-unused-parameter
        -fdata-sections
        -ffunction-sections
        -std=c11
    )
endif()

include(${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/CMakeLists.txt)

#-------------------------------#
#         METAL Library         #
#-------------------------------#
# Create Metal Library
FILE(
    GLOB_RECURSE
    METAL_SRC
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/src/*.c
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/src/*.S
)

FILE(
    GLOB_RECURSE
    GLOSS_SRC
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/gloss/*.c
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/gloss/*.S
)

set(
    METAL_LIB_SRC
    ${METAL_SRC}
    ${GLOSS_SRC}
)

add_library(metal STATIC ${METAL_LIB_SRC})
target_include_directories(
    metal
    PUBLIC
    ${WAV_RTOS_TOP_LEVEL}/freedom-metal/
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/
)

target_compile_options(
    metal
    PRIVATE
    -Wno-array-bounds
)

#-------------------------------#
#     FreeRTOS Library          #
#-------------------------------#
# Create FreeRTOS Library
FILE(
    GLOB
    FREERTOS_SRC
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/*.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/GCC/RISC-V/*.c
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/GCC/RISC-V/*.S
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/MemMang/heap_${RTOS_HEAP_ALLOC_SCHEME}.c
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/freertos/Bridge_Freedom-metal_FreeRTOS.c
)

add_library(
    freertos
    STATIC
    ${FREERTOS_SRC}
)

target_include_directories(
    freertos
    PUBLIC
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/include
    ${WAV_RTOS_TOP_LEVEL}/FreeRTOS-Kernel/portable/GCC/RISC-V/
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/freertos
)

target_compile_options(
    freertos
    PRIVATE
    ${BSP_FREERTOS_COMPILE_OPTIONS}
)

target_link_libraries(freertos metal bsp_freertos)

#-------------------------------#
#      Everything else          #
#-------------------------------#

add_subdirectory("${WAV_RTOS_TOP_LEVEL}/src")
add_subdirectory("${WAV_RTOS_TOP_LEVEL}/app")
