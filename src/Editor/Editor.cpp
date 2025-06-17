#include "Editor.h"

// STL
#include <memory>

// glad
#include "glad/gl.h"

// spdlog
#include "spdlog/spdlog.h"

// SDL
#include "SDL3/SDL.h"

// imgui
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

namespace spaghet {

Editor::Editor()
    : platform_initialized_{ false }
    , window_{ nullptr, nullptr }
    , renderer_context_{ nullptr, nullptr }
    , gui_context_{ nullptr, nullptr }
    , gui_platform_initialized_{ false }
    , gui_renderer_initialized_{ false }
    , should_quit_{ false } {}

Editor::~Editor() {
  // Shutdown automatically just in case
  Shutdown();
}

bool Editor::Initialize() {
  // Initialize platform
  constexpr SDL_InitFlags init_flags{
    SDL_INIT_VIDEO | SDL_INIT_GAMEPAD
  };
  platform_initialized_ = SDL_Init(init_flags);
  if (!platform_initialized_) {
    spdlog::error(SDL_GetError());
    spdlog::error("Editor platform failed to initialize.");
    return false;
  } else {
    spdlog::info("Editor platform initialized.");
  }

  // Create window
  constexpr SDL_WindowFlags window_flags{
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY
  };
  window_ = std::unique_ptr<SDL_Window,
                            decltype(&SDL_DestroyWindow)>{
      SDL_CreateWindow("Spaghet Editor", 1280, 720, window_flags),
      SDL_DestroyWindow
  };
  if (!window_) {
    spdlog::error(SDL_GetError());
    spdlog::error("Editor failed to create window.");
    return false;
  } else {
    spdlog::info("Editor window created.");
  }

  // Create renderer context
  renderer_context_ = std::unique_ptr<SDL_GLContextState,
                                      decltype(&SDL_GL_DestroyContext)>{
    SDL_GL_CreateContext(window_.get()),
    SDL_GL_DestroyContext
  };
  if (!renderer_context_) {
    spdlog::error(SDL_GetError());
    spdlog::error("Editor failed to create renderer context.");
    return false;
  } else {
    spdlog::info("Editor renderer context created.");
  }

  // Load renderer function pointers
  if (!gladLoadGL(static_cast<GLADloadfunc>(SDL_GL_GetProcAddress))) {
    spdlog::error("Editor failed to load renderer function pointers.");
    return false;
  }

  // Create GUI context
  gui_context_ = std::unique_ptr<ImGuiContext,
                                 decltype(&ImGui::DestroyContext)>{
   ImGui::CreateContext(),
   ImGui::DestroyContext
  };
  if (!gui_context_) {
    spdlog::error("Editor failed to create GUI context.");
    return false;
  } else {
    spdlog::info("Editor GUI context created.");
  }

  // Configure GUI context
  gui_context_->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  gui_context_->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  gui_context_->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  gui_context_->IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  // Initialize GUI for platform
  gui_platform_initialized_ = ImGui_ImplSDL3_InitForOpenGL(
    window_.get(), renderer_context_.get()
  );
  if (!gui_platform_initialized_) {
    spdlog::error("Editor GUI failed to initialize for platform.");
    return false;
  } else {
    spdlog::info("Editor GUI initialized for platform.");
  }

  // Initialize GUI for renderer
  gui_renderer_initialized_ = ImGui_ImplOpenGL3_Init();
  if (!gui_renderer_initialized_) {
    spdlog::error("Editor GUI failed to initialize for renderer.");
    return false;
  } else {
    spdlog::info("Editor GUI initialized for renderer.");
  }

  return true;
}

void Editor::Run() {
  // Main loop
  while (!should_quit_) {
    // Handle events
    SDL_Event event{};
    while (SDL_PollEvent(&event)) {
      // Allow GUI to handle event first
      ImGui_ImplSDL3_ProcessEvent(&event);

      switch (event.type) {
        case SDL_EVENT_QUIT: { OnQuit(); break; }
        case SDL_EVENT_WINDOW_RESIZED: { OnWindowResized(); break; }
        default: { break; }
      }
    }

    // Begin new GUI frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // Create fullscreen window over viewport
    if (ImGuiViewport* viewport{ ImGui::GetMainViewport() }) {
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
    }

    constexpr ImGuiWindowFlags host_window_flags{
      ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_NoMove
      | ImGuiWindowFlags_NoCollapse
      | ImGuiWindowFlags_NoBackground
      | ImGuiWindowFlags_NoBringToFrontOnFocus
      | ImGuiWindowFlags_NoNavFocus
      | ImGuiWindowFlags_NoDocking
    };

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0F, 0.0F });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0F);

    if (ImGui::Begin("HostWindow", nullptr, host_window_flags)) {
      ImGui::PopStyleVar(3);

      // Create main menu bar
      CreateMainMenuBar();

      // Create dock space
      const ImGuiID main_dock_space_id{ ImGui::GetID("MainDockSpace") };
      ImGui::DockSpace(main_dock_space_id,
                       ImVec2{ 0.0F, 0.0F },
                       ImGuiDockNodeFlags_PassthruCentralNode);
    } else {
      ImGui::PopStyleVar(3);
    }
    ImGui::End();

    // Create editor window layout
    CreateHierarchyWindow();
    CreateInspectorWindow();
    CreateProjectWindow();
    CreateConsoleWindow();
    CreateSceneWindow();
    CreateGameWindow();

    // Render
    // Clear buffer
    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render GUI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and render platform windows if GUI multi-viewports are enabled
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();

      // Make renderer context current again
      SDL_GL_MakeCurrent(window_.get(), renderer_context_.get());
    }

    // Swap buffers
    SDL_GL_SwapWindow(window_.get());
  }
}

void Editor::Shutdown() {
  // Shutdown GUI for renderer
  if (gui_renderer_initialized_) {
    ImGui_ImplOpenGL3_Shutdown();
    gui_renderer_initialized_ = false;
    spdlog::info("Editor GUI shutdown for renderer.");
  }

  // Shutdown GUI for platform
  if (gui_platform_initialized_) {
    ImGui_ImplSDL3_Shutdown();
    gui_platform_initialized_ = false;
    spdlog::info("Editor GUI shutdown for platform.");
  }

  // Destroy GUI context
  if (gui_context_) {
    gui_context_.reset();
    spdlog::info("Editor GUI context destroyed.");
  }

  // Destroy renderer context
  if (renderer_context_) {
    renderer_context_.reset();
    spdlog::info("Editor renderer context destroyed.");
  }

  // Destroy window
  if (window_) {
    window_.reset();
    spdlog::info("Editor window destroyed.");
  }

  // Shutdown platform
  if (platform_initialized_) {
    SDL_Quit();
    platform_initialized_ = false;
    spdlog::info("Editor platform shut down.");
  }
}

void Editor::OnQuit() {
  should_quit_ = true;
}

void Editor::OnWindowResized() const {
  int window_width{};
  int window_height{};
  SDL_GetWindowSizeInPixels(window_.get(), &window_width, &window_height);
  glViewport(0, 0, window_width, window_height);
}

void Editor::CreateMainMenuBar() {
  // Main menu bar
  if (ImGui::BeginMainMenuBar()) {
    // File
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New Project")) {
        // ???
      }
      if (ImGui::MenuItem("Open Project")) {
        // ???
      }
      if (ImGui::MenuItem("Save Project")) {
        // ???
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Exit")) {
        should_quit_ = true;
      }

      ImGui::EndMenu();
    }

    // Edit
    if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Engine Settings")) {
        // ???
      }
      if (ImGui::MenuItem("Editor Settings")) {
        // ???
      }
      if (ImGui::MenuItem("Project Settings")) {
        // ???
      }

      ImGui::EndMenu();
    }

    // Window
    if (ImGui::BeginMenu("Window")) {
      if (ImGui::MenuItem("Hierarchy")) {
        // ???
      }
      if (ImGui::MenuItem("Inspector")) {
        // ???
      }
      if (ImGui::MenuItem("Project")) {
        // ???
      }
      if (ImGui::MenuItem("Console")) {
        // ???
      }
      if (ImGui::MenuItem("Scene")) {
        // ???
      }
      if (ImGui::MenuItem("Game")) {
        // ???
      }

      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

void Editor::CreateHierarchyWindow() {
  if (ImGui::Begin("Hierarchy")) {
    // ???
  }
  ImGui::End();
}

void Editor::CreateInspectorWindow() {
  if (ImGui::Begin("Inspector")) {
    // ???
  }
  ImGui::End();
}

void Editor::CreateProjectWindow() {
  if (ImGui::Begin("Project")) {
    // ???
  }
  ImGui::End();
}

void Editor::CreateConsoleWindow() {
  if (ImGui::Begin("Console")) {
    // ???
  }
  ImGui::End();
}

void Editor::CreateSceneWindow() {
  if (ImGui::Begin("Scene")) {
    // ???
  }
  ImGui::End();
}

void Editor::CreateGameWindow() {
  if (ImGui::Begin("Game")) {
    // ???
  }
  ImGui::End();
}

} // namespace spaghet
