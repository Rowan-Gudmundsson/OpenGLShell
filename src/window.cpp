#include "window.h"

Window::Window() {
  m_window = nullptr;
}

bool Window::Initialize(const std::string& name, int& width, int& height) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL failed to initialize: " << SDL_GetError() << std::endl;
    return false;
  }

  // Set versions.
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

  // Get the display mode
  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);

  // If fullscreen, create fullscreen window
  if (height == 0 && width == 0) {
    m_window = SDL_CreateWindow(
      name.c_str(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      width,
      height,
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP
    );
    width = current.w;
    height = current.h;
  }

  // Else create window with width and height
  m_window = SDL_CreateWindow(
    name.c_str(),
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    width,
    height,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE
  );

  if (m_window == NULL) {
    std::cout << "Window failed to create: " << SDL_GetError() << std::endl;
    return false;
  }

  m_context = SDL_GL_CreateContext(m_window);
  if (m_context == NULL) {
    std::cout << "OpenGL context not created: " << SDL_GetError() << std::endl;
    return false;
  }

  if (SDL_GL_SetSwapInterval(1) < 0) {
    std::cout << "Unable to use VSync: " << SDL_GetError() << std::endl;
    return false;
  }

  // No errors, init was successful.
  return true;
}

void Window::Swap() {
  SDL_GL_SwapWindow(m_window);
}

Window::~Window() {
  SDL_StopTextInput();
  SDL_DestroyWindow(m_window);
  m_window = nullptr;
  SDL_Quit();
}
