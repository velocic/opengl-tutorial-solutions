#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 WVPMatrix;
uniform mat4 worldMatrix;

out vec2 texCoord0;
out vec3 normal0;

void main()
{
    gl_Position = WVPMatrix * vec4(position, 1);
    texCoord0 = texCoord;
    normal0 = (worldMatrix * vec4(normal, 0.0)).xyz;
}
