#version 450

vec2 position[3] = vec2[]
(
  vec2(0.0f, -0.5f),
  vec2(0.5f, 0.5f),
  vec2(-0.5, 0.5f)
);

void 
main(void)
{
  gl_Position = vec4(position[gl_VertexIndex], 0.0f, 1.0f);
}