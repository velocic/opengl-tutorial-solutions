#ifndef DIRECTIONAL_H
#define DIRECTIONAL_H

#include <GL/gl3w.h>

#include <glm/glm.hpp>

#include <camera.h>
#include <materials/phongmaterial.h>

#include <memory>
#include <unordered_map>
#include <vector>

struct DirectionalLight
{
    glm::vec3 color;
    glm::vec3 direction;
    size_t id;
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
    float specularPower;
};

class DirectionalLightList
{
    private:
        std::vector<DirectionalLight> directionalLights;
        size_t assignedIDCount = 0;
        std::weak_ptr<Camera> camera;
    public:
        DirectionalLightList(std::weak_ptr<Camera> camera)
           : camera(camera)
        {}

        size_t addDirectionalLight(glm::vec3 color, glm::vec3 direction, float ambientIntensity, float diffuseIntensity, float specularIntensity, float specularPower);
        void clear();
        //Note: don't keep these pointers around for long after retrieval
        //They are potentially invalidated after every vector insert/delete
        DirectionalLight *getLightByID(unsigned int lightID);
        void removeLightByID(unsigned int lightID);
        void setLights(const std::unordered_map<std::string, PhongLightAttributes> &lightUniforms, GLint cameraPositionUniform);
        void setViewingCamera(std::weak_ptr<Camera> camera);
};

#endif
