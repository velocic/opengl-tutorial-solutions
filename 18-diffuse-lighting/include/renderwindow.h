#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <SDL2/SDL.h>
#include <GL/gl3w.h>

#include <iostream>
#include <string>

class RenderWindow
{
    private:
        SDL_Window *window = nullptr;
        SDL_GLContext renderContext;
    public:
        RenderWindow(const std::string &windowTitle, unsigned int screenWidth, unsigned int screenHeight);
        ~RenderWindow();
        void freeResources();
        SDL_Window* getRenderWindowHandle();
};

#endif

