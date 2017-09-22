#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl3w.h>
#include <vector>

enum class TextureFormat
{
    PNG
};

enum class TextureType
{
    Diffuse,
    Specular,
    Normal,
    Invalid
};

class Texture
{
    private:
        GLuint textureID = 0; //handle to the texture data buffer on the GPU
        GLenum textureTarget = 0; //1d, 2d, 3d, etc
        TextureType type = TextureType::Invalid;
    public:
        ~Texture();
        void bind(GLenum textureUnit);
        void freeResources();
        GLuint getID() const {return textureID;}
        TextureType getType() const;
        bool load(GLenum textureTarget, const std::vector<uint8_t>& imageData, TextureFormat textureFormat, TextureType type);
        void setTextureParams(GLuint wrapModeS, GLuint wrapModeT, GLuint minFilter, GLuint magFilter, bool generateMipmap);
};

#endif
