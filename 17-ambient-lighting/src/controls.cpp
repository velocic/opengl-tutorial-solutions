#include <controls.h>
// #include <iostream>

bool Controls::isKeyPressed(ControlKeys key) const
{
    return controlState[static_cast<int>(key)];
}

const std::vector<bool> &Controls::getControlState() const
{
    return controlState;
}

std::pair<int, int> Controls::getMouseState() const
{
    return std::make_pair(mousePositionX, mousePositionY);
}

void Controls::update()
{
    controlState[static_cast<int>(ControlKeys::Forward)] = SDLKeyStates[SDL_SCANCODE_W];
    controlState[static_cast<int>(ControlKeys::Backward)] = SDLKeyStates[SDL_SCANCODE_S];
    controlState[static_cast<int>(ControlKeys::Left)] = SDLKeyStates[SDL_SCANCODE_A];
    controlState[static_cast<int>(ControlKeys::Right)] = SDLKeyStates[SDL_SCANCODE_D];
    controlState[static_cast<int>(ControlKeys::Up)] = SDLKeyStates[SDL_SCANCODE_SPACE];
    controlState[static_cast<int>(ControlKeys::Down)] = SDLKeyStates[SDL_SCANCODE_LSHIFT];

    SDL_GetRelativeMouseState(&mousePositionX, &mousePositionY);

    // std::cout << "(" << mousePositionX << "," << mousePositionY << ")" << std::endl;
}
