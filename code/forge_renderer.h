#ifndef FORGE_RENDERER_H
#define FORGE_RENDERER_H

typedef void RendererInitFunction(void);
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

internal RendererBackend renderer_backend_load(String8 path);
internal void renderer_backend_unload(RendererBackend backend);

#endif //FORGE_RENDERER_H