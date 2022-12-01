internal RendererBackend
renderer_backend_load()
{
	String8 renderer_path = {0};
#if FR_VULKAN
	renderer_path = str8_lit("renderer_vulkan");
#elif FR_OPENGL
	renderer_path = str8_lit("renderer_opengl");
#else
#error missing renderer detection
#endif
	RendererBackend backend = {0};
	backend.library = os_library_load(renderer_path);
	backend.init    = (RendererInitFunction *)   os_library_load_symbol(backend.library, str8_lit("init"));
	backend.begin   = (RendererBeginFunction *)  os_library_load_symbol(backend.library, str8_lit("begin"));
	backend.submit  = (RendererSubmitFunction *) os_library_load_symbol(backend.library, str8_lit("submit"));
	backend.end     = (RendererEndFunction *)    os_library_load_symbol(backend.library, str8_lit("end"));
	return backend;
}

internal void
renderer_backend_unload(RendererBackend backend)
{
	os_library_unload(backend.library);
}