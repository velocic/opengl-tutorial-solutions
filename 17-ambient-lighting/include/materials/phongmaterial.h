#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include <GL/gl3w.h>

#include <materials/material.h>

#include <unordered_map>

struct PhongLightAttributes
{
    GLint colorUniformLocation;
    GLint ambientIntensityUniformLocation;
};

class PhongMaterial : public Material
{
    private:
        std::unordered_map<std::string, PhongLightAttributes> lightUniformAttributes;
    public:
        PhongMaterial(
            const std::vector<uint8_t> &vertexShaderSource,
            const std::vector<uint8_t> &geometryShaderSource,
            const std::vector<uint8_t> &fragmentShaderSource
        );
        bool addLightUniformAttribute(std::string uniformName);
        PhongLightAttributes getLightUniformAttribute(std::string uniformName);
        const std::unordered_map<std::string, PhongLightAttributes> &getLightUniforms();
};

#endif
