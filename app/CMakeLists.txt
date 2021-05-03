if (${CONFIG_SRC_ARCH} MATCHES "riscv")
    add_subdirectory("freertos-sample")
endif (${CONFIG_SRC_ARCH} MATCHES "riscv")
