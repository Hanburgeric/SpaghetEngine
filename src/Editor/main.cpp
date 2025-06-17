// spdlog
#include "spdlog/spdlog.h"

// src
#include "Editor.h"

int main(int argc, char* argv[]) {
  // Create and initialize editor
  spaghet::Editor editor{};
  if (!editor.Initialize()) {
    spdlog::error("Editor failed to initialize.");
    return 1;
  }

  // Run editor
  editor.Run();

  return 0;
}
