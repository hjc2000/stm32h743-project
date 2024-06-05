cmake_minimum_required(VERSION 3.20)
include(${cpp_lib_build_scripts_path}/cmake-module/setup.cmake)

set(ProjectName atk-stm32h743-bsp)
project(${ProjectName})
enable_language(ASM)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

add_library(${ProjectName})
target_import_src(${ProjectName})
target_link_libraries(${ProjectName} PUBLIC stm32h743-hal-wrapper)