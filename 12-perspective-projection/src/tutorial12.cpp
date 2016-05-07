#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const double PI = 3.1415926535897;
const unsigned int screenWidth = 800;
const unsigned int screenHeight = 600;

struct ShaderProgramStages
{
    GLuint vertexShader = 0;
    GLuint geometryShader = 0;
    GLuint fragmentShader = 0;
};

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

GLuint compileShaderProgramFromSource(const std::vector<char> &shaderSource, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    const GLchar *glFormatShaderSource = shaderSource.data();
    const GLint shaderSourceLength = shaderSource.size();
    glShaderSource(shader, 1, &glFormatShaderSource, &shaderSourceLength);
    glCompileShader(shader);

    GLint compilationSuccess = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationSuccess);

    if (!compilationSuccess) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);

        for (int i = 0; i < 1024; ++i) {
            std::cout << infoLog[i];
        }
        std::cout << std::endl;

        return 0;
    }

    return shader;
}

GLuint linkShaderProgram(const ShaderProgramStages& shaderStages)
{
    GLuint program = glCreateProgram();
    bool vertexShaderAttached = false;
    bool geometryShaderAttached = false;
    bool fragmentShaderAttached = false;

    if (shaderStages.vertexShader != 0) {
        glAttachShader(program, shaderStages.vertexShader);
        vertexShaderAttached = true;
    }

    if (shaderStages.geometryShader != 0) {
        glAttachShader(program, shaderStages.geometryShader);
        geometryShaderAttached = true;
    }

    if (shaderStages.fragmentShader != 0) {
        glAttachShader(program, shaderStages.fragmentShader);
        fragmentShaderAttached = true;
    }

    glLinkProgram(program);

    if (vertexShaderAttached) {
        glDetachShader(program, shaderStages.vertexShader);
    }

    if (geometryShaderAttached) {
        glDetachShader(program, shaderStages.geometryShader);
    }

    if (fragmentShaderAttached) {
        glDetachShader(program, shaderStages.fragmentShader);
    }

    GLint linkSuccess = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);

        for (int i = 0; i < 1024; ++i) {
            std::cout << infoLog[i];
        }
        std::cout << std::endl;

        return 0;
    }

    return program;
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
        "ogldev.atspace.co.uk Tutorial 12 -- Perspective Projection",
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
    std::vector<char> fragmentShaderSource = loadShaderSourceFromFile("../src/shaders/passthrough.fsh");

    ShaderProgramStages shaderStages;
    shaderStages.vertexShader = compileShaderProgramFromSource(vertexShaderSource, GL_VERTEX_SHADER);
    shaderStages.fragmentShader = compileShaderProgramFromSource(fragmentShaderSource, GL_FRAGMENT_SHADER);

    GLuint passthroughProgram = linkShaderProgram(shaderStages);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glUseProgram(passthroughProgram);


    //Sit around for a while until user input
    SDL_Event event;
    bool userRequestedExit = false;

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    float scale = 0.0f;

    glm::mat4 worldPosition;
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(), glm::vec3(.5, .5, .5));
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), (float)(2 * PI * (60.0 / 360)), glm::vec3(-1, 0, 0));
    glm::mat4 translationMatrix;
    glm::mat4 perspectiveProjection = glm::perspective(
        (GLfloat)degreesToRadians(60.0),
        ((GLfloat)screenWidth / (GLfloat)screenHeight),
        0.1f,
        100.0f
    );

    //Get handle to worldPosition uniform in the shader program
    GLint worldPositionHandle = glGetUniformLocation(passthroughProgram, "worldPosition");

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

        worldPosition = perspectiveProjection * translationMatrix * rotationMatrix * scaleMatrix;

        glUniformMatrix4fv(worldPositionHandle, 1, GL_FALSE, &worldPosition[0][0]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        SDL_GL_SwapWindow(window);
    }

    //Shutdown SDL2 before exiting the program
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
