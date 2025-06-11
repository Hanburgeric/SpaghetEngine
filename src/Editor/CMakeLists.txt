# ======================================================================
# Editor Executable
# ======================================================================
add_executable(
    SpaghetEditor
        main.cpp

        Editor.cpp
        Editor.h
)

target_link_libraries(
    SpaghetEditor PRIVATE
        SpaghetEngine
)
