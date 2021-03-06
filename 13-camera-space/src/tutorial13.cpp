#include <GL/gl3w.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <material.h>

const double PI = 3.1415926535897;
const unsigned int screenWidth = 800;
const unsigned int screenHeight = 600;

double degreesToRadians(double angle)
{
    return (angle * PI) / 180;
}

std::vector<char> loadShaderSourceFromFile(std::string filePath)
{
    std::vector<char> fileContents;
    std::ifstream inFileStream(filePath, std::ios::binary);

    if (!inFileStream) {
        std::cout << "Failed to load " << filePath << std::endl;
        return fileContents;
    }

    inFileStream.seekg(0, std::ios::end);
    std::streampos fileLength = inFileStream.tellg();
    inFileStream.seekg(0, std::ios::beg);

    fileContents.resize(fileLength);
    inFileStream.read(fileContents.data(), fileLength);

    return fileContents;
}

int main()
{
    //Initialize SDL2 with video and event modes enabled,
    //and request an OpenGL 3.3 Core Profile
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_Window* window = SDL_CreateWindow(
        "ogldev.atspace.co.uk Tutorial 13 -- Camera Space",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        SDL_WINDOW_OPENGL
    );

    //Create the OpenGL context before trying to initialize OpenGL API functions
    SDL_GLContext context = SDL_GL_CreateContext(window);

    //Initialize OpenGL API functions, exit if we couldn't load the core functions
    if (gl3wInit()) {
        std::cout << "gl3w failed to initialize OpenGL" << std::endl;
        return EXIT_FAILURE;
    }

    glm::vec3 vertices[] = {
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 1.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    //Allocate a buffer for our vertices
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::vector<GLuint> indices = {
        0, 1, 2,
        0, 3, 2,
        0, 3, 1,
        1, 3, 2
    };

    //Create an index buffer to share vertices among triangles
    GLuint IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    std::vector<char> vertexShaderSource = loadShaderSourceFromFile("../src/shaders/passthrough.vsh");
    std::vector<char> geometryShaderSource;
    std::vector<char> fragmentShaderSource = loadShaderSourceFromFile("../src/shaders/passthrough.fsh");

    Material basicPassthroughMaterial(vertexShaderSource, geometryShaderSource, fragmentShaderSource);

    //Sit around for a while until user input
    SDL_Event event;
    bool userRequestedExit = false;

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    float scale = 0.0f;

    glm::mat4 worldMatrix;
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(), glm::vec3(.5, .5, .5));
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), (float)(2 * PI * (60.0 / 360)), glm::vec3(-1, 0, 0));
    glm::mat4 translationMatrix;
    glm::mat4 perspectiveProjection = glm::perspective(
        (GLfloat)degreesToRadians(60.0),
        ((GLfloat)screenWidth / (GLfloat)screenHeight),
        0.1f,
        100.0f
    );
    glm::mat4 viewMatrix = glm::lookAt(
        glm::vec3(0, 0, 0), //position
        glm::vec3(0, 0, -1), //looking towards this direction
        glm::vec3(0, 1, 0) //up vector
    );
    glm::mat4 viewRotationMatrix;
    glm::mat4 viewTranslationMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, 17));
    glm::mat4 WVPMatrix;

    //Get handle to WVP uniform in the shader program
    basicPassthroughMaterial.addUniformAttribute("WVPMatrix");
    auto WVPMatrixHandle = basicPassthroughMaterial.getUniformAttribute("WVPMatrix");

    //Initialize glVertexAttribPointers while VBO is bound
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    basicPassthroughMaterial.addAttribute("position");
    basicPassthroughMaterial.setGLVertexAttribPointer("position", 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    auto debug = basicPassthroughMaterial.isMaterialValid();

    while (userRequestedExit == false)
    {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                userRequestedExit = true;
            }

        }

        scale += 0.002f;

        rotationMatrix = glm::rotate(rotationMatrix, (float)(2 * PI * (2.0/360)), glm::vec3(0, 1, 0));
        translationMatrix = glm::translate(glm::mat4(), glm::vec3(sinf(scale * 2), 0, -3));

        worldMatrix = translationMatrix * rotationMatrix * scaleMatrix;

        //Play around with rotating the camera
        viewRotationMatrix = glm::rotate(viewRotationMatrix, (float)(2 * PI * (.5/360)), glm::vec3(-1, 0, 0));
        glm::vec4 viewDirectionVec4 = (viewRotationMatrix * glm::vec4(0, 0, -1, 0));
        glm::vec3 viewDirection;
        viewDirection.x = viewDirectionVec4.x;
        viewDirection.y = viewDirectionVec4.y;
        viewDirection.z = viewDirectionVec4.z;

        viewMatrix = glm::lookAt(
            glm::vec3(0, 0, 0),
            viewDirection,
            glm::vec3(0, 1, 0)
        );

        //Translate the view after the rotation has happened, so we correctly rotate around the world origin
        viewMatrix *= viewTranslationMatrix;

        WVPMatrix = perspectiveProjection * viewMatrix * worldMatrix;

        basicPassthroughMaterial.bind();

        glUniformMatrix4fv(WVPMatrixHandle, 1, GL_FALSE, &WVPMatrix[0][0]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        basicPassthroughMaterial.unbind();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        SDL_GL_SwapWindow(window);
    }

    //Shutdown SDL2 before exiting the program
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
