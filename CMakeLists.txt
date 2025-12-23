cmake_minimum_required(VERSION 3.28)

project(RaylibTerrariaClone LANGUAGES CXX)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Raylib path
set(RAYLIB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib")

# Add raylib
add_subdirectory(${RAYLIB_ROOT})

# Source files
file(GLOB_RECURSE SOURCES
    src/main.cpp
    src/Player/Player.cpp
)


add_executable(Terraria ${SOURCES})

# Include directories
target_include_directories(Terraria PRIVATE
    src
    ${RAYLIB_ROOT}/src
)

# Link raylib
target_link_libraries(Terraria PRIVATE raylib)

# Windows-specific settings
if (WIN32)
    target_link_libraries(Terraria PRIVATE winmm)
endif()
