#include <materials/material.h>
#include <algorithm>
#include <iostream>

Material::Material(
    const std::vector<uint8_t> &vertexShaderSource,
    const std::vector<uint8_t> &geometryShaderSource,
    const std::vector<uint8_t> &fragmentShaderSource
)
{
    if (vertexShaderSource.size() > 0) {
        shaderStages.vertexShader = compileShaderProgramFromSource(vertexShaderSource, GL_VERTEX_SHADER);
    }

    if (geometryShaderSource.size() > 0) {
        shaderStages.geometryShader = compileShaderProgramFromSource(geometryShaderSource, GL_GEOMETRY_SHADER);
    }

    if (fragmentShaderSource.size() > 0) {
        shaderStages.fragmentShader = compileShaderProgramFromSource(fragmentShaderSource, GL_FRAGMENT_SHADER);
    }

    shaderProgram = linkShaderProgram(shaderStages);

    if (shaderProgram > 0) {
        isValid = true;
    }
}

Material::~Material()
{
    //Calls to glDeleteShader(0) and glDeleteProgram(0) are silently ignored, according to OGL 3.3 spec
    glDeleteShader(shaderStages.vertexShader);
    glDeleteShader(shaderStages.geometryShader);
    glDeleteShader(shaderStages.fragmentShader);

    glDeleteProgram(shaderProgram);

    isValid = false;
}

bool Material::addAttribute(GLuint VAO, std::string attributeName)
{
    if (isValid == false) {
        //TODO: Replace cout with logfile writing
        std::cout << "addAttribute called on Material instance with invalid state." << std::endl;
        return false;
    }

    if (VAO == 0) {
        std::cout << "addAttribute called on null vertex array object." << std::endl;
        return false;
    }

    glBindVertexArray(VAO);
    GLint attributeLocation = glGetAttribLocation(shaderProgram, attributeName.c_str());

    if (attributeLocation == -1) {
        return false;
    }

    shaderAttributes[VAO].emplace_back(attributeName, attributeLocation);
    glEnableVertexAttribArray(attributeLocation);
    glBindVertexArray(0);

    return true;
}

bool Material::addUniformAttribute(std::string uniformName)
{
    if (isValid == false) {
        //TODO: Replace cout with logfile writing
        std::cout << "addUniformAttribute called on Material instance with invalid state.";
        return false;
    }

    GLint uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
    
    if (uniformLocation == -1) {
        return false;
    }

    uniformAttributes.insert(
        std::pair<std::string, GLint>(uniformName, uniformLocation)
    );

    return true;
}

void Material::bind()
{
    if (isValid == false) {
        //TODO: Replace cout with logfile writing
        std::cout << "bind called on Material instance with invalid state.";
        return;
    }

    //Bind all necessary resources used by this material; Shader program,
    //textures & maps once available
    glUseProgram(shaderProgram);
}

GLint Material::getAttribute(GLuint VAO, std::string attributeName)
{
    auto attribIndex = std::find_if(
        shaderAttributes[VAO].begin(),
        shaderAttributes[VAO].end(),
        [attributeName](const auto& entry) -> bool {
            return entry.first == attributeName;
        }
    );

    if (attribIndex == std::end(shaderAttributes[VAO])) {
        return -1;
    }

    return attribIndex->second;

}

GLint Material::getUniformAttribute(std::string uniformName)
{
    auto mapIterator = uniformAttributes.find(uniformName);

    if (mapIterator == uniformAttributes.end()) {
        return -1;
    }

    return mapIterator->second;
}

void Material::setGLVertexAttribPointer(GLuint VAO, std::string attributeName, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *glPointer)
{
    if (isValid == false) {
        //TODO: Replace cout with logfile writing
        std::cout << "setGLVertexAttribPointer called on Material instance with invalid state." << std::endl;
        return;
    }

    if (VAO == 0) {
        std::cout << "setGLVertexAttribPointer called with a null vertex array object." << std::endl;
    }

    glBindVertexArray(VAO);

    glVertexAttribPointer(
        getAttribute(VAO, attributeName),
        size,
        type,
        normalized,
        stride,
        glPointer
    );

    glBindVertexArray(0);
}

bool Material::setTextureUnits(unsigned int numDiffuseTextures, unsigned int numSpecularTextures, unsigned int numNormalTextures)
{
    unsigned int textureUnitIndex = 0;

    for (unsigned int i = 0; i < numDiffuseTextures; ++i) {
        auto uniformName = "diffuse[" + std::to_string(i) + "]";
        auto isValidUniform = addUniformAttribute(uniformName);

        if (isValidUniform == false) {
            std::cout << uniformName << " is an invalid uniform name" << std::endl;
            return false;
        }

        auto texLocation = getUniformAttribute(uniformName);
        glUniform1i(texLocation, textureUnitIndex); 
        diffuseTextureUnitIndices.push_back(textureUnitIndex);
        ++textureUnitIndex;
    }

    for (unsigned int i = 0; i < numSpecularTextures; ++i) {
        auto uniformName = "specular[" + std::to_string(i) + "]";
        auto isValidUniform = addUniformAttribute(uniformName);

        if (isValidUniform == false) {
            std::cout << uniformName << " is an invalid uniform name" << std::endl;
            return false;
        }

        glUniform1i(getUniformAttribute(uniformName), textureUnitIndex);
        specularTextureUnitIndices.push_back(textureUnitIndex);
        ++textureUnitIndex;
    }

    for (unsigned int i = 0; i < numNormalTextures; ++i) {
        auto uniformName = "normal[" + std::to_string(i) + "]";
        auto isValidUniform = addUniformAttribute(uniformName);

        if (isValidUniform == false) {
            std::cout << uniformName << " is an invalid uniform name" << std::endl;
        }

        glUniform1i(getUniformAttribute(uniformName), textureUnitIndex);
        normalTextureUnitIndices.push_back(textureUnitIndex);
        ++textureUnitIndex;
    }

    return true;
}


void Material::unbind()
{
    glUseProgram(0);
    glBindVertexArray(0);
}

GLuint Material::compileShaderProgramFromSource(const std::vector<uint8_t> &shaderSource, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    const GLchar *glFormatShaderSource = reinterpret_cast<const char *>(shaderSource.data());
    const GLint shaderSourceLength = shaderSource.size();
    glShaderSource(shader, 1, &glFormatShaderSource, &shaderSourceLength);
    glCompileShader(shader);

    GLint compilationSuccess = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationSuccess);

    if (!compilationSuccess) {
        GLchar infoLog[1024] = {0};
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);

        for (int i = 0; i < 1024; ++i) {
            std::cout << infoLog[i];
        }
        std::cout << std::endl;

        return 0;
    }

    return shader;
}

GLuint Material::linkShaderProgram(const ShaderProgramStages& shaderStages)
{
    GLuint program = glCreateProgram();
    bool vertexShaderAttached = false;
    bool geometryShaderAttached = false;
    bool fragmentShaderAttached = false;

    if (shaderStages.vertexShader != 0) {
        glAttachShader(program, shaderStages.vertexShader);
        vertexShaderAttached = true;
    }

    if (shaderStages.geometryShader != 0) {
        glAttachShader(program, shaderStages.geometryShader);
        geometryShaderAttached = true;
    }

    if (shaderStages.fragmentShader != 0) {
        glAttachShader(program, shaderStages.fragmentShader);
        fragmentShaderAttached = true;
    }

    glLinkProgram(program);

    if (vertexShaderAttached) {
        glDetachShader(program, shaderStages.vertexShader);
    }

    if (geometryShaderAttached) {
        glDetachShader(program, shaderStages.geometryShader);
    }

    if (fragmentShaderAttached) {
        glDetachShader(program, shaderStages.fragmentShader);
    }

    GLint linkSuccess = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);

        for (int i = 0; i < 1024; ++i) {
            std::cout << infoLog[i];
        }
        std::cout << std::endl;

        return 0;
    }

    return program;
}

GLuint Material::getShaderProgram()
{
    return shaderProgram;
}
