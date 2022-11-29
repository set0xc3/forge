global AppState app;

internal void 
app_entry_point(void)
{
	logger_print_all();
	
	app.window_state = os_window_create(str8_lit("Forge"), v2i(0, 0), v2i(800, 600));
	os_window_visibility(app.window_state->handle, true);
	
	RendererBackend renderer_backend = renderer_backend_load(str8_lit("renderer_opengl"));
	renderer_backend.init();
	
	while (!app.is_quit)
	{
		if (!os_window_process_event(app.window_state->handle))
		{
			app.is_quit = true;
		}
		
		renderer_backend.begin(app.window_state->handle);
		renderer_backend.submit(app.window_state->handle);
		renderer_backend.end(app.window_state->handle);
		
		os_sleep(1);
	}
}