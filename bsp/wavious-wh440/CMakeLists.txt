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

macro(bsp_post_build target)
    patch_image_header(${target} "${TOOLCHAIN_PREFIX}")
    add_custom_command(TARGET ${target}
            POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} ${target} -O binary  ${target}.bin
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endmacro()
