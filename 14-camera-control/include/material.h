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
        GLuint VAO = 0;
        bool isValid = false;
        //Map of the variables we've enabled in the shader using glEnableVertexAttribArray after querying
        //an attribute index with glGetAttribLocation
        std::unordered_map<std::string, GLint> shaderAttributes;
        std::unordered_map<std::string, GLint> uniformAttributes;

        GLuint compileShaderProgramFromSource(const std::vector<char> &shaderSource, GLenum shaderType);
        GLuint linkShaderProgram(const ShaderProgramStages& shaderStages);
    public:
        Material(
            const std::vector<char> &vertexShaderSource,
            const std::vector<char> &geometryShaderSource,
            const std::vector<char> &fragmentShaderSource
        );

        //Delete shaders, linked program, VBO & VAO
        ~Material();

        bool addAttribute(std::string attributeName);
        bool addUniformAttribute(std::string uniformName);
        void bind();
        GLint getAttribute(std::string attributeName);
        GLint getUniformAttribute(std::string uniformName);
        inline bool isMaterialValid() {return isValid;}

        //Calls glVertexAttribPointer, but allows use of shader attribute location
        //by name. Must be called after proper VBO is bound, which needs to happen
        //externally from this class.
        //Defaults values assume non-normalized, tightly-packed data buffer
        void setGLVertexAttribPointer(
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
