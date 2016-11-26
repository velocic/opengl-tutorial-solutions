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

bool PhongMaterial::addLightUniformAttribute(std::string uniformName)
{
    if (isMaterialValid() == false) {
        std::cout << "addUniformAttribute called on Material instance with invalid state.";
        return false;
    }

    GLint colorLocation = glGetUniformLocation(getShaderProgram(), (uniformName + ".color").c_str());
    GLint ambientIntensityLocation = glGetUniformLocation(getShaderProgram(), (uniformName + ".ambientIntensity").c_str());

    PhongLightAttributes newLightAttributes = {colorLocation, ambientIntensityLocation};

    if (newLightAttributes.colorUniformLocation == -1 || newLightAttributes.ambientIntensityUniformLocation == -1) {
        return false;
    }


    lightUniformAttributes.insert(
        std::pair<std::string, PhongLightAttributes>(uniformName, newLightAttributes)
    );
}

PhongLightAttributes PhongMaterial::getLightUniformAttribute(std::string uniformName)
{
    auto mapIterator = lightUniformAttributes.find(uniformName);

    if (mapIterator == lightUniformAttributes.end()) {
        PhongLightAttributes emptyAttributes = {-1, -1};
        return emptyAttributes;
    }

    return mapIterator->second;
}

const std::unordered_map<std::string, PhongLightAttributes> &PhongMaterial::getLightUniforms()
{
    return lightUniformAttributes;
}
