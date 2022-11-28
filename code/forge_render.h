#ifndef FORGE_RENDER_H
#define FORGE_RENDER_H

typedef void RenderBeginFunction();
typedef void RenderSubmitFunction();
typedef void RenderEndFunction();

typedef struct RenderBackend
{
	void                 *library;
	RenderBeginFunction  *begin;
	RenderSubmitFunction *submit;
	RenderEndFunction    *end;
}RenderBackend;

internal RenderBackend render_backend_load(String8 path);
internal void render_backend_unload(RenderBackend backend);

#endif //FORGE_RENDER_H