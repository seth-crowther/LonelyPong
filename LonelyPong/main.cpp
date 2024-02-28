#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "paddle.cpp"
#include "bufferManager.cpp"
#include "ball.cpp"

// Vertex shader specifies where the vertices should be drawn relative to screen space
static const char* vertexShaderText =
"#version 330 core\n"
"layout(location = 0) in vec4 position;\n" // Specifies index of position attribute
"void main()\n"
"{\n"
"    gl_Position = position;\n"
"}\n";

// Fragment shader specifies color of each pixel
static const char* fragmentShaderText =
"#version 330 core\n"
"layout(location = 0) out vec4 color;\n"
"uniform vec3 u_color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(u_color.x / 255.0, u_color.y / 255.0, u_color.z / 255.0, 1.0);\n"
"}\n";

#include <fstream>
#include <sstream>
#include <string>

Paddle paddle = Paddle();
Ball ball = Ball();
int newPaddleDir = 0;

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // Raw pointer to start of source string
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling for shader compilation
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length);
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader..." << std::endl;
        std::cout << message << std::endl;

        free(message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderText);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderText);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // Since shaders have already been linked to program, you can delete intermediate files
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_LEFT)
    {
        if (action == GLFW_PRESS)
            newPaddleDir -= 1;
        else if (action == GLFW_RELEASE)
            newPaddleDir += 1;
    }
    if (key == GLFW_KEY_RIGHT)
    {
        if (action == GLFW_PRESS)
            newPaddleDir += 1;
        else if (action == GLFW_RELEASE)
            newPaddleDir -= 1;
    }
}

void update(double deltaTime, GLFWwindow* window)
{
    // Update location of paddle and then update vertex buffer
    paddle.ChangeDir(newPaddleDir);
    paddle.UpdateSprite(deltaTime);
    ball.Update(deltaTime);
    ball.CheckBounce(&(paddle.GetRect()->vertexPositions[0]), &(paddle.GetRect()->vertexPositions[4]));

    if (ball.IsGameOver())
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 2 * 4, paddle.GetRect()->vertexPositions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 4, sizeof(float) * 32, ball.GetCircle()->vertexPositions);
}

void draw(GLFWwindow* window)
{
    int width, height;

    // In the case of the window being resized, obtain new width and height for the frame buffer
    glfwGetFramebufferSize(window, &width, &height);

    // Clear screen
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    // Rendering
    glDrawElements(GL_TRIANGLES, 96, GL_UNSIGNED_INT, nullptr);
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(720, 720, "Lonely Pong", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, keyCallback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    VertexBuffer vertices = VertexBuffer(paddle.GetRect()->vertexPositions, ball.GetCircle()->vertexPositions);
    IndexBuffer indices = IndexBuffer(paddle.GetRect()->vertexIndices, ball.GetCircle()->vertexIndices);

    // Setup vertex buffer
    unsigned int vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBuffer), &vertices, GL_STATIC_DRAW);

    // Position vertex attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

    // Setup index buffer
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexBuffer), &indices, GL_STATIC_DRAW);

    unsigned int shader = CreateShader(vertexShaderText, fragmentShaderText);
    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "u_color");
    glUniform3f(location, 194.0f, 249.0f, 187.0f);

    double lastTime = 0.0;
    
    while (!glfwWindowShouldClose(window))
    {
        double time = glfwGetTime();
        double deltaTime = time - lastTime;
        lastTime = time;
        
        update(deltaTime, window);
        
        draw(window);

        glfwSwapBuffers(window); // Swap front/back buffers
        glfwPollEvents(); // Processes pending events
    }

    // Exit safely
    glDeleteProgram(shader);
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}