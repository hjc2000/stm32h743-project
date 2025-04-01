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


include(target_import_freertos)
include(target_import_task)
include(target_import_FatFs)
include(target_import_littlefs)
include(target_import_system_call)
include(target_import_lwip)
include(target_import_stm32h743iit6_p_net)


add_executable(${ProjectName})
target_import_src(${ProjectName})

target_link_libraries(${ProjectName} PUBLIC -Wl,--start-group)
target_import_freertos(${ProjectName} PUBLIC)
target_import_task(${ProjectName} PUBLIC)
target_import_FatFs(${ProjectName} PUBLIC)
target_import_littlefs(${ProjectName} PUBLIC)
target_import_system_call(${ProjectName} PUBLIC)
target_import_lwip(${ProjectName} PUBLIC)
target_import_stm32h743iit6_p_net(${ProjectName} PUBLIC)
target_link_libraries(${ProjectName} PUBLIC atk-stm32h743-bsp)
target_link_libraries(${ProjectName} PUBLIC -Wl,--end-group)

add_custom_command(TARGET ${ProjectName} POST_BUILD
				   COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${ProjectName}>)
