global AppState app;

internal void 
app_entry_point(void)
{
	logger_print_all();
	
	os_setup();
	
	app.window_state = os_window_create(str8_lit("Forge"), v2i(0, 0), v2i(800, 600));
	os_window_visibility(app.window_state->handle, true);
	
	RendererBackend renderer_backend = renderer_backend_load();
	renderer_backend.init(app.window_state->handle);
	
	f64 running_time         = 0.0;
	f64 last_time            = 0.0;
	i64 frame_count          = 0;
	f64 target_frame_seconds = 1.0 / 60;
	
	os_clock_start(&app.clock);
	os_clock_update(&app.clock);
	last_time = app.clock.elapsed;
	
	while (!app.is_quit)
	{
		os_clock_update(&app.clock);
		f64 current_time     = app.clock.elapsed;
		f64 delta_time       = (current_time - last_time);
		f64 frame_start_time = os_get_now_time();
		
		if (!os_window_process_event(app.window_state->handle))
		{
			app.is_quit = true;
		}
		
		//for (i64 i = 0; i < 10000000; i += 1)
		{
			
		}
		
		renderer_backend.begin(app.window_state->handle);
		renderer_backend.submit(app.window_state->handle);
		renderer_backend.end(app.window_state->handle);
		
		f64 frame_end_time     = os_get_now_time();
		f64 frame_elapsed_time = (frame_end_time - frame_start_time);
		f64 remaining_seconds  = (target_frame_seconds - frame_elapsed_time);
		running_time += frame_elapsed_time;
		
		if (remaining_seconds > 0)
		{
			frame_count += 1;
		}
		
		last_time = current_time;
		
		os_sleep(1);
	}
	os_clock_stop(&app.clock);
	
	os_shutdown();
}