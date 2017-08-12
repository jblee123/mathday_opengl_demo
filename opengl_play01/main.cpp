#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <unistd.h>

#include "gl_utils.h"
#include "stb_image.h"

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

static GLuint s_vao;
static GLuint s_prog1;

static GLint s_mv_matrix_loc;
static GLint s_proj_matrix_loc;

static GLuint s_smiley_tex;

static Vec3f s_cam_pos;

static void error_callback(int error, const char* description) {
    std::cout << description << std::endl;
}

void create_smiley_texture() {
    int width, height, channels;
    unsigned char* data = stbi_load(
        "../../../images/smiley_face.png", &width, &height, &channels, 0);
    if (!data) {
        std::cout << "couldn't load smiley.png" << std::endl;
        exit(-1);
    }

    glGenTextures(1, &s_smiley_tex);
    check_gl_err_or_die("gen tex s_smiley_tex");

    glBindTexture(GL_TEXTURE_2D, s_smiley_tex);
    check_gl_err_or_die("binding s_smiley_tex");

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, data);
    check_gl_err_or_die("glTexImage2D smiley tex");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

void draw(GLFWwindow* window) {
    static uint32_t frame_num = 0;
    static const float FOV = 90;
    static const float NEAR = 0.1f;
    static const float FAR = 1000.0f;
    static const Vec3f AT = {0, 0, 0};
    static const Vec3f UP = {0, 1, 0};

    int screen_w, screen_h;
    glfwGetFramebufferSize(window, &screen_w, &screen_h);

    float aspect_ratio = (float)screen_w / screen_h;

    float bottom, top, left, right;
    get_perspective_info(
        FOV, aspect_ratio, NEAR, FAR,
        bottom, top, left, right);

    Matrix44f mv_matrix;
    calc_lookat_matrix(s_cam_pos, AT, UP, mv_matrix);

    Matrix44f proj_matrix;
    calc_proj_matrix(
        bottom, top, left, right, NEAR, FAR,
        proj_matrix);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(s_prog1);

    glUniformMatrix4fv(s_mv_matrix_loc, 1, false, (float*)mv_matrix);
    glUniformMatrix4fv(s_proj_matrix_loc, 1, false, (float*)proj_matrix);

    glBindTexture(GL_TEXTURE_2D, s_smiley_tex);

    glBindVertexArray(s_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);

    frame_num++;
}

int main(int argc, const char* argv[]) {
    char buf[256];
    getcwd(buf, sizeof(buf));
    std::cout << "cwd: " << buf << std::endl;

    s_cam_pos = { 1, 1, 2 };

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    glfwSetErrorCallback(error_callback);

    GLFWwindow* window = glfwCreateWindow(
        WIDTH, HEIGHT, "OpenGL Play 01", nullptr, nullptr);

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

    s_mv_matrix_loc = get_uniform_loc(s_prog1, "mv_matrix");
    s_proj_matrix_loc = get_uniform_loc(s_prog1, "proj_matrix");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    create_smiley_texture();

    glGenVertexArrays(1, &s_vao);

    glViewport(0, 0, screen_width, screen_height);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        draw(window);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
