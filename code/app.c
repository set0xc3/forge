global AppState app;

internal void 
app_entry_point(void)
{
	logger_print_all();
	
	app.window_state = os_window_create(str8_lit("Forge"), v2i(0, 0), v2i(800, 600));
	os_window_visibility(app.window_state->handle, true);
	
	//RenderBackend render_backend = render_backend_load(str8_lit("render_opengl"));
	//render_backend_unload(render_backend);
	
	while (!app.is_quit)
	{
		if (!os_window_process_event(app.window_state->handle))
		{
			app.is_quit = true;
		}
		
		//render_backend.begin();
		//render_backend.submit();
		//render_backend.end();
		
		os_sleep(1);
	}
}