#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include <GL/gl3w.h>

#include <materials/material.h>

#include <unordered_map>

struct DirectionalLightAttributes
{
    GLint colorUniformLocation;
    GLint ambientIntensityUniformLocation;
    GLint directionUniformLocation;
    GLint diffuseIntensityUniformLocation;
    GLint specularIntensityUniformLocation;
    GLint specularPowerUniformLocation;
};

struct PointLightAttributes
{
    GLint colorUniformLocation;
    GLint ambientIntensityUniformLocation;
    GLint positionUniformLocation;
    GLint diffuseIntensityUniformLocation;
    GLint attenuationConstantUniformLocation;
    GLint attenuationLinearUniformLocation;
    GLint attenuationExponentialUniformLocation;
    GLint specularIntensityUniformLocation;
    GLint specularPowerUniformLocation;
};

struct SpotLightAttributes
{
    GLint colorUniformLocation;
    GLint ambientIntensityUniformLocation;
    GLint positionUniformLocation;
    GLint directionUniformLocation;
    GLint diffuseIntensityUniformLocation;
    GLint attenuationConstantUniformLocation;
    GLint attenuationLinearUniformLocation;
    GLint attenuationExponentialUniformLocation;
    GLint specularIntensityUniformLocation;
    GLint specularPowerUniformLocation;
    GLint falloffUniformLocation;
};

class PhongMaterial : public Material
{
    private:
        std::unordered_map<std::string, DirectionalLightAttributes> directionalLightUniformAttributes;
        std::unordered_map<std::string, PointLightAttributes> pointLightUniformAttributes;
        std::unordered_map<std::string, SpotLightAttributes> spotLightUniformAttributes;
        GLint cameraWorldPositionUniformLocation;
    public:
        PhongMaterial(
            const std::vector<uint8_t> &vertexShaderSource,
            const std::vector<uint8_t> &geometryShaderSource,
            const std::vector<uint8_t> &fragmentShaderSource
        );
        bool addCameraPositionUniformAttribute(std::string uniformName);
        bool addDirectionalLightUniformAttribute(std::string uniformName);
        bool addPointLightUniformAttribute(std::string uniformName);
        bool addSpotLightUniformAttribute(std::string uniformName);
        GLint getCameraPositionUniformAttribute() const;
        DirectionalLightAttributes getDirectionalLightUniformAttribute(std::string uniformName) const;
        PointLightAttributes getPointLightUniformAttribute(std::string uniformName) const;
        SpotLightAttributes getSpotLightUniformAttribute(std::string uniformName) const;
        const std::unordered_map<std::string, DirectionalLightAttributes> &getDirectionalLightUniforms() const;
        const std::unordered_map<std::string, PointLightAttributes> &getPointLightUniforms() const;
        const std::unordered_map<std::string, SpotLightAttributes> &getSpotLightUniforms() const;
};

#endif
