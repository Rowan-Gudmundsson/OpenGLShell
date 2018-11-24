#include "engine.h"

Engine::Engine(const std::string& name, int width, int height) : options(config) {
  options.window.name = name;
  options.window.height = height;
  options.window.width = width;
  options.window.fullscreen = false;
}

Engine::Engine(const std::string& name) : options(config) {
  options.window.name = name;
  options.window.width = 0;
  options.window.height = 0;
  options.window.fullscreen = true;
}

bool Engine::Initialize() {
  // Window setup
  m_window = new Window(&options);
  if (!m_window->Initialize()) {
    std::cout << "Window failed to initialize." << std::endl;
    return false;
  }

  // Graphics setup
  m_graphics = new Graphics(&options);
  if (!m_graphics->Initialize()) {
    std::cout << "Graphics failed to initialize." << std::endl;
    return false;
  }

  // Load all game objects
  LoadGameObjects();

  // Load all lights
  LoadLights();

  // Get the current time
  m_current_time_millis = GetCurrentTimeMillis();

  // No errors, setup successful
  return true;
}

void Engine::LoadGameObjects() {
  // Itterate through game objects and add them to graphics as root objects
  for (auto i : config["OBJECTS"]) {
    Object* tmp = ParseConfig(i);
    m_graphics->AddObject(i["SHADER"], tmp, true);
  }
}

void Engine::LoadLights() {
  // Itterate through all the lights
  for (auto i : config["LIGHTS"]) {
    std::string type = i["TYPE"].get<std::string>();
    if (type == "point") {
      m_graphics->AddPointLight(i);
    } else if (type == "directional") {
      m_graphics->AddDirectionalLight(i);
    }
  }
}

Object* Engine::ParseConfig(json obj) {
  // Construct the new object from the config
  Object* object = new Object(obj, &options);

  // Loop through the dependants adding the children to the parent
  for (auto i : obj["DEPENDANTS"]) {
    Object* tmp = ParseConfig(i);
    m_graphics->AddObject(i["SHADER"], tmp, false);
    object->AddChild(tmp);
  }

  // Return the parent
  return object;
}

void Engine::Run() {
  // Start simulation
  m_running = true;

  while (m_running) {
    // Update the timestep
    m_DT = GetDT();

    // Monitor for SDL events
    while (SDL_PollEvent(&m_event) != 0) {
      switch (m_event.type) {
        case SDL_QUIT: {
          m_running = false;
          break;
        }
        case SDL_KEYDOWN: {
          if (m_event.key.keysym.sym == SDLK_ESCAPE) {
            m_running = false;
          } else {
            KeyDown();
          }
          break;
        }
        case SDL_KEYUP: {
          KeyUp();
          break;
        }
        case SDL_MOUSEBUTTONDOWN: {
          MouseDown();
          break;
        }
        case SDL_MOUSEBUTTONUP: {
          MouseUp();
          break;
        }
        case SDL_MOUSEWHEEL:
        case SDL_MOUSEMOTION: {
          MouseMove();
          break;
        }
      }
    }

    // Update and render graphics
    m_graphics->Update(m_DT);
    m_graphics->Render();

    // Swap to window
    m_window->Swap();
  }
}

unsigned Engine::GetDT() {
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_current_time_millis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_current_time_millis);
  m_current_time_millis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis() {
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}

void Engine::KeyDown() {

}

void Engine::KeyUp() {

}

void Engine::MouseDown() {
  switch (m_event.button.button) {
    case SDL_BUTTON_LEFT: {
      // Initialize click position
      m_event_options.click_x = m_event.button.x;
      m_event_options.click_y = m_event.button.y;
      break;
    }
  }
}

void Engine::MouseUp() {
  switch (m_event.button.button) {
    case SDL_BUTTON_LEFT: {
      break;
    }
  }
}

void Engine::MouseMove() {
  switch (m_event.type) {
    case SDL_MOUSEMOTION: {
      // Check if clicking while dragging
      if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        // If this is the initial click, set options to mouse position
        if (m_event_options.click_x == 0 && m_event_options.click_y == 0) {
          m_event_options.click_x = m_event.motion.x;
          m_event_options.click_y = m_event.motion.y;
        }
        // Weight move amount by how far we've moved since the last tick as a fraction of
        // screen size
        float move_x = float(m_event_options.click_x - m_event.motion.x) / float(options.window.width);
        float move_y = float(m_event.motion.y - m_event_options.click_y) / float(options.window.height);
        // Update the camera
        m_graphics->UpdateCamera(move_x, move_y);
        // Update last tick position
        m_event_options.click_x = m_event.motion.x;
        m_event_options.click_y = m_event.motion.y;
      }
      break;
    }
    case SDL_MOUSEWHEEL: {
      // Update the camera zoom
      m_graphics->UpdateCamera(m_event.wheel.y);
      break;
    }
  }
}

Engine::~Engine() {
  delete m_window;
  delete m_graphics;
  m_window = nullptr;
  m_graphics = nullptr;
}
