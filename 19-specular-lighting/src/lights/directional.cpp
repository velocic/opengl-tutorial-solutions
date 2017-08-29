#include <lights/directional.h>
#include <iostream>

size_t DirectionalLightList::addDirectionalLight(glm::vec3 color, glm::vec3 direction, float ambientIntensity, float diffuseIntensity, float specularIntensity, float specularPower)
{
    // DirectionalLight newLight = {assignedIDCount, color, ambientIntensity};
    DirectionalLight newLight = {
        color,
        glm::normalize(direction),
        assignedIDCount,
        ambientIntensity,
        diffuseIntensity,
        specularIntensity,
        specularPower
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

void DirectionalLightList::setLights(const std::unordered_map<std::string, PhongLightAttributes> &lightUniforms, GLint cameraPositionUniform)
{
    unsigned int lightCounter = 0;

    if (camera.expired() == true) {
        std::cout << "Camera expired on Directional Light List. Each light in this list will be cleared." << std::endl;
        clear();
    }

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

        glUniform1f(
            lightUniform.second.specularIntensityUniformLocation,
            currentLight.specularIntensity
        );

        glUniform1f(
            lightUniform.second.specularPowerUniformLocation,
            currentLight.specularPower
        );

        ++lightCounter;
    }

    //TODO: The camera uniform probably shouldn't be controlled from the light list,
    //but it makes a bit of sense for now. Consider moving this out when this framework
    //is built up a bit more
    auto cameraSharedPointer = camera.lock();
    auto cameraWorldPosition = cameraSharedPointer->getWorldPosition();
    glUniform3f(
        cameraPositionUniform,
        cameraWorldPosition.x,
        cameraWorldPosition.y,
        cameraWorldPosition.z
    );
}

void DirectionalLightList::setViewingCamera(std::weak_ptr<Camera> camera)
{
    this->camera = camera;
}
