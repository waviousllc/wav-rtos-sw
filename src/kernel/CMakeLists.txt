set(KERNEL_SRC
    ${CMAKE_CURRENT_LIST_DIR}/irq.c
    ${CMAKE_CURRENT_LIST_DIR}/completion.c
    ${CMAKE_CURRENT_LIST_DIR}/list.c
    ${CMAKE_CURRENT_LIST_DIR}/fsm.c
    ${CMAKE_CURRENT_LIST_DIR}/notification.c
    ${CMAKE_CURRENT_LIST_DIR}/messenger.c
    ${CMAKE_CURRENT_LIST_DIR}/stateMachine.c
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

target_link_libraries(
    kernel
    hal
    freertos
)
