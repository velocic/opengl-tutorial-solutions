#version 330

in vec2 texCoord0;
in vec3 normal0;

out vec4 fragColor;

struct DirectionalLight
{
    vec3 color;
    vec3 direction;
    float ambientIntensity;
    float diffuseIntensity;
};

uniform DirectionalLight directionalLight;
uniform sampler2D sampler;

void main()
{
    vec4 ambientColor = vec4(directionalLight.color * directionalLight.ambientIntensity, 1.0f);

    //Reverse the direction of the light vector, so it points from fragment->light source
    float diffuseFactor = dot(normalize(normal0), -directionalLight.direction);
    vec4 diffuseColor = vec4(0, 0, 0, 0);

    //If the surface was struck at an angle [0, 90)
    if (diffuseFactor > 0) {
        diffuseColor = vec4(directionalLight.color * directionalLight.diffuseIntensity * diffuseFactor, 1.0f);
    }

    fragColor = texture2D(sampler, texCoord0.xy) * (ambientColor + diffuseColor);
}
