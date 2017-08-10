#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <unistd.h>
#include "gl_utils.h"

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

static GLuint s_vao;
static GLuint s_prog1;
static GLint s_angle_rads_loc;

static void error_callback(int error, const char* description) {
    std::cout << description << std::endl;
}

void draw() {
    static uint32_t frame_num = 0;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(s_prog1);

    float angle = (2 * PI_F) * ((frame_num % 120) / 120.f);
    glUniform1f(s_angle_rads_loc, angle);

    glBindVertexArray(s_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    frame_num++;
}

int main(int argc, const char* argv[]) {
    char buf[256];
    getcwd(buf, sizeof(buf));
    std::cout << "cwd: " << buf << std::endl;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    glfwSetErrorCallback(error_callback);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Play 01", nullptr, nullptr);

    if (!window) {
        glGetError();
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return -1;
    }
    
    int screen_width, screen_height;
    glfwGetFramebufferSize(window, &screen_width, &screen_height);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    s_prog1 = make_prog(
        "../../../shaders/vert_shader_1.vert",
        "../../../shaders/frag_shader_1.frag");
    if (s_prog1 == 0) {
        std::cout << "Failed to create the shader program" << std::endl;
        return -1;
    }

    s_angle_rads_loc = glGetUniformLocation(s_prog1, "angle_rads");
    if (s_angle_rads_loc == -1) {
        std::cout << "couldn't get the uniform for angle_rads" << std::endl;
        return -1;
    }

    glGenVertexArrays(1, &s_vao);

    glViewport(0, 0, screen_width, screen_height);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        draw();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
