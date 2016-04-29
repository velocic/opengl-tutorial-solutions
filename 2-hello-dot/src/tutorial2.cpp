#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <iostream>

int main()
{
    //Initialize SDL2 with video and event modes enabled,
    //and request an OpenGL 3.3 Core Profile
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_Window* window = SDL_CreateWindow(
        "ogldev.atspace.co.uk Tutorial 2 -- Hello dot!",
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

    glm::vec3 vertices[1];
    vertices[0] = glm::vec3(0.0f, 0.0f, 0.0f);

    //Allocate a buffer for our lonely vertex
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Sit around for a while until user input
    SDL_Event event;
    bool userRequestedExit = false;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    while (userRequestedExit == false)
    {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                userRequestedExit = true;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_POINTS, 0, 1);

        glDisableVertexAttribArray(0);

        SDL_GL_SwapWindow(window);
    }


    //Shutdown SDL2 before exiting the program
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
