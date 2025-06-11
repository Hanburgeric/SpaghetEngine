#include "Editor.h"

// STL
#include <iostream>

// Engine
#include "Engine.h"

namespace spaghet {
namespace editor {

bool Editor::Initialize() {
  if (!engine_.Initialize()) {
    std::cout << "[Engine] Failed to initialize SpaghetEngine!" << std::endl;
    return false;
  }

  std::cout << "[Editor] Initializing SpaghetEditor..." << std::endl;
  std::cout << "[Editor] Editor initialized successfully!" << std::endl;
  return true;
}

void Editor::Run() {
  std::cout << "[Editor] Running SpaghetEditor..." << std::endl;
}

void Editor::Shutdown() {
  std::cout << "[Editor] Shutting down SpaghetEditor..." << std::endl;
  std::cout << "[Editor] Editor shut down." << std::endl;
}

} // namespace editor
} // namespace spaghet
