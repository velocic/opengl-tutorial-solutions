#version 330

in vec2 texCoord0;

out vec4 fragColor;

struct DirectionalLight
{
    vec3 color;
    float ambientIntensity;
};

uniform DirectionalLight directionalLight;
uniform sampler2D sampler;

void main()
{
    fragColor = texture2D(sampler, texCoord0.st) *
                vec4(directionalLight.color, 1.0f) *
                directionalLight.ambientIntensity;
}
