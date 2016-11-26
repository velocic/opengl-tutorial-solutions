#include <lights/directional.h>

unsigned int DirectionalLightList::addDirectionalLight(glm::vec3 color, float ambientIntensity)
{
    DirectionalLight newLight = {color, ambientIntensity};
    directionalLights.push_back(newLight);
}

void DirectionalLightList::clear()
{
    directionalLights.clear();
}

void DirectionalLightList::removeLightByIndex(unsigned int lightIndex)
{
    directionalLights.erase(directionalLights.begin() + lightIndex);
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
