#include <camera.h>

Camera::Camera(glm::mat4 defaultViewMatrix) :
    viewMatrix(defaultViewMatrix)
{
    initCameraRotationAngles();
}

void Camera::initCameraRotationAngles()
{
    auto target = glm::normalize(forwardVector + cameraWorldPosition);

    //Direction vector parallel to the xz plane
    auto horizontalTarget = glm::normalize(glm::vec3(target.x, 0.0, target.z));

    //Calculate angle on 2D unit circle to determine our horizontal facing
    if (horizontalTarget.z >= 0.0f) {
        if (horizontalTarget.x >= 0.0f) {
            //Top-right quadrant
            horizontalAngle = 360.0f - Utilities::Math::radiansToDegrees(asin(horizontalTarget.z));
        } else {
            //Top-left quadrant
            horizontalAngle = 180.0f + Utilities::Math::radiansToDegrees(asin(horizontalTarget.z));
        }
    } else {
        if (horizontalTarget.x >=0) {
            //Bottom-right quadrant
            horizontalAngle = Utilities::Math::radiansToDegrees(asin(-horizontalTarget.z));
        } else {
            //Bottom-left quadrant
            horizontalAngle = 90.0f + Utilities::Math::radiansToDegrees(asin(-horizontalTarget.z));
        }
    }

    //Vertical angle is limited from -90 to +90 on the yz plane so we can't look up/down into a full circle.
    verticalAngle = -Utilities::Math::radiansToDegrees(asin(target.y));
}

void Camera::updateCameraRotationAngles(const std::pair<int, int>& mouseDelta)
{
    horizontalAngle += mouseDelta.first * mouseMovementSpeedDampingFactor;
    verticalAngle += mouseDelta.second * mouseMovementSpeedDampingFactor;
}

void Camera::update(const Controls& input)
{
    if (input.isKeyPressed(ControlKeys::Forward)) {
        cameraWorldPosition.z -= keyboardMovementSpeed;
    }

    if (input.isKeyPressed(ControlKeys::Backward)) {
        cameraWorldPosition.z += keyboardMovementSpeed;
    }

    if (input.isKeyPressed(ControlKeys::Left)) {
        cameraWorldPosition.x -= keyboardMovementSpeed;
    }

    if (input.isKeyPressed(ControlKeys::Right)) {
        cameraWorldPosition.x += keyboardMovementSpeed;
    }

    if (input.isKeyPressed(ControlKeys::Up)) {
        cameraWorldPosition.y += keyboardMovementSpeed;
    }

    if (input.isKeyPressed(ControlKeys::Down)) {
        cameraWorldPosition.y -= keyboardMovementSpeed;
    }

    updateCameraRotationAngles(input.getMouseState());

    auto target = glm::normalize(glm::rotateY(forwardVector, horizontalAngle));
    auto horizontalAxis = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), target));
    target = glm::normalize(glm::rotateX(forwardVector, verticalAngle));
    auto verticalAxis = glm::normalize(glm::cross(horizontalAxis, target));

    viewMatrix = glm::lookAt(
        cameraWorldPosition,
        // (cameraDirection + cameraWorldPosition) * cameraRotation,
        (target + cameraWorldPosition),
        // glm::vec3(0, 1, 0)
        verticalAxis
    );
}
