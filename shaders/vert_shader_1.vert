#version 410 core

out vec2 fs_param;

void main(void)
{
    const vec4 vertices[] = vec4[](
        vec4( 0.25, -0.25, 0.5, 1.0),
        vec4(-0.25, -0.25, 0.5, 1.0),
        vec4( 0.25,  0.25, 0.5, 1.0),

        vec4(-0.25, -0.25, 0.5, 1.0),
        vec4( 0.25,  0.25, 0.5, 1.0),
        vec4(-0.25,  0.25, 0.5, 1.0));

    const vec2 fs_params[] = vec2[](
        vec2(1, 1),
        vec2(0, 1),
        vec2(1, 0),

        vec2(0, 1),
        vec2(1, 0),
        vec2(0, 0));

    gl_Position = vertices[gl_VertexID];
    fs_param = fs_params[gl_VertexID];
}
