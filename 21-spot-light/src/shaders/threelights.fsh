#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

out vec4 fragColor;

const int numDirectionalLights = 1;
const int numPointLights = 1;
const int numSpotLights = 2;

struct Light
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
    float specularPower;
};

struct DirectionalLight
{
    Light base;
    vec3 direction;
};

struct PointLight
{
    Light base;
    vec3 position;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationExponential;
};

struct SpotLight
{
    PointLight point;
    vec3 direction;
    float falloff;
};

uniform DirectionalLight directionalLight[numDirectionalLights];
uniform PointLight pointLight[numPointLights];
uniform SpotLight spotLight[numSpotLights];
uniform vec3 cameraPosition;
uniform sampler2D sampler;

vec4 calculateBaseLight(Light light, vec3 fragmentToLightDirection, vec3 normal)
{
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

    float diffuseFactor = dot(normal, fragmentToLightDirection);
    vec4 diffuseColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    //If the surface was struck at an angle [0, 90)
    if (diffuseFactor > 0) {
        diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

        vec3 vertexToCamera = normalize(cameraPosition - worldPos0);
        vec3 lightReflection = normalize(reflect(fragmentToLightDirection, normal));
        float specularFactor = dot(vertexToCamera, lightReflection);

        if (specularFactor > 0) {
            specularFactor = pow(specularFactor, light.specularPower);
            specularColor = vec4(light.color * light.specularIntensity * specularFactor, 1.0f);
        }
    }

    return (ambientColor + diffuseColor + specularColor);
}

vec4 calculateDirectionalLight(DirectionalLight light, vec3 fragmentToLightDirection, vec3 normal)
{
    return calculateBaseLight(light.base, fragmentToLightDirection, normal);
}

vec4 calculatePointLight(PointLight light, vec3 fragmentWorldPos, vec3 normal)
{
    vec3 fragmentToLightDirection = fragmentWorldPos - light.position;
    float fragmentLightDistance = length(fragmentToLightDirection);
    fragmentToLightDirection = normalize(fragmentToLightDirection);

    vec4 color = calculateBaseLight(light.base, fragmentToLightDirection, normal);
    float attenuation = light.attenuationConstant +
                        light.attenuationLinear * fragmentLightDistance +
                        light.attenuationExponential * fragmentLightDistance * fragmentLightDistance;

    return color / attenuation;
}

vec4 calculateSpotLight(SpotLight light, vec3 fragmentWorldPos, vec3 normal)
{
    vec3 fragmentToLight = normalize(fragmentWorldPos - light.point.position);
    float spotFactor = dot(fragmentToLight, light.direction);

    if (spotFactor > light.falloff) {
        vec4 color = calculatePointLight(light.point, fragmentWorldPos, normal);
        return color * (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - light.falloff));
    } else {
        return vec4(0, 0, 0, 0);
    }
}

void main()
{
    vec3 normal = normalize(normal0);

    //Reverse the direction of the light vector, so it points from fragment->light source

    vec4 directionalLightColor = vec4(0, 0, 0, 0);
    for (int i = 0; i < numDirectionalLights; ++i) {
        vec3 fragmentToDirectionalLightDirection = -directionalLight[i].direction;
        directionalLightColor += calculateDirectionalLight(directionalLight[i], fragmentToDirectionalLightDirection, normal);
    }

    vec4 pointLightColor = vec4(0, 0, 0, 0);
    for (int i = 0; i < numPointLights; ++i) {
        pointLightColor += calculatePointLight(pointLight[i], worldPos0, normal);
    }

    vec4 spotLightColor = vec4(0, 0, 0, 0);
    for (int i = 0; i < numSpotLights; ++i) {
        spotLightColor += calculateSpotLight(spotLight[i], worldPos0, normal);
    }

    vec4 finalLightColor = directionalLightColor + pointLightColor + spotLightColor;

    fragColor = texture2D(sampler, texCoord0.xy) * finalLightColor;
}
