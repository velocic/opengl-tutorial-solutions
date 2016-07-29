#include <camera.h>

Camera::Camera(glm::mat4 defaultViewMatrix) :
    viewMatrix(defaultViewMatrix)
{
}

void Camera::calculateRotation()
{
    auto target = cameraDirection + cameraWorldPosition;

    //Vector parallel to the xz plane
    auto horizontalTarget = glm::vec3(target.x, 0.0, target.z);

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
    calculateRotation();

    viewMatrix = glm::lookAt(
        cameraWorldPosition,
        (cameraDirection + cameraWorldPosition) * cameraRotation,
        glm::vec3(0, 1, 0)
    );
}
