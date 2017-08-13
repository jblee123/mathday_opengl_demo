#version 410 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex_uv;
layout (location = 2) in vec3 in_color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec2 fs_tex_uv;
out vec4 fs_color;

void main(void)
{
    fs_tex_uv = in_tex_uv;
    fs_color = vec4(in_color, 1);

    gl_Position = vec4(in_pos, 1);
    gl_Position = proj_matrix * mv_matrix * gl_Position;
}
