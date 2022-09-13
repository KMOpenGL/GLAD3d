
#pragma comment(lib, "lib/glfw3.lib")

#define GLFW_INCLUDE_NONE

#include "Includes.h"

void error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error: %s\n", description);
}

int main()
{
    if (!glfwInit())
    {
        std::cout << "GLFW Init Failed!" << std::endl;
        return 0;
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "GLAD3D", NULL, NULL);
    if (!window)
    {
        std::cout << "GLFW Window Failed!" << std::endl;
        return 0;
    }

    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSwapInterval(1);

    GL::projection = glm::ortho(0.0f, (float)1280, (float)720, 0.0f, -1.0f, 1.0f);

    GL::genShader = new Shader();
    GL::genShader->GL_CompileShader(NULL, NULL);
    GL::genShader->SetProject(GL::genShader, GL::projection);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.f, 1.f, 1.f, 1.f);

        Rect r;
        r.x = 620;
        r.y = 340;
        r.z = 0;
        r.w = 20;
        r.h = 20;

        Rect src;
        src.w = 1;
        src.h = 1;

        Rendering::PushQuad(&r, &src, NULL, GL::genShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
