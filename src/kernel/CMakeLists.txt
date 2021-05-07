set(KERNEL_SRC
    ${CMAKE_CURRENT_LIST_DIR}/irq.c
    ${CMAKE_CURRENT_LIST_DIR}/completion.c
    ${CMAKE_CURRENT_LIST_DIR}/list.c
    ${CMAKE_CURRENT_LIST_DIR}/fsm.c
)

add_library(
    kernel
    STATIC
    ${KERNEL_SRC}
)

target_include_directories(
    kernel
    PUBLIC
    ${WAV_RTOS_TOP_LEVEL}/include
)

target_compile_definitions(kernel
    PUBLIC
    -DCONFIG_CPU_NUM_IRQ=${CONFIG_CPU_NUM_IRQ}
)

target_link_libraries(
    kernel
    metal
    freertos
)
