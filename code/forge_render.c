internal RenderBackend
render_backend_load(String8 path)
{
	RenderBackend backend = {0};
	backend.library = os_library_load(path);
	backend.begin   = (RenderBeginFunction *)  os_library_load_symbol(backend.library, str8_lit("begin"));
	backend.submit  = (RenderSubmitFunction *) os_library_load_symbol(backend.library, str8_lit("submit"));
	backend.end     = (RenderEndFunction *)    os_library_load_symbol(backend.library, str8_lit("end"));
	return backend;
}

internal void
render_backend_unload(RenderBackend backend)
{
	os_library_unload(backend.library);
}