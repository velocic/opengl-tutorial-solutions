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

    GLint colorLocation = glGetUniformLocation(shaderProgram, (uniformName + ".base.color").c_str());
    GLint ambientIntensityLocation = glGetUniformLocation(shaderProgram, (uniformName + ".base.ambientIntensity").c_str());
    GLint directionLocation = glGetUniformLocation(shaderProgram, (uniformName + ".direction").c_str());
    GLint diffuseIntensityLocation = glGetUniformLocation(shaderProgram, (uniformName + ".base.diffuseIntensity").c_str());
    GLint specularIntensityLocation = glGetUniformLocation(shaderProgram, (uniformName + ".base.specularIntensity").c_str());
    GLint specularPowerLocation = glGetUniformLocation(shaderProgram, (uniformName + ".base.specularPower").c_str());

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

    GLint colorLocation = glGetUniformLocation(shaderProgram, (uniformName + ".base.color").c_str());
    GLint ambientIntensityLocation = glGetUniformLocation(shaderProgram, (uniformName + ".base.ambientIntensity").c_str());
    GLint positionLocation = glGetUniformLocation(shaderProgram, (uniformName + ".position").c_str());
    GLint diffuseIntensityLocation = glGetUniformLocation(shaderProgram, (uniformName + ".base.diffuseIntensity").c_str());
    GLint attenuationConstantUniformLocation = glGetUniformLocation(shaderProgram, (uniformName + ".attenuationConstant").c_str());
    GLint attenuationLinearUniformLocation = glGetUniformLocation(shaderProgram, (uniformName + ".attenuationLinear").c_str());
    GLint attenuationExponentialUniformLocation = glGetUniformLocation(shaderProgram, (uniformName + ".attenuationExponential").c_str());
    GLint specularIntensityLocation = glGetUniformLocation(shaderProgram, (uniformName + ".base.specularIntensity").c_str());
    GLint specularPowerLocation = glGetUniformLocation(shaderProgram, (uniformName + ".base.specularPower").c_str());

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

    pointLightUniformAttributes.insert(
        std::pair<std::string, PointLightAttributes>(uniformName, newLightAttributes)
    );

    return true;
}

bool PhongMaterial::addSpotLightUniformAttribute(std::string uniformName)
{
    if (isMaterialValid() == false) {
        std::cout << "addSpotLightUniformAttribute called on Material instance with invalid state.";
        return false;
    }

    auto shaderProgram = getShaderProgram();

    GLint colorLocation = glGetUniformLocation(shaderProgram, (uniformName + ".point.base.color").c_str());
    GLint ambientIntensityLocation = glGetUniformLocation(shaderProgram, (uniformName + ".point.base.ambientIntensity").c_str());
    GLint positionLocation = glGetUniformLocation(shaderProgram, (uniformName + ".point.position").c_str());
    GLint directionLocation = glGetUniformLocation(shaderProgram, (uniformName = ".direction").c_str());
    GLint diffuseIntensityLocation = glGetUniformLocation(shaderProgram, (uniformName + ".point.base.diffuseIntensity").c_str());
    GLint attenuationConstantUniformLocation = glGetUniformLocation(shaderProgram, (uniformName + ".point.attenuationConstant").c_str());
    GLint attenuationLinearUniformLocation = glGetUniformLocation(shaderProgram, (uniformName + ".point.attenuationLinear").c_str());
    GLint attenuationExponentialUniformLocation = glGetUniformLocation(shaderProgram, (uniformName + ".point.attenuationExponential").c_str());
    GLint specularIntensityLocation = glGetUniformLocation(shaderProgram, (uniformName + ".point.base.specularIntensity").c_str());
    GLint specularPowerLocation = glGetUniformLocation(shaderProgram, (uniformName + ".point.base.specularPower").c_str());
    GLint falloffLocation = glGetUniformLocation(shaderProgram, (uniformName + ".falloff").c_str());

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

    spotLightUniformAttributes.insert(std::make_pair(uniformName, newLightAttributes));

    return true;
}

GLint PhongMaterial::getCameraPositionUniformAttribute() const
{
    return cameraWorldPositionUniformLocation;
}

DirectionalLightAttributes PhongMaterial::getDirectionalLightUniformAttribute(std::string uniformName) const
{
    auto mapIterator = directionalLightUniformAttributes.find(uniformName);

    if (mapIterator == directionalLightUniformAttributes.end()) {
        DirectionalLightAttributes emptyAttributes = {-1, -1, -1, -1, -1, -1};
        return emptyAttributes;
    }

    return mapIterator->second;
}

PointLightAttributes PhongMaterial::getPointLightUniformAttribute(std::string uniformName) const
{
    auto mapIterator = pointLightUniformAttributes.find(uniformName);

    if (mapIterator == pointLightUniformAttributes.end()) {
        PointLightAttributes emptyAttributes = {-1, -1, -1, -1, -1, -1, -1};
        return emptyAttributes;
    }

    return mapIterator->second;
}

const std::unordered_map<std::string, DirectionalLightAttributes> &PhongMaterial::getDirectionalLightUniforms() const
{
    return directionalLightUniformAttributes;
}

const std::unordered_map<std::string, PointLightAttributes> &PhongMaterial::getPointLightUniforms() const
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

