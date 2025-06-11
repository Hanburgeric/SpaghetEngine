#include "Engine.h"

// STL
#include <iostream>

namespace spaghet {
namespace engine {

bool Engine::Initialize() {
  std::cout << "[Engine] Initializing SpaghetEngine..." << std::endl;
  std::cout << "[Engine] Engine initialized successfully!" << std::endl;
  return true;
}

void Engine::Shutdown() {
  std::cout << "[Engine] Shutting down SpaghetEngine..." << std::endl;
  std::cout << "[Engine] Engine shut down." << std::endl;
}

} // namespace engine
} // namespace spaghet
