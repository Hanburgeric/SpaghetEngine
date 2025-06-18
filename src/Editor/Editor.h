#ifndef EDITOR_H
#define EDITOR_H

// STL
#include <memory>

// SDL
#include "SDL3/SDL.h"

// imgui
#include "imgui.h"

namespace spaghet {

class Editor {
public:
  Editor();
  ~Editor();

  bool Initialize();
  void Run();
  void Shutdown();

private:
  void OnQuit();
  void OnWindowResized() const;

  void CreateMainMenuBar();

  void SetupDefaultEditorDockSpaceLayout(ImGuiID dock_space_id);

  void CreateHierarchyWindow();
  void CreateInspectorWindow();
  void CreateProjectWindow();
  void CreateConsoleWindow();
  void CreateSceneWindow();
  void CreateGameWindow();

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

  bool first_run_;

  bool show_hierarchy_window_;
  bool show_inspector_window_;
  bool show_project_window_;
  bool show_console_window_;
  bool show_scene_window_;
  bool show_game_window_;
};

} // namespace spaghet

#endif // EDITOR_H
