#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include <GL/gl3w.h>

#include <materials/material.h>

#include <vector>

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
        std::vector<DirectionalLightAttributes> directionalLightUniformAttributes;
        std::vector<PointLightAttributes> pointLightUniformAttributes;
        std::vector<SpotLightAttributes> spotLightUniformAttributes;
        GLint cameraWorldPositionUniformLocation;
    public:
        PhongMaterial(
            const std::vector<uint8_t> &vertexShaderSource,
            const std::vector<uint8_t> &geometryShaderSource,
            const std::vector<uint8_t> &fragmentShaderSource
        );
        bool addCameraPositionUniformAttribute();
        bool addDirectionalLightUniformAttributes(unsigned int numDirectionalLights);
        bool addPointLightUniformAttributes(unsigned int numPointLights);
        bool addSpotLightUniformAttributes(unsigned int numSpotLights);
        GLint getCameraPositionUniformAttribute() const;
        const std::vector<DirectionalLightAttributes> &getDirectionalLightUniforms() const;
        const std::vector<PointLightAttributes> &getPointLightUniforms() const;
        const std::vector<SpotLightAttributes> &getSpotLightUniforms() const;
};

#endif
