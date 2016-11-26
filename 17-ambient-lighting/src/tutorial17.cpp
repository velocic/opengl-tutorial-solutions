#include <GL/gl3w.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <camera.h>
#include <controls.h>
#include <lights/directional.h>
#include <materials/phongmaterial.h>
#include <renderwindow.h>
#include <texture.h>
#include <utilities.h>

const unsigned int screenWidth = 800;
const unsigned int screenHeight = 600;

struct Vertex
{
    glm::vec3 position;
    glm::vec2 uvCoords;
};

int main()
{
    //Initialize SDL2 with video and event modes enabled,
    //and request an OpenGL 3.3 Core Profile
    RenderWindow renderWindow(screenWidth, screenHeight);

    Vertex vertices[] = {
        {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.0f, -1.0f, 1.0f), glm::vec2(0.5f, 0.0f)},
        {glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 1.0f)}
    };

    //Allocate a buffer for our vertices
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

    std::string vertexShaderPath = "../src/shaders/passthrough.vsh";
    std::string fragmentShaderPath = "../src/shaders/passthrough.fsh";
    std::vector<uint8_t> vertexShaderSource;
    std::vector<uint8_t> geometryShaderSource;
    std::vector<uint8_t> fragmentShaderSource;

    if (!Utilities::File::getFileContents(vertexShaderSource, vertexShaderPath)) {
        std::cout << "Failed to load " + vertexShaderPath << std::endl;
    }

    if (!Utilities::File::getFileContents(fragmentShaderSource, fragmentShaderPath)) {
        std::cout << "Failed to load " + fragmentShaderPath << std::endl;
    }

    PhongMaterial basicPassthroughMaterial(vertexShaderSource, geometryShaderSource, fragmentShaderSource);

    //Sit around for a while until user input
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
    Camera playerCamera(
        glm::lookAt(
            glm::vec3(0, 0, 0), //position
            glm::vec3(0, 0, -1), //looking towards this direction
            glm::vec3(0, 1, 0) //up vector
        )
    );
    glm::mat4 viewMatrix = playerCamera.getViewMatrix();
    
    glm::mat4 WVPMatrix;

    //Get handle to WVP uniform in the shader program
    basicPassthroughMaterial.addUniformAttribute("WVPMatrix");
    auto WVPMatrixHandle = basicPassthroughMaterial.getUniformAttribute("WVPMatrix");

    //Initialize glVertexAttribPointers while VBO is bound
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    basicPassthroughMaterial.addAttribute("position");
    basicPassthroughMaterial.setGLVertexAttribPointer("position", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    basicPassthroughMaterial.addAttribute("texCoord");
    basicPassthroughMaterial.setGLVertexAttribPointer("texCoord", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(glm::vec3));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Set the sampler uniform for the fragment shader that we're going to use.
    //Note: technically unnecessary since we're only using 1 texture unit,
    //but good to be explicitly correct when possible

    //Note: need to bind the material so the contained shader program is active, otherwise
    //the glUniform* don't set values on the appropriate shader program
    basicPassthroughMaterial.bind();
    basicPassthroughMaterial.addUniformAttribute("sampler");
    glUniform1i(basicPassthroughMaterial.getUniformAttribute("sampler"), 0);

    //Add a directional light into the scene
    basicPassthroughMaterial.addLightUniformAttribute("directionalLight");
    DirectionalLightList directionalLights;
    directionalLights.addDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f);
    directionalLights.setLights(basicPassthroughMaterial.getLightUniforms());
    basicPassthroughMaterial.unbind();

    //Load the texture for our basic pyramid object
    std::vector<uint8_t> rawImageData;
    bool loadedImageFile = Utilities::File::getFileContents(rawImageData, "../test.png");

    if (!loadedImageFile) {
        std::cout << "Failed to load the texture image";
        return EXIT_FAILURE;
    }

    Texture pyramidTexture;
    pyramidTexture.load(GL_TEXTURE_2D, rawImageData, TextureFormat::PNG);
    pyramidTexture.setTextureParams(
        GL_REPEAT,
        GL_REPEAT,
        GL_LINEAR,
        GL_LINEAR,
        false
    );
    pyramidTexture.bind(GL_TEXTURE0);

    while (userRequestedExit == false)
    {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                userRequestedExit = true;
            }

        }

        scale += 0.002f;

        playerControls.update();
        playerCamera.update(playerControls);
        rotationMatrix = glm::rotate(rotationMatrix, (float)(2 * Utilities::Math::PI * (2.0/360)), glm::vec3(0, 1, 0));
        translationMatrix = glm::translate(glm::mat4(), glm::vec3(sinf(scale * 2), 0, -3));

        worldMatrix = translationMatrix * rotationMatrix * scaleMatrix;

        viewMatrix = playerCamera.getViewMatrix();
        WVPMatrix = perspectiveProjection * viewMatrix * worldMatrix;

        basicPassthroughMaterial.bind();

        glUniformMatrix4fv(WVPMatrixHandle, 1, GL_FALSE, &WVPMatrix[0][0]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        basicPassthroughMaterial.unbind();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        SDL_GL_SwapWindow(renderWindow.getRenderWindowHandle());
    }

    //Shutdown SDL2 before exiting the program
    renderWindow.freeResources();

    return EXIT_SUCCESS;
}
