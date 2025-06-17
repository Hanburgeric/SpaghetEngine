# ======================================================================
# Editor Dependencies
# ======================================================================
find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)

include(FetchContent)

FetchContent_Declare(
    EnTT            # v3.15.0
    GIT_REPOSITORY  https://github.com/skypjack/entt.git
    GIT_TAG         d4014c74dc3793aba95ae354d6e23a026c2796db
)

FetchContent_Declare(
    glad            # v2.0.8
    GIT_REPOSITORY  https://github.com/Dav1dde/glad.git
    GIT_TAG         73db193f853e2ee079bf3ca8a64aa2eaf6459043
)

FetchContent_Declare(
    glfw            # 3.4
    GIT_REPOSITORY  https://github.com/glfw/glfw.git
    GIT_TAG         7b6aead9fb88b3623e3b3725ebb42670cbe4c579
)

FetchContent_Declare(
    glm             # 1.0.1
    GIT_REPOSITORY  https://github.com/g-truc/glm.git
    GIT_TAG         0af55ccecd98d4e5a8d1fad7de25ba429d60e863
)

FetchContent_Declare(
    imgui           # v1.91.9b-docking
    GIT_REPOSITORY  https://github.com/ocornut/imgui.git
    GIT_TAG         4806a1924ff6181180bf5e4b8b79ab4394118875
)

FetchContent_Declare(
    SDL3            # release-3.2.16
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL.git
    GIT_TAG         c9a6709bd21750f1ad9597be21abace78c6378c9
)

FetchContent_Declare(
    spdlog          # v1.15.3
    GIT_REPOSITORY  https://github.com/gabime/spdlog.git
    GIT_TAG         6fa36017cfd5731d617e1a934f0e5ea9c4445b13
)

FetchContent_MakeAvailable(
    EnTT glad glfw glm imgui SDL3 spdlog
)

# ======================================================================
# Editor Executable
# ======================================================================
add_subdirectory(${glad_SOURCE_DIR}/cmake ${glad_BINARY_DIR})
glad_add_library(glad REPRODUCIBLE API gl:core=4.6)

add_executable(
    SpaghetEditor
        main.cpp

        Editor.cpp
        Editor.h
)

target_sources(
    SpaghetEditor PRIVATE
        ${imgui_SOURCE_DIR}/imconfig.h
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imstb_rectpack.h
        ${imgui_SOURCE_DIR}/imstb_textedit.h
        ${imgui_SOURCE_DIR}/imstb_truetype.h

        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h

        ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
        ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h
)

target_include_directories(
    SpaghetEditor PRIVATE
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends

        ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
    SpaghetEditor PRIVATE
        SpaghetEngine

        EnTT::EnTT
        glad
        glfw
        glm::glm
        OpenGL::GL
        SDL3::SDL3
        spdlog::spdlog
        Vulkan::Vulkan
)

# ======================================================================
# Post Build Commands
# ======================================================================
get_filename_component(
    CMAKE_CXX_COMPILER_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY
)

foreach(
    dll IN ITEMS
    libstdc++-6.dll
    libgcc_s_seh-1.dll
    libwinpthread-1.dll
)
    add_custom_command(
        TARGET SpaghetEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CXX_COMPILER_DIR}/${dll}"
        $<TARGET_FILE_DIR:SpaghetEditor>
    )
endforeach()
