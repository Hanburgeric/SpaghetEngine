#ifndef EDITOR_H
#define EDITOR_H

// Engine
#include "Engine.h"

namespace spaghet {
namespace editor {

class Editor {
public:
  bool Initialize();
  void Run();
  void Shutdown();

private:
  engine::Engine engine_;
};

} // namespace editor
} // namespace spaghet

#endif // EDITOR_H
