#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_uv;

out vec3 our_color;
out vec2 our_uv;

void 
main(void)
{
    gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0f);
    our_color   = a_color;
    our_uv      = a_uv;
}
