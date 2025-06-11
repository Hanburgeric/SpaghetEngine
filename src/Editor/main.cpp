// STL
#include <iostream>

// Editor
#include "Editor.h"

int main(int argc, char* argv[]) {
  spaghet::editor::Editor editor{};
  if (!editor.Initialize()) {
    std::cout << "[Editor] Failed to initialize SpaghetEditor!" << std::endl;
    return 1;
  }

  editor.Run();

  editor.Shutdown();

  return 0;
}
