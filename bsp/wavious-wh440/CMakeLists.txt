################################################################################
##                      BSP Variables
##
##  Sets CMAKE flags / varaibles necessary for bulding code for this BSP.
###############################################################################/

# TODO: Configurable in the future
set(CONFIG_TARGET_HAL riscv)

################################################################################
##                         Board Library
##
##  This library builds the Wavious-MCU board library. It should be linked
##  against the HAL library.
###############################################################################/
add_library(
    board
    STATIC
    ${CMAKE_CURRENT_LIST_DIR}/board/board.c
)

target_include_directories(
    board
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

################################################################################
##                      BSP FreeRTOS Sources
##
##  Defines all files that are required to build FreeRTOS for this board.
###############################################################################/
FILE(
    GLOB
    BSP_FREERTOS_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/freertos/vector.S
)

################################################################################
##                      BSP FreeRTOS Library
##
##  This interface library defines all freertos related headers and compile
##  options specific to this board. FreeRTOS itself is built elsewhere.
###############################################################################/
add_library(
    bsp_freertos
    INTERFACE
)

target_include_directories(
    bsp_freertos
    INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/freertos/
)

################################################################################
##                      BSP Post Build MACRO
##
##  Macro for how binaries should be exported / patched once built. This macro
##  should be called for all targets that build an application for this board.
###############################################################################/
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
            COMMAND ${CMAKE_OBJCOPY} ${TARGET} -O binary  ${TARGET}.bin
            WORKING_DIRECTORY ${WORKING_DIRECTORY}
    )
endmacro()
