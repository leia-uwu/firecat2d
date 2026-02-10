#version 310 es

precision highp float;

layout (location = 0) out vec4 v_color;
layout (location = 1) out vec2 v_textureCord;

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_textureCord;
layout (location = 2) in uint a_color;

layout (set = 1, binding = 0, row_major) uniform Uniforms {
    mat3 u_transform;
};

void main()
{
    float r = float((a_color >> 24) & 255u) / 255.0;
    float g = float((a_color >> 16) & 255u) / 255.0;
    float b = float((a_color >> 8 ) & 255u) / 255.0;
    float a = float((a_color      ) & 255u) / 255.0;
    v_color = vec4(r, g, b, a);

    v_textureCord = a_textureCord;
    gl_Position = vec4(vec3(a_pos, 1.0) * u_transform, 1.0);
}
