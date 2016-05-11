#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <controls.h>

class Camera
{
    private:
        glm::mat4 viewMatrix;
        glm::vec3 cameraWorldPosition = glm::vec3(0, 0, 0);
        glm::vec3 cameraDirection = glm::vec3(0, 0, -1);
        double movementSpeed = .1;

    public:
        Camera(glm::mat4 defaultViewMatrix) :
            viewMatrix(defaultViewMatrix)
        {}

        void update(const Controls& input);
        inline glm::mat4 getViewMatrix() {return viewMatrix;}
};

#endif
