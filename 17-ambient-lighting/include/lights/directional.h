#ifndef DIRECTIONAL_H
#define DIRECTIONAL_H

#include <GL/gl3w.h>

#include <glm/glm.hpp>

#include <materials/phongmaterial.h>

#include <unordered_map>
#include <vector>

struct DirectionalLight
{
    size_t id;
    glm::vec3 color;
    float ambientIntensity;
};

class DirectionalLightList
{
    private:
        std::vector<DirectionalLight> directionalLights;
        size_t assignedIDCount = 0;
    public:
        size_t addDirectionalLight(glm::vec3 color, float ambientIntensity);
        void clear();
        //Note: don't keep these pointers around for long after retrieval
        //They are potentially invalidated after every vector insert/delete
        DirectionalLight *getLightByID(unsigned int lightID);
        void removeLightByID(unsigned int lightID);
        void setLights(const std::unordered_map<std::string, PhongLightAttributes> &lightUniforms);
};

#endif
