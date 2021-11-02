set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/driver.c
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
    -DWAV_MESSENGER_INTF_HOST=0
)

target_link_libraries(
    messenger_driver
    kernel
)
