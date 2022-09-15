
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

    glEnable(GL_DEPTH_TEST);

    static GLenum dcomp_map[] = {
        /*DepthCompare_Never*/ GL_NEVER,
        /*DepthCompare_Less*/ GL_LESS,
        /*DepthCompare_Equal*/ GL_EQUAL,
        /*DepthCompare_LessEqual*/ GL_LEQUAL,
        /*DepthCompare_Greater*/ GL_GREATER,
        /*DepthCompare_NotEqual*/ GL_NOTEQUAL,
        /*DepthCompare_GreaterEqual*/ GL_GEQUAL,
        /*DepthCompare_Always*/ GL_ALWAYS,
    };

    glDepthFunc(dcomp_map[Rendering::DepthCompare::DepthCompare_LessEqual]);

    GL::projection = glm::perspective(35.0f, 1.0f, 1.0f, 1000.0f);

    GL::genShader = new Shader();
    GL::genShader->GL_CompileShader(NULL, NULL);
    GL::genShader->SetProject(GL::genShader, GL::projection);

    Rendering::Render_GLInit(GL::genShader);

    Texture* t = Texture::createWithImage("texture.png");

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(.25f, .25f, .25f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        Rect r;
        r.x = 620;
        r.y = 340;
        r.z = -12;
        r.w = 200;
        r.h = 200;

        Rect src;
        src.w = 1;
        src.h = 1;

        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        glm::mat4 view;
        view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

        glUniformMatrix4fv(glGetUniformLocation(GL::genShader->program, "u_view"), 1, GL_FALSE, &view[0][0]);

        Rendering::PushQuad(&r, &src, t, GL::genShader);

        Rendering::drawBatch();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
