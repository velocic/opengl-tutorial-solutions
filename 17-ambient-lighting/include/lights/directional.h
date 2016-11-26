#ifndef DIRECTIONAL_H
#define DIRECTIONAL_H

#include <GL/gl3w.h>

#include <glm/glm.hpp>

#include <materials/phongmaterial.h>

#include <unordered_map>
#include <vector>

struct DirectionalLight
{
    glm::vec3 color;
    float ambientIntensity;
};

class DirectionalLightList
{
    private:
        std::vector<DirectionalLight> directionalLights;
    public:
        unsigned int addDirectionalLight(glm::vec3 color, float ambientIntensity);
        void clear();
        void removeLightByIndex(unsigned int lightIndex);
        void setLights(const std::unordered_map<std::string, PhongLightAttributes> &lightUniforms);
};

#endif
