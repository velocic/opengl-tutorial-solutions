#include <camera.h>

Camera::Camera(glm::mat4 defaultViewMatrix, unsigned int windowWidth, unsigned int windowHeight) :
    viewMatrix(defaultViewMatrix),
    windowWidth(windowWidth),
    windowHeight(windowHeight)
{
    mousePositionX = windowWidth / 2;
    mousePositionY = windowHeight / 2;
}

void Camera::calculateRotation()
{
}

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
        (cameraForward + cameraWorldPosition) * cameraRotation,
        glm::vec3(0, 1, 0)
    );
}
