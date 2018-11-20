#pragma once

#include <SDL2/SDL.h>

#include "graphics_headers.h"

class Window {
  public:
    // Constructors
    Window();

    // Setup functions
    bool Initialize(const std::string&, int&, int&);

    // Runtime functions
    void Swap();

    // Destructors
    ~Window();

  private:
    SDL_Window* m_window;
    SDL_GLContext m_context;
};
