#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <controls.h>

#include <cmath>

class Camera
{
    private:
        glm::mat4 viewMatrix;
        glm::vec3 cameraWorldPosition = glm::vec3(0, 0, 0);
        glm::vec3 cameraForward = glm::vec3(0, 0, -1);
        glm::quat cameraRotation;
        double movementSpeed = .1;
        unsigned int windowWidth;
        unsigned int windowHeight;
        unsigned int mousePositionX;
        unsigned int mousePositionY;

        void calculateRotation();

    public:
        Camera(glm::mat4 defaultViewMatrix, unsigned int windowWidth, unsigned int windowHeight);

        void update(const Controls& input);
        inline glm::mat4 getViewMatrix() {return viewMatrix;}
};

#endif
