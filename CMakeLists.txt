if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project("WAV-RTOS-SW" C ASM)
    cmake_minimum_required(VERSION 3.13)

    # Set the output folder where your program will be created
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/app)

    # Make sure everything is built for correct architecture
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

    # Build configuration
    include(${CMAKE_CURRENT_LIST_DIR}/CMake.in)

    # Build versioning
    set(VERSION_UPDATE_FROM_GIT TRUE)
    find_package(BuildInfo REQUIRED)

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
    )
endif()

include(${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/CMakeLists.txt)

#-------------------------------#
#      Build all layers         #
#-------------------------------#
add_subdirectory("${WAV_RTOS_TOP_LEVEL}/src")
add_subdirectory("${WAV_RTOS_TOP_LEVEL}/app")
