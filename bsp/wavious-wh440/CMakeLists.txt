# Set Messaging Interface to MCU Type
# wavious-wh440 is type host, set host to 0x1
set(CONFIG_WAV_MESSENGER_INTF_HOST 0x1)

#-------------------------------#
#     FreeRTOS Library          #
#-------------------------------#
add_library(
    bsp_freertos
    INTERFACE
)

target_sources(
    bsp_freertos
    INTERFACE
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/freertos/vector.S
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/freertos/printf.c
)

target_compile_options(
    bsp_freertos
    INTERFACE
    -DWAV_MESSENGER_INTF_HOST=${CONFIG_WAV_MESSENGER_INTF_HOST}
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
