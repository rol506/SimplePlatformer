#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"

using std::cout;

int g_windowSizeX = 640;
int g_windowSizeY = 480;

GLfloat point[] = {
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }

}

int main(int argc, char** argv)
{
    /* Initialize the library */
    if (!glfwInit()) {
        cout << "Can't initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Simple Platformer", NULL, NULL);
    if (!pWindow)
    {
        cout << "Can't create window\n";
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL()) {
        cout << "Can't load GLAD\n";
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

    glClearColor(0, 1, 0, 0);

    {
        ResourceManager resourceManager(argv[0]);

        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShaderProgram", "resources/shaders/vertex.txt", "resources/shaders/fragment.txt");
        if (!pDefaultShaderProgram) {
            std::cerr << "Can't create shader program: " << "DefaultShaderProgram\n";
            return -1;
        }

        GLuint points_vbo = 0;                            //generate id for points vbo (Vertex Buffer Object)
        glGenBuffers(1, &points_vbo);                     //generate vbo
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);        //make buffer current
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW); //insert data in buffer (works only with current buffer)

        GLuint colors_vbo = 0;                            //generate id for colors vbo (Vertex Buffer Object)
        glGenBuffers(1, &colors_vbo);                     //generate vbo
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);        //make buffer current
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW); //insert data in buffer (works only with current buffer)

        GLuint vao = 0;                                   //generate id for vao(Vertex Array Object)
        glGenVertexArrays(1, &vao);                       //generate vao
        glBindVertexArray(vao);                           //make vao current

        glEnableVertexAttribArray(0);                     //enable location 0(see vertex shader)
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);        //make buffer current
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); //link data

        glEnableVertexAttribArray(1);                     //enable location 1(see vertex shader)
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);        //make buffer current
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr); //link data

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}