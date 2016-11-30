#include <lights/directional.h>

size_t DirectionalLightList::addDirectionalLight(glm::vec3 color, glm::vec3 direction, float ambientIntensity, float diffuseIntensity)
{
    // DirectionalLight newLight = {assignedIDCount, color, ambientIntensity};
    DirectionalLight newLight = {
        color,
        glm::normalize(direction),
        assignedIDCount,
        ambientIntensity,
        diffuseIntensity
    };
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
        auto& currentLight = directionalLights[lightCounter];
        glUniform3f(
            lightUniform.second.colorUniformLocation,
            currentLight.color.r,
            currentLight.color.g,
            currentLight.color.b
        );

        glUniform1f(
            lightUniform.second.ambientIntensityUniformLocation, 
            currentLight.ambientIntensity
        );

        glUniform3f(
            lightUniform.second.directionUniformLocation,
            currentLight.direction.x,
            currentLight.direction.y,
            currentLight.direction.z
        );

        glUniform1f(
            lightUniform.second.diffuseIntensityUniformLocation,
            currentLight.diffuseIntensity
        );

        ++lightCounter;
    }
}
