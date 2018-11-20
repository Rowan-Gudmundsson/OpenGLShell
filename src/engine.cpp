#include "engine.h"

Engine::Engine(const std::string& name, int width, int height) {
  w_info.name = name;
  w_info.width = width;
  w_info.height = height;
  w_info.fullscreen = false;
}

Engine::Engine(const std::string& name) {
  w_info.name = name;
  w_info.width = 0;
  w_info.height = 0;
  w_info.fullscreen = true;
}

bool Engine::Initialize() {
  // Window setup
  m_window = new Window();
  if (!m_window->Initialize(w_info.name, w_info.width, w_info.height)) {
    std::cout << "Window failed to initialize." << std::endl;
    return false;
  }

  // Graphics setup
  m_graphics = new Graphics();
  if (!m_graphics->Initialize(w_info.width, w_info.height)) {
    std::cout << "Graphics failed to initialize." << std::endl;
    return false;
  }

  // Load all game objects
  LoadGameObjects();

  // Get the current time
  m_current_time_millis = GetCurrentTimeMillis();

  // No errors, setup successful
  return true;
}

void Engine::LoadGameObjects() {

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
          }
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

Engine::~Engine() {
  delete m_window;
  delete m_graphics;
  m_window = nullptr;
  m_graphics = nullptr;
}
