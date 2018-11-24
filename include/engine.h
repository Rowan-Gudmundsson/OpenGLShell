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
    void LoadLights();
    Object* ParseConfig(json);

    // Runtime functions
    void Run();
    unsigned int GetDT();
    long long GetCurrentTimeMillis();

    // Event handlers
    void KeyDown();
    void KeyUp();
    void MouseDown();
    void MouseUp();
    void MouseMove();

    // Destructors
    ~Engine();

  private:
    struct EventOptions {
      bool mouse_pressed;
      int click_x = 0, click_y = 0;
    } m_event_options;

    Options options;
    Window* m_window;

    SDL_Event m_event;

    Graphics* m_graphics;

    unsigned int m_DT;
    long long m_current_time_millis;
    bool m_running;
};
