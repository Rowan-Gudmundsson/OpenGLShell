#pragma once

#include <SDL2/SDL.h>

#include "graphics_headers.h"

class Window {
  public:
    // Constructors
    Window(Options*);

    // Setup functions
    bool Initialize();

    // Runtime functions
    void Swap();

    // Destructors
    ~Window();

  private:
    Options* options;
    SDL_Window* m_window;
    SDL_GLContext m_context;
};
