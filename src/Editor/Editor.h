#ifndef EDITOR_H
#define EDITOR_H

// STL
#include <memory>

// SDL
#include "SDL3/SDL.h"

// imgui
#include "imgui.h"

namespace spaghet {
namespace editor {

class Editor {
public:
  Editor();
  ~Editor();

  bool Initialize();
  void Run();
  void Shutdown();

private:
  void OnQuit();
  void OnWindowResized();

private:
  bool platform_initialized_;
  std::unique_ptr<SDL_Window,
                  decltype(&SDL_DestroyWindow)> window_;
  std::unique_ptr<SDL_GLContextState,
                  decltype(&SDL_GL_DestroyContext)> renderer_context_;
  std::unique_ptr<ImGuiContext,
                  decltype(&ImGui::DestroyContext)> gui_context_;
  bool gui_platform_initialized_;
  bool gui_renderer_initialized_;
  bool should_quit_;
};

} // namespace editor
} // namespace spaghet

#endif // EDITOR_H
