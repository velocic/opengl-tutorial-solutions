#version 330

layout (location = 0) in vec3 position;

uniform mat4 worldPosition;

void main()
{
    /* gl_Position = vec4(position.x, position.y, position.z, 1.0); */
    gl_Position = worldPosition * vec4(position, 1);
}
