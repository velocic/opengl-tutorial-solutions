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
    float ambientIntensity = 0.0;
    float diffuseIntensity = 0.0;
    float specularIntensity = 0.0;
    float specularPower = 0.0;
    size_t id = 0;
};

struct DirectionalLight : Light
{
    glm::vec3 direction;
};

struct PointLight : Light
{
    glm::vec3 position;
    float attenuationConstant = 0.0;
    float attenuationLinear = 0.0;
    float attenuationExponential = 0.0;
};

struct SpotLight : PointLight
{
    glm::vec3 direction;
    float falloff = 0.0;
};

class LightList
{
    private:
        std::vector<DirectionalLight> directionalLights;
        std::vector<PointLight> pointLights;
        std::vector<SpotLight> spotLights;
        size_t assignedIDCount = 0;
        std::weak_ptr<Camera> camera;
    public:
        LightList(std::weak_ptr<Camera> camera)
           : camera(camera)
        {}

        size_t addDirectionalLight(
            glm::vec3 color,
            glm::vec3 direction,
            float ambientIntensity,
            float diffuseIntensity,
            float specularIntensity,
            float specularPower
        );
        size_t addPointLight(
            glm::vec3 color,
            glm::vec3 position,
            float ambientIntensity,
            float diffuseIntensity,
            float specularIntensity,
            float specularPower,
            float attenuationConstant,
            float attenuationLinear,
            float attenuationExponential
        );
        size_t addSpotLight(
            glm::vec3 color,
            glm::vec3 position,
            glm::vec3 direction,
            float ambientIntensity,
            float diffuseIntensity,
            float specularIntensity,
            float specularPower,
            float attenuationConstant,
            float attenuationLinear,
            float attenuationExponential,
            float falloff
        );
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
