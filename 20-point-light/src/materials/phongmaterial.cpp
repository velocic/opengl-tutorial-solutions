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

bool PhongMaterial::addDirectionalLightUniformAttribute(std::string uniformName)
{
    if (isMaterialValid() == false) {
        std::cout << "addDirectionalLightUniformAttribute called on Material instance with invalid state.";
        return false;
    }

    auto shaderProgram = getShaderProgram();

    GLint colorLocation = glGetUniformLocation(shaderProgram, (uniformName + ".color").c_str());
    GLint ambientIntensityLocation = glGetUniformLocation(shaderProgram, (uniformName + ".ambientIntensity").c_str());
    GLint directionLocation = glGetUniformLocation(shaderProgram, (uniformName + ".direction").c_str());
    GLint diffuseIntensityLocation = glGetUniformLocation(shaderProgram, (uniformName + ".diffuseIntensity").c_str());
    GLint specularIntensityLocation = glGetUniformLocation(shaderProgram, (uniformName + ".specularIntensity").c_str());
    GLint specularPowerLocation = glGetUniformLocation(shaderProgram, (uniformName + ".specularPower").c_str());

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


    directionalLightUniformAttributes.insert(
        std::pair<std::string, DirectionalLightAttributes>(uniformName, newLightAttributes)
    );

    return true;
}

bool PhongMaterial::addPointLightUniformAttribute(std::string uniformName)
{
    if (isMaterialValid() == false) {
        std::cout << "addPointLightUniformAttribute called on Material instance with invalid state.";
        return false;
    }

    auto shaderProgram = getShaderProgram();

    GLint colorLocation = glGetUniformLocation(shaderProgram, (uniformName + ".color").c_str());
    GLint ambientIntensityLocation = glGetUniformLocation(shaderProgram, (uniformName + ".ambientIntensity").c_str());
    GLint positionLocation = glGetUniformLocation(shaderProgram, (uniformName + ".position").c_str());
    GLint diffuseIntensityLocation = glGetUniformLocation(shaderProgram, (uniformName + ".diffuseIntensity").c_str());
    GLint attenuationConstantUniformLocation = glGetUniformLocation(shaderProgram, (uniformName + ".attenuationConstant").c_str());
    GLint attenuationLinearUniformLocation = glGetUniformLocation(shaderProgram, (uniformName + ".attenuationLinear").c_str());
    GLint attenuationExponentialUniformLocation = glGetUniformLocation(shaderProgram, (uniformName + ".attenuationExponential").c_str());

    PointLightAttributes newLightAttributes = {
        colorLocation,
        ambientIntensityLocation,
        positionLocation,
        diffuseIntensityLocation,
        attenuationConstantUniformLocation,
        attenuationLinearUniformLocation,
        attenuationExponentialUniformLocation
    };

    if (
        colorLocation == -1 ||
        ambientIntensityLocation == -1 ||
        positionLocation == -1 ||
        diffuseIntensityLocation == -1 ||
        attenuationConstantUniformLocation == -1 ||
        attenuationLinearUniformLocation == -1 ||
        attenuationExponentialUniformLocation == -1
    ) {
        std::cout << "Cannot set Point light in PhongMaterial. One or more uniform locations is invalid." << std::endl;
        return false;
    }

    pointLightUniformAttributes.insert(
        std::pair<std::string, PointLightAttributes>(uniformName, newLightAttributes)
    );

    return true;
}

GLint PhongMaterial::getCameraPositionUniformAttribute()
{
    return cameraWorldPositionUniformLocation;
}

DirectionalLightAttributes PhongMaterial::getDirectionalLightUniformAttribute(std::string uniformName)
{
    auto mapIterator = directionalLightUniformAttributes.find(uniformName);

    if (mapIterator == directionalLightUniformAttributes.end()) {
        DirectionalLightAttributes emptyAttributes = {-1, -1, -1, -1, -1, -1};
        return emptyAttributes;
    }

    return mapIterator->second;
}

PointLightAttributes PhongMaterial::getPointLightUniformAttribute(std::string uniformName)
{
    auto mapIterator = pointLightUniformAttributes.find(uniformName);

    if (mapIterator == pointLightUniformAttributes.end()) {
        PointLightAttributes emptyAttributes = {-1, -1, -1, -1, -1, -1, -1};
        return emptyAttributes;
    }

    return mapIterator->second;
}

const std::unordered_map<std::string, DirectionalLightAttributes> &PhongMaterial::getDirectionalLightUniforms()
{
    return directionalLightUniformAttributes;
}

const std::unordered_map<std::string, PointLightAttributes> &PhongMaterial::getPointLightUniforms()
{
    return pointLightUniformAttributes;
}

bool PhongMaterial::addCameraPositionUniformAttribute(std::string uniformName)
{
    cameraWorldPositionUniformLocation = glGetUniformLocation(getShaderProgram(), uniformName.c_str());

    if (cameraWorldPositionUniformLocation == -1) {
        return false;
    }

    return true;
}

