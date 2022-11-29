internal RendererBackend
renderer_backend_load(String8 path)
{
	RendererBackend backend = {0};
	backend.library = os_library_load(path);
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