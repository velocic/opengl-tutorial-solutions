#include <lights/directional.h>

size_t DirectionalLightList::addDirectionalLight(glm::vec3 color, float ambientIntensity)
{
    DirectionalLight newLight = {assignedIDCount, color, ambientIntensity};
    directionalLights.push_back(newLight);
    ++assignedIDCount;

    return assignedIDCount - 1;
}

void DirectionalLightList::clear()
{
    directionalLights.clear();
}

DirectionalLight *DirectionalLightList::getLightByID(unsigned int lightID)
{
    for (auto it = directionalLights.begin(); it != directionalLights.end(); ++it) {
        if (it->id == lightID) {
            return &(*it);
        }
    }

    return nullptr;
}

void DirectionalLightList::removeLightByID(unsigned int lightID)
{
    unsigned int positionCount = 0;
    for (auto directionalLight : directionalLights) {
        if (directionalLight.id == lightID) {
            directionalLights.erase(directionalLights.begin() + positionCount);
            return;
        }
        ++positionCount;
    }
}

void DirectionalLightList::setLights(const std::unordered_map<std::string, PhongLightAttributes> &lightUniforms)
{
    unsigned int lightCounter = 0;
    for (auto& lightUniform : lightUniforms) {
        glUniform3f(
            lightUniform.second.colorUniformLocation,
            directionalLights[lightCounter].color.r,
            directionalLights[lightCounter].color.g,
            directionalLights[lightCounter].color.b
        );

        glUniform1f(
            lightUniform.second.ambientIntensityUniformLocation, 
            directionalLights[lightCounter].ambientIntensity
        );

        ++lightCounter;
    }
}
