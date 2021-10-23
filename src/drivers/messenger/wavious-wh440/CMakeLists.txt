# Share the same driver as wavious-mcu but just has a different compile flag
set(
    SRC
    ${WAV_RTOS_TOP_LEVEL}/messenger/wavious-mcu/driver.c
)

add_library(
    messenger_driver
    STATIC
    ${SRC}
)

target_include_directories(
    messenger_driver
    PUBLIC
    ${WAV_RTOS_TOP_LEVEL}/include
    ${WAV_RTOS_TOP_LEVEL}/include/drivers
)

target_compile_options(
    messenger_driver
    PRIVATE
    -DWAV_MESSENGER_INTF_HOST=${CONFIG_WAV_MESSENGER_INTF_HOST}
)

target_link_libraries(
    messenger_driver
    kernel
)
