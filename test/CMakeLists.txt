cmake_minimum_required(VERSION 3.20)
include($ENV{cpp_lib_build_scripts_path}/cmake-module/setup.cmake)
include(target_import_freertos)
include(target_import_task)
include(target_import_FatFs)
include(target_import_littlefs)
include(target_import_system_call)
include(target_import_lwip)
include(target_import_stm32h743iit6_p_net)

set(ProjectName test)
project(${ProjectName})
enable_language(ASM)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

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
target_link_libraries(${ProjectName} PUBLIC c m atk-stm32h743-bsp)
target_link_libraries(${ProjectName} PUBLIC -Wl,--end-group)

add_custom_command(TARGET ${ProjectName} POST_BUILD
				   COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${ProjectName}>)

add_custom_command(TARGET ${ProjectName} POST_BUILD
				   COMMAND ${obj_copy} -O binary $<TARGET_FILE:${ProjectName}> $<TARGET_FILE_DIR:${ProjectName}>/${ProjectName}.bin
				   COMMENT "由 elf 生成 bin 文件。")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${ProjectName}.bin"
    	DESTINATION bin)

# 安装时自动尝试将 bin 文件下载到单片机
install(CODE "
    message(STATUS \"正在将 ${ProjectName}.bin 下载到单片机\")

    execute_process(
        COMMAND st-flash --reset write \"${CMAKE_INSTALL_PREFIX}/bin/${ProjectName}.bin\" 0x8000000
        RESULT_VARIABLE cmd_result
    )

    if(\${cmd_result} STREQUAL \"0\")
        message(STATUS \"将 ${ProjectName}.bin 下载到单片机成功 \")
    else()
        message(STATUS \"尝试将 ${ProjectName}.bin 下载到单片机失败: \${cmd_result}\")
    endif()
")
