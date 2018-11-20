#include "engine.h"

int main() {
  Engine* engine = new Engine("Window test", 800, 600);

  if (!engine->Initialize()) {
    std::cout << "Engine failed to start." << std::endl;
    delete engine;
    engine = nullptr;
    return 1;
  }

  engine->Run();
  delete engine;
  engine = nullptr;

  return 0;
}
