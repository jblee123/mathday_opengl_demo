#version 410 core

uniform sampler2D smiley_sampler;

out vec4 color;

in vec2 fs_tex_uv;

void main(void)
{
    color = texture(smiley_sampler, fs_tex_uv);

    if (color.a == 0) {
        color = vec4(1, 0, 0, 0.25);
    }
}
