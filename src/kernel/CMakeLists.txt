set(KERNEL_SRC
    ${CMAKE_CURRENT_LIST_DIR}/irq.c
    ${CMAKE_CURRENT_LIST_DIR}/completion.c
)

add_library(
    kernel
    STATIC
    ${KERNEL_SRC}
)

target_include_directories(
    kernel
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
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
