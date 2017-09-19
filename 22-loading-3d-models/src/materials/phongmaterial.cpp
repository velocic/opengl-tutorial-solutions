#include <materials/phongmaterial.h>
#include <iostream>

PhongMaterial::PhongMaterial(
    const std::vector<uint8_t> &vertexShaderSource,
    const std::vector<uint8_t> &geometryShaderSource,
    const std::vector<uint8_t> &fragmentShaderSource
)
    : Material(vertexShaderSource, geometryShaderSource, fragmentShaderSource)
{
}

bool PhongMaterial::addDirectionalLightUniformAttributes(unsigned int numDirectionalLights)
{
    if (isMaterialValid() == false) {
        std::cout << "addDirectionalLightUniformAttribute called on Material instance with invalid state.";
        return false;
    }

    auto shaderProgram = getShaderProgram();

    for (unsigned int i = 0; i < numDirectionalLights; ++i) {
        GLint colorLocation = glGetUniformLocation(shaderProgram, ("directionalLight[" + std::to_string(i) + "].base.color").c_str());
        GLint ambientIntensityLocation = glGetUniformLocation(shaderProgram, ("directionalLight[" + std::to_string(i) + "].base.ambientIntensity").c_str());
        GLint directionLocation = glGetUniformLocation(shaderProgram, ("directionalLight[" + std::to_string(i) + "].direction").c_str());
        GLint diffuseIntensityLocation = glGetUniformLocation(shaderProgram, ("directionalLight[" + std::to_string(i) + "].base.diffuseIntensity").c_str());
        GLint specularIntensityLocation = glGetUniformLocation(shaderProgram, ("directionalLight[" + std::to_string(i) + "].base.specularIntensity").c_str());
        GLint specularPowerLocation = glGetUniformLocation(shaderProgram, ("directionalLight[" + std::to_string(i) + "].base.specularPower").c_str());

        DirectionalLightAttributes newLightAttributes = {
            colorLocation,
            ambientIntensityLocation,
            directionLocation,
            diffuseIntensityLocation,
            specularIntensityLocation,
            specularPowerLocation
        };

        if (
            newLightAttributes.colorUniformLocation == -1 ||
            newLightAttributes.ambientIntensityUniformLocation == -1 ||
            newLightAttributes.directionUniformLocation == -1 ||
            newLightAttributes.diffuseIntensityUniformLocation == -1 ||
            newLightAttributes.specularIntensityUniformLocation == -1 ||
            newLightAttributes.specularPowerUniformLocation == -1
        ) {
            std::cout << "Cannot set Directional light in PhongMaterial. One or more uniform locations is invalid." << std::endl;
            return false;
        }


        directionalLightUniformAttributes.push_back(
            std::move(newLightAttributes)
        );
    }


    return true;
}

bool PhongMaterial::addPointLightUniformAttributes(unsigned int numPointLights)
{
    if (isMaterialValid() == false) {
        std::cout << "addPointLightUniformAttribute called on Material instance with invalid state.";
        return false;
    }

    auto shaderProgram = getShaderProgram();

    for (unsigned int i = 0; i < numPointLights; ++i) {
        GLint colorLocation = glGetUniformLocation(shaderProgram, ("pointLight[" + std::to_string(i) + "].base.color").c_str());
        GLint ambientIntensityLocation = glGetUniformLocation(shaderProgram, ("pointLight[" + std::to_string(i) + "].base.ambientIntensity").c_str());
        GLint positionLocation = glGetUniformLocation(shaderProgram, ("pointLight[" + std::to_string(i) + "].position").c_str());
        GLint diffuseIntensityLocation = glGetUniformLocation(shaderProgram, ("pointLight[" + std::to_string(i) + "].base.diffuseIntensity").c_str());
        GLint attenuationConstantUniformLocation = glGetUniformLocation(shaderProgram, ("pointLight[" + std::to_string(i) + "].attenuationConstant").c_str());
        GLint attenuationLinearUniformLocation = glGetUniformLocation(shaderProgram, ("pointLight[" + std::to_string(i) + "].attenuationLinear").c_str());
        GLint attenuationExponentialUniformLocation = glGetUniformLocation(shaderProgram, ("pointLight[" + std::to_string(i) + "].attenuationExponential").c_str());
        GLint specularIntensityLocation = glGetUniformLocation(shaderProgram, ("pointLight[" + std::to_string(i) + "].base.specularIntensity").c_str());
        GLint specularPowerLocation = glGetUniformLocation(shaderProgram, ("pointLight[" + std::to_string(i) + "].base.specularPower").c_str());

        PointLightAttributes newLightAttributes = {
            colorLocation,
            ambientIntensityLocation,
            positionLocation,
            diffuseIntensityLocation,
            attenuationConstantUniformLocation,
            attenuationLinearUniformLocation,
            attenuationExponentialUniformLocation,
            specularIntensityLocation,
            specularPowerLocation
        };

        if (
            colorLocation == -1 ||
            ambientIntensityLocation == -1 ||
            positionLocation == -1 ||
            diffuseIntensityLocation == -1 ||
            attenuationConstantUniformLocation == -1 ||
            attenuationLinearUniformLocation == -1 ||
            attenuationExponentialUniformLocation == -1 ||
            specularIntensityLocation == -1 ||
            specularPowerLocation == -1
        ) {
            std::cout << "Cannot set Point light in PhongMaterial. One or more uniform locations is invalid." << std::endl;
            return false;
        }

        pointLightUniformAttributes.push_back(std::move(newLightAttributes));
    }

    return true;
}

bool PhongMaterial::addSpotLightUniformAttributes(unsigned int numSpotLights)
{
    if (isMaterialValid() == false) {
        std::cout << "addSpotLightUniformAttribute called on Material instance with invalid state.";
        return false;
    }

    auto shaderProgram = getShaderProgram();

    for (unsigned int i = 0; i < numSpotLights; ++i) {
        GLint colorLocation = glGetUniformLocation(shaderProgram, ("spotLight[" + std::to_string(i) + "].point.base.color").c_str());
        GLint ambientIntensityLocation = glGetUniformLocation(shaderProgram, ("spotLight[" + std::to_string(i) + "].point.base.ambientIntensity").c_str());
        GLint positionLocation = glGetUniformLocation(shaderProgram, ("spotLight[" + std::to_string(i) + "].point.position").c_str());
        GLint directionLocation = glGetUniformLocation(shaderProgram, ("spotLight[" + std::to_string(i) + "].direction").c_str());
        GLint diffuseIntensityLocation = glGetUniformLocation(shaderProgram, ("spotLight[" + std::to_string(i) + "].point.base.diffuseIntensity").c_str());
        GLint attenuationConstantUniformLocation = glGetUniformLocation(shaderProgram, ("spotLight[" + std::to_string(i) + "].point.attenuationConstant").c_str());
        GLint attenuationLinearUniformLocation = glGetUniformLocation(shaderProgram, ("spotLight[" + std::to_string(i) + "].point.attenuationLinear").c_str());
        GLint attenuationExponentialUniformLocation = glGetUniformLocation(shaderProgram, ("spotLight[" + std::to_string(i) + "].point.attenuationExponential").c_str());
        GLint specularIntensityLocation = glGetUniformLocation(shaderProgram, ("spotLight[" + std::to_string(i) +"].point.base.specularIntensity").c_str());
        GLint specularPowerLocation = glGetUniformLocation(shaderProgram, ("spotLight[" + std::to_string(i) + "].point.base.specularPower").c_str());
        GLint falloffLocation = glGetUniformLocation(shaderProgram, ("spotLight[" + std::to_string(i) + "].falloff").c_str());

        SpotLightAttributes newLightAttributes = {
            colorLocation,
            ambientIntensityLocation,
            positionLocation,
            directionLocation,
            diffuseIntensityLocation,
            attenuationConstantUniformLocation,
            attenuationLinearUniformLocation,
            attenuationExponentialUniformLocation,
            specularIntensityLocation,
            specularPowerLocation,
            falloffLocation
        };

        if (
            colorLocation == -1 ||
            ambientIntensityLocation == -1 ||
            positionLocation == -1 ||
            directionLocation == -1 ||
            diffuseIntensityLocation == -1 ||
            attenuationConstantUniformLocation == -1 ||
            attenuationLinearUniformLocation == -1 ||
            attenuationExponentialUniformLocation == -1 ||
            specularIntensityLocation == -1 ||
            specularPowerLocation == -1 ||
            falloffLocation == -1
        ) {
            std::cout << "Cannot set Spotlight in PhongMaterial. One or more uniform locations is invalid." << std::endl;
            return false;
        }

        spotLightUniformAttributes.push_back(std::move(newLightAttributes));
    }

    return true;
}

GLint PhongMaterial::getCameraPositionUniformAttribute() const
{
    return cameraWorldPositionUniformLocation;
}

const std::vector<DirectionalLightAttributes> &PhongMaterial::getDirectionalLightUniforms() const
{
    return directionalLightUniformAttributes;
}

const std::vector<PointLightAttributes> &PhongMaterial::getPointLightUniforms() const
{
    return pointLightUniformAttributes;
}

const std::vector<SpotLightAttributes> &PhongMaterial::getSpotLightUniforms() const
{
    return spotLightUniformAttributes;
}

bool PhongMaterial::addCameraPositionUniformAttribute()
{
    std::string uniformName = "cameraPosition";
    cameraWorldPositionUniformLocation = glGetUniformLocation(getShaderProgram(), uniformName.c_str());

    if (cameraWorldPositionUniformLocation == -1) {
        std::cout << "error: could not find uniform " + uniformName + " in phongmaterial" << std::endl;
        return false;
    }

    return true;
}

