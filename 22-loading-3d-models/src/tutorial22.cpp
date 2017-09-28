#include <GL/gl3w.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <camera.h>
#include <controls.h>
#include <lights/lightlist.h>
#include <asset/mesh.h>
#include <materials/phongmaterial.h>
#include <renderwindow.h>
#include <texture.h>
#include <utilities.h>

const unsigned int screenWidth = 800;
const unsigned int screenHeight = 600;

void calculateNormals(
    const std::vector<GLuint> &indices,
    std::vector<Vertex> &vertices
)
{
    for (unsigned int i = 0; i < indices.size(); i += 3) {
        auto index0 = indices[i];
        auto index1 = indices[i + 1];
        auto index2 = indices[i + 2];

        glm::vec3 v1 = vertices[index1].position - vertices[index0].position;
        glm::vec3 v2 = vertices[index2].position - vertices[index0].position;
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        vertices[index0].normal += normal;
        vertices[index1].normal += normal;
        vertices[index2].normal += normal;
    }

    for (unsigned int i = 0; i < vertices.size(); ++i) {
        vertices[i].normal = glm::normalize(vertices[i].normal);
    }
}

int main()
{
    //Initialize SDL2 with video and event modes enabled,
    //and request an OpenGL 3.3 Core Profile
    RenderWindow renderWindow("ogldev.atspace.co.uk Tutorial 22 - Loading 3D models using Open Asset Import Library", screenWidth, screenHeight);

    //Vector, UV, normal
    std::vector<Vertex> vertices = {
        {glm::vec3(-10.0f, -10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.0f, -10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f, 0.0f)},
        {glm::vec3(10.0f, -10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f, 1.0f)}
    };

    //Note: the order of indices for each vertex matters. They are used to determine the "facing" of the traingle.
    //These are specified properly in clockwise order, but mix it up and you'll get faces culled
    //because of the backface culling operation
    std::vector<GLuint> indices = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    //Create an index buffer to share vertices among triangles
    GLuint IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    calculateNormals(indices, vertices);

    //Allocate a buffer for our vertices
    GLuint VBO = 0;
    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::string vertexShaderPath = "../src/shaders/passthrough.vsh";
    std::string fragmentShaderPath = "../src/shaders/threelights.fsh";
    std::vector<uint8_t> vertexShaderSource;
    std::vector<uint8_t> geometryShaderSource;
    std::vector<uint8_t> fragmentShaderSource;

    if (!Utilities::File::getFileContents(vertexShaderSource, vertexShaderPath)) {
        std::cout << "Failed to load " + vertexShaderPath << std::endl;
    }

    if (!Utilities::File::getFileContents(fragmentShaderSource, fragmentShaderPath)) {
        std::cout << "Failed to load " + fragmentShaderPath << std::endl;
    }

    PhongMaterial threeLightsMaterial(vertexShaderSource, geometryShaderSource, fragmentShaderSource);

    SDL_Event event;
    bool userRequestedExit = false;

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    float scale = 0.0f;

    glm::mat4 worldMatrix;
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(), glm::vec3(.5, .5, .5));
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), (float)(2 * Utilities::Math::PI * (60.0 / 360)), glm::vec3(-1, 0, 0));
    glm::mat4 translationMatrix;
    glm::mat4 perspectiveProjection = glm::perspective(
        (GLfloat)Utilities::Math::degreesToRadians(60.0),
        ((GLfloat)screenWidth / (GLfloat)screenHeight),
        0.1f,
        100.0f
    );

    Controls playerControls;

    auto playerCamera = std::make_shared<Camera>(glm::lookAt(
            glm::vec3(0, 0, 0), //position
            glm::vec3(0, 0, -1), //looking towards this direction
            glm::vec3(0, 1, 0) //up vector
        )
    );

    glm::mat4 viewMatrix = playerCamera->getViewMatrix();
    
    glm::mat4 WVPMatrix;

    //Get handle to WVP uniform in the shader program
    threeLightsMaterial.addUniformAttribute("WVPMatrix");
    auto WVPMatrixHandle = threeLightsMaterial.getUniformAttribute("WVPMatrix");

    //Get handle to world matrix uniform for use w/ normals for lighting
    threeLightsMaterial.addUniformAttribute("worldMatrix");
    auto worldMatrixHandle = threeLightsMaterial.getUniformAttribute("worldMatrix");

    //Initialize glVertexAttribPointers while VBO is bound
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    threeLightsMaterial.addAttribute(VAO, "position");
    threeLightsMaterial.setGLVertexAttribPointer(VAO, "position", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    threeLightsMaterial.addAttribute(VAO, "normal");
    threeLightsMaterial.setGLVertexAttribPointer(VAO, "normal", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(glm::vec3));
    threeLightsMaterial.addAttribute(VAO, "texCoord");
    threeLightsMaterial.setGLVertexAttribPointer(VAO, "texCoord", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(glm::vec3) * 2));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Set the sampler uniform for the fragment shader that we're going to use.
    //Note: technically unnecessary since we're only using 1 texture unit,
    //but good to be explicitly correct when possible

    //Note: need to bind the material so the contained shader program is active, otherwise
    //the glUniform* don't set values on the appropriate shader program
    threeLightsMaterial.bind();
    threeLightsMaterial.setTextureUnits(1, 0, 0);

    //Enable one directional, point, and 2 spot light and set up their parameters
    threeLightsMaterial.addDirectionalLightUniformAttributes(1);
    threeLightsMaterial.addPointLightUniformAttributes(1);
    threeLightsMaterial.addSpotLightUniformAttributes(2);
    threeLightsMaterial.addCameraPositionUniformAttribute();
    LightList lights(playerCamera);
    auto directionalLightHandle = lights.addDirectionalLight(
        glm::vec3(1.0f, 1.0f, 1.0f), //light color
        glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)), //light direction
        0.1f, //ambient intensity
        0.8f, //diffuse intensity
        1.0f, //specular intensity
        32    //specular power
    );
    auto pointLightHandle = lights.addPointLight(
        glm::vec3(1.0f, 0.5f, 0.0f), //color
        glm::vec3(0.0f, 0.0f, 8.0f), //position
        0.1f, //ambient intensity
        0.5f, //diffuse intensity
        1.0f, //specular intensity
        32,   //specular power
        1.0f, //attenuation constant
        0.1f, //attenuation linear
        0.0f  //attenuation exponential
    );
    auto spotLightHandle = lights.addSpotLight(
        glm::vec3(0.0f, 1.0f, 0.0f), //color
        glm::vec3(-5.0f, -7.0f, 15.0f), //position
        glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)), //direction
        0.1f, //ambient intensity
        0.8f, //diffuse intensity
        1.0f, //specular intensity
        32,   //specular power
        1.0f, //attenuation constant
        0.1f, //attenuation linear
        0.0f,  //attenuation exponential
        40.0f //falloff angle
    );
    auto spotLightHandle2 = lights.addSpotLight(
        glm::vec3(1.0f, 0.0f, 1.0f), //color
        glm::vec3(15.0f, -7.0f, 7.0f), //position
        glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f)), //direction
        0.1f, //ambient intensity
        0.8f, //diffuse intensity
        1.0f, //specular intensity
        32,   //specular power
        1.0f, //attenuation constant
        0.1f, //attenuation linear
        0.0f,  //attenuation exponential
        40.0f //falloff angle
    );
    lights.setLights(
        threeLightsMaterial.getDirectionalLightUniforms(),
        threeLightsMaterial.getPointLightUniforms(),
        threeLightsMaterial.getSpotLightUniforms(),
        threeLightsMaterial.getCameraPositionUniformAttribute()
    );
    threeLightsMaterial.unbind();

    //Load the texture for our basic pyramid object
    std::vector<uint8_t> rawImageData;
    bool loadedImageFile = Utilities::File::getFileContents(rawImageData, "../test.png");

    if (!loadedImageFile) {
        std::cout << "Failed to load the texture image";
        return EXIT_FAILURE;
    }

    Texture pyramidTexture;
    pyramidTexture.load(GL_TEXTURE_2D, rawImageData, TextureFormat::PNG, TextureType::Specular);
    pyramidTexture.setTextureParams(
        GL_REPEAT,
        GL_REPEAT,
        GL_LINEAR,
        GL_LINEAR,
        false
    );
    pyramidTexture.bind(GL_TEXTURE0);

    auto pointLightBase = lights.getLightByID(pointLightHandle);
    auto pointLight = static_cast<PointLight*>(pointLightBase);
    auto spotLightBase = lights.getLightByID(spotLightHandle);
    auto spotLight = static_cast<SpotLight*>(spotLightBase);
    while (userRequestedExit == false)
    {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                userRequestedExit = true;
            }

        }

        playerControls.update();
        playerCamera->update(playerControls);

        //Quick hack for ambient light controls
        if (playerControls.isKeyPressed(ControlKeys::AmbientLightUp)) {
            lights.getLightByID(directionalLightHandle)->ambientIntensity += 0.05f;
        }

        if (playerControls.isKeyPressed(ControlKeys::AmbientLightDown)) {
            lights.getLightByID(directionalLightHandle)->ambientIntensity -= 0.05f;
        }

        scale += 0.002f;

        //Spin & move the dummy pyriamid
        rotationMatrix = glm::rotate(rotationMatrix, (float)(2 * Utilities::Math::PI * (2.0/360)), glm::vec3(0, 1, 0));
        translationMatrix = glm::translate(glm::mat4(), glm::vec3(sinf(scale * 2), 0, -3));

        // worldMatrix = translationMatrix * rotationMatrix * scaleMatrix;

        //Move the point light back and forth
        float lightMoveIncrement = .1;
        if (pointLight->position.x > 15 || pointLight->position.x < -15) {
            pointLight->position.x = -15;
        }
        pointLight->position.x += lightMoveIncrement;

        if (spotLight->position.z < 5) {
            spotLight->position.z = 15;
        }
        spotLight->position.z -= lightMoveIncrement * .5;

        viewMatrix = playerCamera->getViewMatrix();
        WVPMatrix = perspectiveProjection * viewMatrix * worldMatrix;

        threeLightsMaterial.bind();

        lights.setLights(
            threeLightsMaterial.getDirectionalLightUniforms(),
            threeLightsMaterial.getPointLightUniforms(),
            threeLightsMaterial.getSpotLightUniforms(),
            threeLightsMaterial.getCameraPositionUniformAttribute()
        );
        glUniformMatrix4fv(WVPMatrixHandle, 1, GL_FALSE, &WVPMatrix[0][0]);
        glUniformMatrix4fv(worldMatrixHandle, 1, GL_FALSE, &worldMatrix[0][0]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        threeLightsMaterial.unbind();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        SDL_GL_SwapWindow(renderWindow.getRenderWindowHandle());
    }

    //Shutdown SDL2 before exiting the program
    renderWindow.freeResources();

    return EXIT_SUCCESS;
}
