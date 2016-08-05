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
    horizontalAngle -= mouseDelta.first * mouseMovementSpeedDampingFactor;
    verticalAngle -= mouseDelta.second * mouseMovementSpeedDampingFactor;
}

void Camera::update(const Controls& input)
{
    updateCameraRotationAngles(input.getMouseState());

    auto target = glm::normalize(glm::rotateY(forwardVector, horizontalAngle));
    auto horizontalAxis = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), target));
    target = glm::normalize(glm::rotateX(target, verticalAngle));
    auto verticalAxis = glm::normalize(glm::cross(horizontalAxis, target));

    if (input.isKeyPressed(ControlKeys::Forward)) {
        cameraWorldPosition += target * keyboardMovementSpeed;
    }

    if (input.isKeyPressed(ControlKeys::Backward)) {
        cameraWorldPosition -= target * keyboardMovementSpeed;
    }

    if (input.isKeyPressed(ControlKeys::Left)) {
        auto left = glm::normalize(glm::cross(target, verticalAxis));
        left *= keyboardMovementSpeed;
        cameraWorldPosition += left;
    }

    if (input.isKeyPressed(ControlKeys::Right)) {
        auto right = glm::normalize(glm::cross(verticalAxis, target));
        right *= keyboardMovementSpeed;
        cameraWorldPosition += right;
    }

    if (input.isKeyPressed(ControlKeys::Up)) {
        auto up = glm::normalize(glm::cross(target, horizontalAxis));
        up *= keyboardMovementSpeed;
        cameraWorldPosition += up;
    }

    if (input.isKeyPressed(ControlKeys::Down)) {
        auto down = glm::normalize(glm::cross(horizontalAxis, target));
        down *= keyboardMovementSpeed;
        cameraWorldPosition += down;
    }

    viewMatrix = glm::lookAt(
        cameraWorldPosition,
        (target + cameraWorldPosition),
        glm::vec3(0.0, 1.0, 0.0)
    );
}
