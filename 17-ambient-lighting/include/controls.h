#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL2/SDL.h>
#include <vector>

enum class ControlKeys : int
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
        int mousePositionX = 0;
        int mousePositionY = 0;

        //Note: SDLKeyStates is populated from within SDL2, and this
        //only happens when all SDLEvents have been processed.
        const uint8_t *SDLKeyStates;
    public:
        Controls() :
            controlState(static_cast<int>(ControlKeys::AssignedKeyCount), false),
            SDLKeyStates(SDL_GetKeyboardState(NULL))
        {}

        const std::vector<bool> &getControlState() const;
        std::pair<int, int> getMouseState() const;
        bool isKeyPressed(ControlKeys key) const;
        void update();
};


#endif
