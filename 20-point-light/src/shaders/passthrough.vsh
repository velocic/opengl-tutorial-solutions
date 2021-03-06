#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 WVPMatrix;
uniform mat4 worldMatrix;

out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;

void main()
{
    texCoord0 = texCoord;
    normal0 = (worldMatrix * vec4(normal, 0.0f)).xyz;
    worldPos0 = (worldMatrix * vec4(position, 1.0f)).xyz;
    gl_Position = WVPMatrix * vec4(position, 1.0f);
}
