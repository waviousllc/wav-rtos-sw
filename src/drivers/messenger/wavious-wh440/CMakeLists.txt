# Share the same driver as wavious-mcu but just has a different compile flag
set(
    SRC
    ${WAV_RTOS_TOP_LEVEL}/src/drivers/messenger/wavious-mcu/driver.c
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
    -DWAV_MESSENGER_INTF_HOST=1
)

target_link_libraries(
    messenger_driver
    kernel
)
