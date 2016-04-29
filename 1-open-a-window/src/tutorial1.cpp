#include <GL/gl3w.h>
#include <SDL2/SDL.h>

#include <iostream>

int main()
{
    //Initialize SDL2 with video and event modes enabled,
    //and request an OpenGL 3.3 Core Profile
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_Window* window = SDL_CreateWindow(
        "ogldev.atspace.co.uk Tutorial 1 -- Create a Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_OPENGL
    );

    //Create the OpenGL context before trying to initialize OpenGL API functions
    SDL_GLContext context = SDL_GL_CreateContext(window);

    //Initialize OpenGL API functions, exit if we couldn't load the core functions
    if (gl3wInit()) {
        std::cout << "gl3w failed to initialize OpenGL" << std::endl;
        return EXIT_FAILURE;
    }

    //Sit around for a while until user input
    SDL_Event event;
    bool userRequestedExit = false;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    while (userRequestedExit == false)
    {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                userRequestedExit = true;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    }


    //Shutdown SDL2 before exiting the program
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
