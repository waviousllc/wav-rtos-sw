# TODO: key off of ${CONFIG_TARGET_OSAL}
add_subdirectory(freertos)

# Add on OSAL independent sources
target_sources(
    osal
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/printf.c
)
