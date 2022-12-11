#ifndef FORGE_RENDERER_H
#define FORGE_RENDERER_H

typedef struct Vertex2d
{
	Vector2f position;
	Vector3f color;
}Vertex2d;

typedef struct Vertex3d
{
	Vector3f position;
	Vector3f color;
	//Vector3f normal;
	//Vector2f uv;
}Vertex3d;

typedef void RendererInitFunction(void *window_handle);
typedef void RendererBeginFunction(void *window_handle);
typedef void RendererSubmitFunction(void *window_handle);
typedef void RendererEndFunction(void *window_handle);

typedef struct RendererBackend
{
	void                   *library;
	RendererInitFunction   *init;
	RendererBeginFunction  *begin;
	RendererSubmitFunction *submit;
	RendererEndFunction    *end;
}RendererBackend;

internal RendererBackend renderer_backend_load(void);
internal void renderer_backend_unload(RendererBackend backend);

#endif //FORGE_RENDERER_H