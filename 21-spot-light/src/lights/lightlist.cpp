#include <lights/lightlist.h>
#include <iostream>

size_t LightList::addDirectionalLight(
    glm::vec3 color,
    glm::vec3 direction,
    float ambientIntensity,
    float diffuseIntensity,
    float specularIntensity,
    float specularPower
)
{
    DirectionalLight newLight;
    newLight.color = color;
    newLight.direction = glm::normalize(direction);
    newLight.id = assignedIDCount;
    newLight.ambientIntensity = ambientIntensity;
    newLight.diffuseIntensity = diffuseIntensity;
    newLight.specularIntensity = specularIntensity;
    newLight.specularPower = specularPower;

    directionalLights.push_back(newLight);
    ++assignedIDCount;

    return assignedIDCount - 1;
}

size_t LightList::addPointLight(
    glm::vec3 color,
    glm::vec3 position,
    float ambientIntensity,
    float diffuseIntensity,
    float specularIntensity,
    float specularPower,
    float attenuationConstant,
    float attenuationLinear,
    float attenuationExponential
)
{
    PointLight newLight;
    newLight.color = color;
    newLight.position = position;
    newLight.id = assignedIDCount;
    newLight.ambientIntensity = ambientIntensity;
    newLight.diffuseIntensity = diffuseIntensity;
    newLight.attenuationConstant = attenuationConstant;
    newLight.attenuationLinear = attenuationLinear;
    newLight.attenuationExponential = attenuationExponential;
    newLight.specularIntensity = specularIntensity;
    newLight.specularPower = specularPower;

    pointLights.push_back(newLight);
    ++assignedIDCount;

    return assignedIDCount - 1;
}

void LightList::clear()
{
    directionalLights.clear();
}

Light *LightList::getLightByID(unsigned int lightID)
{
    for (auto it = directionalLights.begin(); it != directionalLights.end(); ++it) {
        if (it->id == lightID) {
            return &(*it);
        }
    }

    for (auto it = pointLights.begin(); it != pointLights.end(); ++it) {
        if (it->id == lightID) {
            return &(*it);
        }
    }

    return nullptr;
}

void LightList::removeLightByID(unsigned int lightID)
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

void LightList::setLights(
    const std::unordered_map<std::string, DirectionalLightAttributes> &directionalLightUniforms,
    const std::unordered_map<std::string, PointLightAttributes> &pointLightUniforms,
    GLint cameraPositionUniform
)
{
    unsigned int lightCounter = 0;

    if (camera.expired() == true) {
        std::cout << "Camera expired on Directional Light List. Each light in this list will be cleared." << std::endl;
        clear();
    }

    //Set the uniform variables for each directional light
    for (auto& lightUniform : directionalLightUniforms) {
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

    lightCounter = 0;
    //Set the uniform variables for each point light
    for (auto& lightUniform : pointLightUniforms) {
        auto& currentLight = pointLights[lightCounter];

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

        glUniform1f(
            lightUniform.second.diffuseIntensityUniformLocation,
            currentLight.diffuseIntensity
        );

        glUniform3f(
            lightUniform.second.positionUniformLocation,
            currentLight.position.x,
            currentLight.position.y,
            currentLight.position.z
        );

        glUniform1f(
            lightUniform.second.attenuationConstantUniformLocation,
            currentLight.attenuationConstant
        );

        glUniform1f(
            lightUniform.second.attenuationLinearUniformLocation,
            currentLight.attenuationLinear
        );

        glUniform1f(
            lightUniform.second.attenuationExponentialUniformLocation,
            currentLight.attenuationExponential
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

void LightList::setViewingCamera(std::weak_ptr<Camera> camera)
{
    this->camera = camera;
}
