#include <renderwindow.h>

RenderWindow::RenderWindow(const std::string &windowTitle, unsigned int screenWidth, unsigned int screenHeight)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    window = SDL_CreateWindow(
        windowTitle.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        SDL_WINDOW_OPENGL
    );

    //Create the OpenGL context before trying to initialize OpenGL API functions
    renderContext = SDL_GL_CreateContext(window);

    //Initialize OpenGL API functions, exit if we couldn't load the core functions
    if (gl3wInit()) {
        std::cout << "gl3w failed to initialize OpenGL" << std::endl;
    }
}

RenderWindow::~RenderWindow()
{
    freeResources();
}

void RenderWindow::freeResources()
{
    if (window != nullptr) {
        SDL_GL_DeleteContext(renderContext);
        SDL_DestroyWindow(window);
        SDL_Quit();

        window = nullptr;
    }
}

SDL_Window* RenderWindow::getRenderWindowHandle()
{
    return window;
}
