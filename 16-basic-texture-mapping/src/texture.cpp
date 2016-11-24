#include <texture.h>
#include <lodepng.h>
#include <iostream>

Texture::~Texture()
{
    freeResources();
}

void Texture::bind(GLenum textureUnit)
{
    //Make active the texture unit we want to bind to
    //this allows us to use many textures simultaneously in the future
    glActiveTexture(textureUnit);
    glBindTexture(textureTarget, textureID);
}

void Texture::freeResources()
{
    glDeleteTextures(1, &textureID);
    textureTarget = 0;
}

bool Texture::load(GLenum textureTarget, const std::vector<uint8_t>& imageData, TextureFormat textureFormat)
{
    //Check if we've been assigned a texture handle previously. If so, free everything
    if (textureID != 0) {
        freeResources();
    }

    this->textureTarget = textureTarget;

    unsigned int width;
    unsigned int height;

    std::vector<unsigned char> decodedPNG;
    auto error = lodepng::decode(decodedPNG, width, height, imageData);

    if (error) {
        std::cout << "lodePNG decoder error: " << error << ": " << lodepng_error_text(error) << std::endl;
        return false;
    }

    //create the buffer to hold the texture data
    glGenTextures(1, &textureID);
    glBindTexture(textureTarget, textureID);

    //pass the decoded image data to the texture buffer
    glTexImage2D(
        textureTarget,
        0, //LOD level
        GL_RGBA,
        width,
        height,
        0, //border. docs.gl says this value must be 0. what?
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        decodedPNG.data()
    );

    //unbind the texture to keep the state clean
    glBindTexture(textureTarget, 0);

    return true;
}

void Texture::setTextureParams(GLuint wrapModeS, GLuint wrapModeT, GLuint minFilter, GLuint magFilter, bool generateMipmap)
{
    glBindTexture(textureTarget, textureID);

    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, wrapModeS);
    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, wrapModeT);
    glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, magFilter);

    if (generateMipmap) {
        glGenerateMipmap(textureTarget);
    }

    glBindTexture(textureTarget, 0);
}
