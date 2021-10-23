set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project("msg_loopback" C ASM)

# Directories
set(APP_DIR         "${CMAKE_CURRENT_LIST_DIR}")

# Source Files
set(SOURCE_FILES
    ${APP_DIR}/main.c
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${APP_DIR}
    ${WAV_RTOS_TOP_LEVEL}/bsp/${CONFIG_TARGET_BOARD}/freertos
    ${WAV_BUILD_TOP_LEVEL}/include
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    -Wno-unused-result
    -Wno-pointer-to-int-cast
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    -DGIT_SHA=\"${VERSION_GIT_SHA}\"
    -DGIT_DIRTY=${VERSION_DIRTY}
    -DGIT_AHEAD=${VERSION_AHEAD}
)

target_link_libraries(${PROJECT_NAME}
    kernel
    messenger_driver
)

target_link_options(
    ${PROJECT_NAME}
    PUBLIC
    -Wl,--defsym,__stack_size=0x200
    -Wl,--defsym,__heap_size=0x0
    -Wl,--build-id
)

bsp_post_build(${PROJECT_NAME})
