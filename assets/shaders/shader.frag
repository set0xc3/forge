#version 330 core

layout (location = 0) out vec4 out_color;

in vec3 our_color;
in vec2 our_uv;

void 
main(void)
{
    out_color = vec4(our_color.x, our_color.y, our_color.z, 1.0f);
}
