# Lightning Display CMakeLists.txt
target_sources(app PRIVATE
    lightning_display.c
)

target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
) 
