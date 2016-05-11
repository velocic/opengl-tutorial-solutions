#include <camera.h>

void Camera::update(const Controls& input)
{
    if (input.isKeyPressed(ControlKeys::Forward)) {
        cameraWorldPosition.z -= movementSpeed;
    }

    if (input.isKeyPressed(ControlKeys::Backward)) {
        cameraWorldPosition.z += movementSpeed;
    }

    if (input.isKeyPressed(ControlKeys::Left)) {
        cameraWorldPosition.x -= movementSpeed;
    }

    if (input.isKeyPressed(ControlKeys::Right)) {
        cameraWorldPosition.x += movementSpeed;
    }

    if (input.isKeyPressed(ControlKeys::Up)) {
        cameraWorldPosition.y += movementSpeed;
    }

    if (input.isKeyPressed(ControlKeys::Down)) {
        cameraWorldPosition.y -= movementSpeed;
    }

    //Handle camera pan/tilt with mouse controls here

    viewMatrix = glm::lookAt(
        cameraWorldPosition,
        cameraDirection,
        glm::vec3(0, 1, 0)
    );
}
