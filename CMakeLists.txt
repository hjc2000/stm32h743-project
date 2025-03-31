cmake_minimum_required(VERSION 3.8)

if(1)
	get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
	set(ProjectName "${CURRENT_DIR_NAME}")

	include($ENV{cpp_lib_build_scripts_path}/cmake-module/setup.cmake)

	project(${ProjectName})
	enable_language(ASM)
	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_C_STANDARD 17)
endif()

include(target_import_base)


add_executable(${ProjectName})

target_compile_definitions(
	${ProjectName}
	PUBLIC
	USE_HAL_DRIVER=1
	STM32H743xx=1
)

target_import_src(${ProjectName})

target_link_libraries(${ProjectName} PUBLIC -Wl,--start-group)
# target_import_base(${ProjectName} PUBLIC)
target_link_libraries(${ProjectName} PUBLIC c m)
target_link_libraries(${ProjectName} PUBLIC -Wl,--end-group)

add_custom_command(TARGET ${ProjectName} POST_BUILD
				   COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${ProjectName}>)
