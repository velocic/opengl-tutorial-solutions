#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <controls.h>
#include <utilities.h>

#include <cmath>

class Camera
{
    private:
        glm::mat4 viewMatrix;
        glm::vec3 cameraWorldPosition = glm::vec3(0, 0, 5);
        glm::vec3 forwardVector = glm::vec3(0, 0, -1);
        float keyboardMovementSpeed = .1;
        float mouseMovementSpeedDampingFactor = .01;
        unsigned int mousePositionX;
        unsigned int mousePositionY;
        float horizontalAngle;
        float verticalAngle;

        void initCameraRotationAngles();
        void updateCameraRotationAngles(const std::pair<int, int>& mouseDelta);

    public:
        Camera(glm::mat4 defaultViewMatrix);

        void update(const Controls& input);
        inline glm::mat4 getViewMatrix() const {return viewMatrix;}
};

#endif
