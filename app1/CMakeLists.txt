cmake_minimum_required(VERSION 3.20.0)

# Let Zephyr find <zephyr-course>/dts/bindings/our,driver.yaml
list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(app1)

target_sources(app PRIVATE src/main.cpp)

add_subdirectory(drivers/our_driver)
