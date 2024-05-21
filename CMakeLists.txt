cmake_minimum_required(VERSION 3.24)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_FLAGS_DEBUG
    "${CMAKE_C_FLAGS_DEBUG} -g3 -Wall -Wextra -O1 -Wformat=2")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(MilTWM)

add_executable(MilTWM src/main.c src/keyboard.c src/error.c src/tiling.c src/doublylinkedlist.c)
target_link_libraries(MilTWM PRIVATE kernel32 user32 ole32)
