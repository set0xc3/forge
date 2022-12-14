#ifndef FORGE_RENDERER_TYPES_H
#define FORGE_RENDERER_TYPES_H

typedef struct Vertex2d
{
	Vector2f position;
	Vector3f color;
	Vector2f uv;
}Vertex2d;

typedef struct Shader
{
	u64 id;
}Shader;

#endif //FORGE_RENDERER_TYPES_H