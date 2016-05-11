#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL2/SDL.h>
#include <vector>

enum class ControlKeys
{
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down,
    AssignedKeyCount
};

class Controls
{
    private:
        std::vector<bool> controlState;

        //Note: SDLKeyStates is populated from within SDL2, and this
        //only happens when all SDLEvents have been processed.
        const uint8_t *SDLKeyStates;
    public:
        Controls() :
            controlState(static_cast<int>(ControlKeys::AssignedKeyCount), false),
            SDLKeyStates(SDL_GetKeyboardState(NULL))
        {}

        bool isKeyPressed(ControlKeys key) const;
        const std::vector<bool> &getControlState() const;
};


#endif
