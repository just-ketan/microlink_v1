cmake_minimum_required(VERSION 3.16)
project(microlink C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(MICRLINK_SANITIZE "Enable ASan/UBSan when toolchain supports them" OFF)

include(${CMAKE_SOURCE_DIR}/cmake/CheckSanitizers.cmake)
microlink_check_sanitizers(MICRLINK_SANITIZERS_AVAILABLE)

if(MICRLINK_SANITIZE)
    if(MICRLINK_SANITIZERS_AVAILABLE)
        message(STATUS "MicroLink: sanitizers enabled (ASan/UBSan)")
    else()
        message(WARNING "MicroLink: MICRLINK_SANITIZE=ON but toolchain lacks libasan/libubsan — sanitizers disabled")
        set(MICRLINK_SANITIZE OFF)
    endif()
endif()

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

if(MICRLINK_SANITIZE AND MICRLINK_SANITIZERS_AVAILABLE AND NOT MSVC)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

set(ML_SOURCES
    virtual_mcu/registers/register.c
    virtual_mcu/registers/register_bank.c
    virtual_mcu/registers/register_definitions.c
    virtual_mcu/memory_map/memory_map.c
    virtual_mcu/clock/clock.c
    virtual_mcu/core/cycle_model.c
    virtual_mcu/interrupt_controller/irqc.c
    virtual_mcu/fault_injection/fault.c
    virtual_mcu/peripherals/gpio/gpio_periph.c
    virtual_mcu/peripherals/uart/uart_periph.c
    virtual_mcu/peripherals/spi/spi_periph.c
    virtual_mcu/peripherals/i2c/i2c_periph.c
    virtual_mcu/peripherals/timer/timer_periph.c
    virtual_mcu/peripherals/radio/radio_periph.c
    virtual_mcu/core/mcu.c
    firmware/rtos/task.c
    firmware/rtos/queue.c
    firmware/rtos/semaphore.c
    firmware/rtos/tick.c
    firmware/rtos/scheduler.c
    firmware/drivers/gpio/gpio_driver.c
    firmware/drivers/uart/uart_driver.c
    firmware/drivers/spi/spi_driver.c
    firmware/drivers/i2c/i2c_driver.c
    firmware/drivers/timer/timer_driver.c
    firmware/hal/gpio_hal.c
    firmware/hal/uart_hal.c
    firmware/hal/spi_hal.c
    firmware/hal/i2c_hal.c
    firmware/hal/timer_hal.c
    firmware/hal/radio_hal.c
    firmware/protocol/packet.c
    firmware/transceiver/radio_fw.c
    firmware/interrupts/isr.c
    firmware/memory/firmware_mem.c
    firmware/app/application.c
    cosim/serialization/jsonish.c
    cosim/ipc/tcp_ipc.c
    cosim/bridge/bridge.c
)

add_library(microlink STATIC ${ML_SOURCES})
target_include_directories(microlink PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/virtual_mcu/registers
)

if(WIN32)
    target_link_libraries(microlink PUBLIC ws2_32)
endif()

add_executable(microlink_sim firmware/app/main.c)
target_link_libraries(microlink_sim PRIVATE microlink)

add_executable(microlink_tests
    tests/unit/test_runner.c
    tests/unit/test_registers.c
    tests/unit/test_peripherals.c
)
target_link_libraries(microlink_tests PRIVATE microlink)

add_executable(microlink_bench benchmarks/bench.c)
target_link_libraries(microlink_bench PRIVATE microlink)

enable_testing()
add_test(NAME microlink_c_unit COMMAND microlink_tests)
