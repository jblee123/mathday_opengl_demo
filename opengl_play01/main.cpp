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

enum BUFFER_TYPE_t
{
    POSITION_A,
    POSITION_B,
    VELOCITY_A,
    VELOCITY_B,
    CONNECTION
};

enum
{
    POINTS_X            = 50,
    POINTS_Y            = 50,
    POINTS_TOTAL        = (POINTS_X * POINTS_Y),
    CONNECTIONS_TOTAL   = (POINTS_X - 1) * POINTS_Y + (POINTS_Y - 1) * POINTS_X
};

GLuint          m_vao[2];
GLuint          m_vbo[5];
GLuint          m_index_buffer;
GLuint          m_pos_tbo[2];
GLuint          m_update_program;
GLuint          m_render_program;
GLuint          m_iteration_index;

int             iterations_per_frame = 16;




static void error_callback(int error, const char* description) {
    std::cout << description << std::endl;
}

void load_shaders() {

    GLuint vs = load_shader("../../../shaders/springmass/update.vs.glsl", GL_VERTEX_SHADER);

    if (m_update_program)
        glDeleteProgram(m_update_program);
    m_update_program = glCreateProgram();
    glAttachShader(m_update_program, vs);

    static const char* tf_varyings[] = {
        "tf_position_mass",
        "tf_velocity"
    };

    glTransformFeedbackVaryings(m_update_program, 2, tf_varyings, GL_SEPARATE_ATTRIBS);

    glLinkProgram(m_update_program);

    m_render_program = make_prog(
        "../../../shaders/springmass/render.vs.glsl",
        "../../../shaders/springmass/render.fs.glsl");
    if (m_render_program == 0) {
        std::cout << "Failed to create the shader program" << std::endl;
        exit(-1);
    }
}

void startup() {
    int i, j;

    load_shaders();

    Vec4f* initial_positions = new Vec4f[POINTS_TOTAL];
    Vec3f* initial_velocities = new Vec3f[POINTS_TOTAL];
    Vec4i* connection_vectors = new Vec4i[POINTS_TOTAL];

    int n = 0;

    for (j = 0; j < POINTS_Y; j++) {
        float fj = (float)j / (float)POINTS_Y;
        for (i = 0; i < POINTS_X; i++) {
            float fi = (float)i / (float)POINTS_X;

            initial_positions[n] = Vec4f((fi - 0.5f) * (float)POINTS_X,
                                         (fj - 0.5f) * (float)POINTS_Y,
                                         0.6f * sinf(fi) * cosf(fj),
                                         1.0f);
            initial_velocities[n] = Vec3f(0, 0, 0);

            connection_vectors[n] = Vec4i(-1, -1, -1, -1);

            if (j != (POINTS_Y - 1))
            {
                if (i != 0)
                    connection_vectors[n][0] = n - 1;

                if (j != 0)
                    connection_vectors[n][1] = n - POINTS_X;

                if (i != (POINTS_X - 1))
                    connection_vectors[n][2] = n + 1;

                if (j != (POINTS_Y - 1))
                    connection_vectors[n][3] = n + POINTS_X;
            }
            n++;
        }
    }

    glGenVertexArrays(2, m_vao);
    glGenBuffers(5, m_vbo);

    for (i = 0; i < 2; i++) {
        glBindVertexArray(m_vao[i]);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo[POSITION_A + i]);
        glBufferData(GL_ARRAY_BUFFER, POINTS_TOTAL * sizeof(Vec4f), initial_positions, GL_DYNAMIC_COPY);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VELOCITY_A + i]);
        glBufferData(GL_ARRAY_BUFFER, POINTS_TOTAL * sizeof(Vec3f), initial_velocities, GL_DYNAMIC_COPY);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo[CONNECTION]);
        glBufferData(GL_ARRAY_BUFFER, POINTS_TOTAL * sizeof(Vec4i), connection_vectors, GL_STATIC_DRAW);
        glVertexAttribIPointer(2, 4, GL_INT, 0, NULL);
        glEnableVertexAttribArray(2);
    }

    delete [] connection_vectors;
    delete [] initial_velocities;
    delete [] initial_positions;

    glGenTextures(2, m_pos_tbo);
    glBindTexture(GL_TEXTURE_BUFFER, m_pos_tbo[0]);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vbo[POSITION_A]);
    glBindTexture(GL_TEXTURE_BUFFER, m_pos_tbo[1]);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vbo[POSITION_B]);

    int lines = (POINTS_X - 1) * POINTS_Y + (POINTS_Y - 1) * POINTS_X;

    glGenBuffers(1, &m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, lines * 2 * sizeof(int), NULL, GL_STATIC_DRAW);

    int * e = (int *)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, lines * 2 * sizeof(int), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    for (j = 0; j < POINTS_Y; j++)
    {
        for (i = 0; i < POINTS_X - 1; i++)
        {
            *e++ = i + j * POINTS_X;
            *e++ = 1 + i + j * POINTS_X;
        }
    }

    for (i = 0; i < POINTS_X; i++)
    {
        for (j = 0; j < POINTS_Y - 1; j++)
        {
            *e++ = i + j * POINTS_X;
            *e++ = POINTS_X + i + j * POINTS_X;
        }
    }

    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

// void render(double t)
void render(GLFWwindow* window)
{
    int i;
    glUseProgram(m_update_program);

    glEnable(GL_RASTERIZER_DISCARD);

    for (i = iterations_per_frame; i != 0; --i)
    {
        glBindVertexArray(m_vao[m_iteration_index & 1]);
        glBindTexture(GL_TEXTURE_BUFFER, m_pos_tbo[m_iteration_index & 1]);
        m_iteration_index++;
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[POSITION_A + (m_iteration_index & 1)]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_vbo[VELOCITY_A + (m_iteration_index & 1)]);
        glBeginTransformFeedback(GL_POINTS);
        glDrawArrays(GL_POINTS, 0, POINTS_TOTAL);
        glEndTransformFeedback();
    }

    glDisable(GL_RASTERIZER_DISCARD);

    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    //glViewport(0, 0, info.windowWidth, info.windowHeight);
    glClearBufferfv(GL_COLOR, 0, black);

    glUseProgram(m_render_program);

    // if (draw_points)
    // {
        glPointSize(4.0f);
        glDrawArrays(GL_POINTS, 0, POINTS_TOTAL);
    // }

    // if (draw_lines)
    // {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
        glDrawElements(GL_LINES, CONNECTIONS_TOTAL * 2, GL_UNSIGNED_INT, NULL);
    // }
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

    startup();


    glViewport(0, 0, screen_width, screen_height);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        render(window);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
