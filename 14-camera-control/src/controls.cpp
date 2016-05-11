#include <controls.h>

bool Controls::isKeyPressed(ControlKeys key) const
{
    return controlState[static_cast<int>(key)];
}

const std::vector<bool> &Controls::getControlState() const
{
    return controlState;
}
