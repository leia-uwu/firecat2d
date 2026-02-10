#version 310 es

precision highp float;

layout (location = 0) in vec4 v_color;
layout (location = 1) in vec2 v_textureCord;

layout (location = 0, index = 0) out vec4 o_color;

layout (set = 2, binding = 0) uniform sampler2D u_texture;

void main()
{
    o_color = texture(u_texture, v_textureCord) * v_color;
}
