#include "engine.h"

std::string load_file(std::string filename) {
  std::fstream file(filename.c_str(), std::ios::in);
  std::string result( (std::istreambuf_iterator<char>(file)),
                      (std::istreambuf_iterator<char>()));
  file.close();
  return result;
}

std::string clip_path(std::string full_path) {
  auto pos = full_path.find_last_of('/');
  return full_path.substr(pos + 1);
}

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
