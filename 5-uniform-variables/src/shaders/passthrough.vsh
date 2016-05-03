#version 330

layout (location = 0) in vec3 position;

uniform float scale;

void main()
{
    gl_Position = vec4(scale * position.x, scale * position.y, position.z, 1.0);
}
