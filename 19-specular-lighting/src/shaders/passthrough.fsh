#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

out vec4 fragColor;

struct DirectionalLight
{
    vec3 color;
    vec3 direction;
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
    float specularPower;
};

uniform DirectionalLight directionalLight;
uniform vec3 cameraPosition;
uniform sampler2D sampler;

void main()
{
    vec4 ambientColor = vec4(directionalLight.color * directionalLight.ambientIntensity, 1.0f);
    vec3 lightDirection = -directionalLight.direction;
    vec3 normal = normalize(normal0);

    //Reverse the direction of the light vector, so it points from fragment->light source
    float diffuseFactor = dot(normal, lightDirection);
    vec4 diffuseColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    //If the surface was struck at an angle [0, 90)
    if (diffuseFactor > 0) {
        diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

        vec3 vertexToCamera = normalize(cameraPosition - worldPos0);
        vec3 lightReflection = normalize(reflect(directionalLight.direction, normal));
        float specularFactor = dot(vertexToCamera, lightReflection);

        if (specularFactor > 0) {
            specularFactor = pow(specularFactor, directionalLight.specularPower);
            specularColor = vec4(directionalLight.color * directionalLight.specularIntensity * specularFactor, 1.0f);
        }
    }

    fragColor = texture2D(sampler, texCoord0.xy) * (ambientColor + diffuseColor + specularColor);
}
