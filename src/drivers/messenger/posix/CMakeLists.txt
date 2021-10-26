# set(
#     SRC
#     ${CMAKE_CURRENT_LIST_DIR}/driver.c
# )

# add_library(
#     messenger_driver
#     STATIC
#     ${SRC}
# )

# target_include_directories(
#     messenger_driver
#     PUBLIC
#     ${WAV_RTOS_TOP_LEVEL}/include
#     ${WAV_RTOS_TOP_LEVEL}/include/drivers
# )

# target_link_libraries(
#     messenger_driver
#     kernel
# )

# Share the same driver as wavious-mcu
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

target_link_libraries(
    messenger_driver
    kernel
)
