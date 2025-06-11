# ======================================================================
# Engine Library
# ======================================================================
add_library(
    SpaghetEngine SHARED
        Engine.cpp
        Engine.h
)

target_include_directories(
    SpaghetEngine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
