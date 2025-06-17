cmake_minimum_required(VERSION 3.25)
project(Spaghet VERSION 0.1.0 LANGUAGES C CXX)

# ======================================================================
# Compiler Settings
# ======================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ======================================================================
# Output Directories
# ======================================================================
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

# ======================================================================
# Project Targets
# ======================================================================
add_subdirectory(src/Engine)
add_subdirectory(src/Editor)
