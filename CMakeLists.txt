cmake_minimum_required(VERSION 3.24)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# Install dlls in the same directory as the executable on Windows
if(WIN32)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else()
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -g3 -Wall -Wextra -O1 -Wformat=2")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(MilTWM)

add_library(wm_dll SHARED src/wm_dll.c src/shared_memory.c src/error.c)
target_link_libraries(wm_dll PRIVATE user32)

add_executable(MilTWM src/main.c src/keyboard.c src/error.c src/shared_memory.c src/tiling.c)
target_link_libraries(MilTWM PRIVATE kernel32 user32 ole32)
