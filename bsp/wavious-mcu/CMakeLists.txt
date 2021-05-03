# MCU Interrupt Configuration
set(CONFIG_WAV_MCU_IRQ_SYNC_CFG 0x00000403)
set(CONFIG_WAV_MCU_IRQ_EDGE_CFG 0x1FFFFBFC)

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
    ${CMAKE_SOURCE_DIR}/bsp/${CONFIG_TARGET_BOARD}/freertos/vector.S
)

target_compile_options(
    bsp_freertos
    INTERFACE
    -DWAV_MCU_IRQ_SYNC_CFG=${CONFIG_WAV_MCU_IRQ_SYNC_CFG}
    -DWAV_MCU_IRQ_EDGE_CFG=${CONFIG_WAV_MCU_IRQ_EDGE_CFG}
)

macro(bsp_post_build target)
    add_custom_command(TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} ${target} -O binary ${target}_itcm.bin -j .text -j .init
        COMMAND ${CMAKE_OBJCOPY} ${target} -O binary ${target}_dtcm.bin -j .preinit_array -j .init_array -j .fini_array -j .rodata -j .data -j .tdata -j .tbss -j .bss
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endmacro()
