#version 410 core

uniform float angle_rads;

out vec2 fs_tex_uv;

void main(void)
{
    const vec2 vertices[] = vec2[](
        vec2( 0.25, -0.25),
        vec2(-0.25, -0.25),
        vec2( 0.5,  0.25),

        vec2(-0.25, -0.25),
        vec2( 0.25,  0.5),
        vec2(-0.25,  0.25));

    const vec2 fs_tex_uvs[] = vec2[](
        vec2(1, 1),
        vec2(0, 1),
        vec2(1, 0),

        vec2(0, 1),
        vec2(1, 0),
        vec2(0, 0));

    vec2 pos = vertices[gl_VertexID];
    fs_tex_uv = fs_tex_uvs[gl_VertexID];

    float sin_theta = sin(angle_rads);
    float cos_theta = cos(angle_rads);

    gl_Position.x = pos.x * cos_theta - pos.y * sin_theta;
    gl_Position.y = pos.x * sin_theta + pos.y * cos_theta;
    gl_Position.z = 0.5;
    gl_Position.w = 1.0;
}
