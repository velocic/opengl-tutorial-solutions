#ifndef LIGHTLIST_H
#define LIGHTLIST_H

#include <GL/gl3w.h>

#include <glm/glm.hpp>

#include <camera.h>
#include <materials/phongmaterial.h>

#include <memory>
#include <unordered_map>
#include <vector>

struct Light
{
    glm::vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight : Light
{
    glm::vec3 direction;
    size_t id;
    float specularIntensity;
    float specularPower;
};

struct PointLight : Light
{
    glm::vec3 position;
    size_t id;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationExponential;
};

class LightList
{
    private:
        std::vector<DirectionalLight> directionalLights;
        std::vector<PointLight> pointLights;
        size_t assignedIDCount = 0;
        std::weak_ptr<Camera> camera;
    public:
        LightList(std::weak_ptr<Camera> camera)
           : camera(camera)
        {}

        size_t addDirectionalLight(glm::vec3 color, glm::vec3 direction, float ambientIntensity, float diffuseIntensity, float specularIntensity, float specularPower);
        size_t addPointLight(glm::vec3 color, glm::vec3 position, float ambientIntensity, float diffuseIntensity, float attenuationConstant, float attenuationLinear, float attenuationExponential);
        void clear();
        //Note: don't keep these pointers around for long after retrieval
        //They are potentially invalidated after every vector insert/delete
        Light *getLightByID(unsigned int lightID);
        void removeLightByID(unsigned int lightID);
        void setLights(
            const std::unordered_map<std::string, DirectionalLightAttributes> &directionalLightUniforms,
            const std::unordered_map<std::string, PointLightAttributes> &pointLightUniforms,
            GLint cameraPositionUniform
        );
        void setViewingCamera(std::weak_ptr<Camera> camera);
};

#endif
