#include "engine.h"

json config;
void GetConfig(std::string filename);

/**
 * Loads a file and returns a string
 * @param  filename - The filename of the file to read.
 * @return          The string containing the contents of the file.
 */
std::string load_file(std::string filename) {
  std::fstream file(filename.c_str(), std::ios::in);
  std::string result( (std::istreambuf_iterator<char>(file)),
                      (std::istreambuf_iterator<char>()));
  file.close();
  return result;
}

/**
 * Returns the name of a file given a path to the file
 * @param  full_path - The path to the file
 * @return           The filename of the file
 */
std::string clip_path(std::string full_path) {
  auto pos = full_path.find_last_of('/');
  return full_path.substr(pos + 1);
}

int main() {
  GetConfig("../include/config.json");
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

/**
 * Read the config file into the global json object
 * @param filename - The name of the json file.
 */
void GetConfig(std::string filename) {
  std::fstream file(filename.c_str(), std::ios::in);

  file >> config;

  file.close();
}
