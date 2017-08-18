#version 410 core

uniform sampler2D smiley_sampler;

out vec4 color;

in vec2 fs_tex_uv;
in vec4 fs_color;
in vec4 fs_normal;

void main()
{
    const vec4 light_vec = vec4(0, 0, 1, 0);

    color = texture(smiley_sampler, fs_tex_uv);
    float brightness_factor = dot(fs_normal, light_vec);
    brightness_factor *= 0.5;
    brightness_factor += 0.5;

    if (color.a == 0) {
        color = fs_color;
    }

    color = vec4(color.rgb * brightness_factor, color.w);
}
