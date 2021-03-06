#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/gl3w.h>
#include <string>
#include <unordered_map>
#include <vector>

struct ShaderProgramStages
{
    GLuint vertexShader = 0;
    GLuint geometryShader = 0;
    GLuint fragmentShader = 0;
};

class Material
{
    private:
        ShaderProgramStages shaderStages;
        GLuint shaderProgram = 0;
        bool isValid = false;
        //Map of the variables we've enabled in the shader using glEnableVertexAttribArray after querying
        //an attribute index with glGetAttribLocation, sorted per vertex array object
        std::unordered_map<GLuint, std::vector<std::pair<std::string, GLint>>> shaderAttributes;
        std::unordered_map<std::string, GLint> uniformAttributes;

        std::vector<unsigned int> diffuseTextureUnitIndices;
        std::vector<unsigned int> specularTextureUnitIndices;
        std::vector<unsigned int> normalTextureUnitIndices;

        GLuint compileShaderProgramFromSource(const std::vector<uint8_t> &shaderSource, GLenum shaderType);
        GLuint linkShaderProgram(const ShaderProgramStages& shaderStages);
    protected:
        GLuint getShaderProgram();
    public:
        Material(
            const std::vector<uint8_t> &vertexShaderSource,
            const std::vector<uint8_t> &geometryShaderSource,
            const std::vector<uint8_t> &fragmentShaderSource
        );

        //Delete shaders, linked program
        ~Material();

        bool addAttribute(GLuint VAO, std::string attributeName);
        bool addUniformAttribute(std::string uniformName);
        void bind();
        GLint getAttribute(GLuint VAO, std::string attributeName);
        GLint getUniformAttribute(std::string uniformName);
        const std::vector<unsigned int> &getDiffuseTextureUnits() const {return diffuseTextureUnitIndices;}
        const std::vector<unsigned int> &getSpecularTextureUnits() const {return specularTextureUnitIndices;}
        const std::vector<unsigned int> &getNormalTextureUnits() const {return normalTextureUnitIndices;}
        inline bool isMaterialValid() {return isValid;}
        bool setTextureUnits(unsigned int numDiffuseTextures, unsigned int numSpecularTextures, unsigned int numNormalTextures);

        //Calls glVertexAttribPointer, but allows use of shader attribute location
        //by name. Must be called after proper VBO is bound, which needs to happen
        //externally from this class.
        //Defaults values assume non-normalized, tightly-packed data buffer
        void setGLVertexAttribPointer(
            GLuint VAO,
            std::string attributeName,
            GLint size,
            GLenum type,
            GLboolean normalized = GL_FALSE,
            GLsizei stride = 0,
            const GLvoid *glPointer = 0
        );
        void unbind();
};

#endif
