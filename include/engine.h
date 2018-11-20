#pragma once

#include <sys/time.h>
#include <assert.h>

#include "graphics.h"
#include "window.h"

class Engine {
  public:
    // Constructors
    Engine(const std::string&, int, int);
    Engine(const std::string&);

    // Setup functions
    bool Initialize();
    void LoadGameObjects();

    // Runtime functions
    void Run();
    unsigned int GetDT();
    long long GetCurrentTimeMillis();

    // Destructors
    ~Engine()

  private:
    Window* m_window;
    struct WindowInfo {
      std::string name;
      int width, height;
      bool fullscreen;
    } w_info;

    SDL_Event m_event;

    Graphics* m_graphics;

    unsigned int m_DT;
    long long m_current_time_millis;
    bool m_running;
};
