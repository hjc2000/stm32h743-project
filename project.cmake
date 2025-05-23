set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")

add_executable(${ProjectName})
target_import_src(${ProjectName})

target_import_task(${ProjectName} PUBLIC)
target_import_FatFs(${ProjectName} PUBLIC)
target_import_stm32h743iit6_p_net(${ProjectName} PUBLIC)
target_import_lwip(${ProjectName} PUBLIC)
target_import_lwip_wrapper(${ProjectName} PUBLIC)
target_import_freertos(${ProjectName} PUBLIC)
target_import_littlefs(${ProjectName} PUBLIC)
target_import_stm32h743iit6_hal(${ProjectName} PUBLIC)
target_import_stm32h743iit6_peripherals(${ProjectName} PUBLIC)
target_link_libraries(${ProjectName} PUBLIC c stdc++)

target_obj_size(${ProjectName})
