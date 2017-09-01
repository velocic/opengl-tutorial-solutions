#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl3w.h>
#include <vector>

enum class TextureFormat
{
    PNG
};

class Texture
{
    private:
        GLuint textureID; //handle to the texture data buffer on the GPU
        GLenum textureTarget; //1d, 2d, 3d, etc
    public:
        ~Texture();
        void bind(GLenum textureUnit);
        void freeResources();
        bool load(GLenum textureTarget, const std::vector<uint8_t>& imageData, TextureFormat textureFormat);
        void setTextureParams(GLuint wrapModeS, GLuint wrapModeT, GLuint minFilter, GLuint magFilter, bool generateMipmap);
};

#endif
