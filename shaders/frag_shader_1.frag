#version 410 core

out vec4 color;

in vec2 fs_param;

void main(void)
{
    color = vec4(fs_param.x, fs_param.y, 1.0, 1.0);
}
