#version 410 core

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec2 fs_tex_uv;

void main(void)
{
    const vec2 vertices[] = vec2[](
        vec2( 1, -1),
        vec2(-1, -1),
        vec2( 1,  1),

        vec2(-1, -1),
        vec2( 1,  1),
        vec2(-1,  1));

    const vec2 fs_tex_uvs[] = vec2[](
        vec2(1, 1),
        vec2(0, 1),
        vec2(1, 0),

        vec2(0, 1),
        vec2(1, 0),
        vec2(0, 0));

    fs_tex_uv = fs_tex_uvs[gl_VertexID];

    gl_Position = vec4(vertices[gl_VertexID], 0, 1);
    gl_Position = proj_matrix * mv_matrix * gl_Position;
}
