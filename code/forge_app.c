
global AppState app_state;

internal void 
app_run(void)
{
	os_setup();
	
	app_state.window_state = os_window_create(str8_lit("Forge"), v2i(0, 0), v2i(800, 600));
	os_window_visibility(app_state.window_state->handle, true);
	
	RendererBackend renderer_backend = renderer_backend_load();
	renderer_backend.init(app_state.window_state->handle);
	
	app_state.game_backend.libray_handle = os_library_load(str8_lit("game"));
	app_state.game_backend.start         = (GameStartFunction *)  os_library_load_symbol(app_state.game_backend.libray_handle, str8_lit("start"));
	app_state.game_backend.update        = (GameUpdateFunction *) os_library_load_symbol(app_state.game_backend.libray_handle, str8_lit("update"));
	app_state.game_backend.stop          = (GameStopFunction *)   os_library_load_symbol(app_state.game_backend.libray_handle, str8_lit("stop"));
	
	// Start Modules
	{
		app_state.game_backend.start();
	}
	
	local_const f32 fps_max        = 60.0f;
	local_const f32 period_max     = 1.0f / fps_max;
	const f64 perf_count_frequency = os_time_frequency();
	
	u64 begin_counter  = os_time_now();
	u64 end_counter = 0;
	
	while (!app_state.is_quit)
	{
		begin_counter       = os_time_now();
		
		f64 counter_elapsed = (f64)(begin_counter - end_counter);
		f32 ms_per_frame    = (f32)(counter_elapsed / perf_count_frequency);
		f32 fps             = (f32)(perf_count_frequency / counter_elapsed);
#if 0
#endif
		
#if 1
		if (ms_per_frame >= period_max)
#endif
		{
			if (ms_per_frame >= 1.0f)
			{
				ms_per_frame = period_max;
			}
			
			if (!os_window_process_event(app_state.window_state->handle))
			{
				app_state.is_quit = true;
			}
			
			// Update Modules
			{
				app_state.game_backend.update(ms_per_frame);
			}
			
			//for (u64 i = 0; i < 1000000; i++)
			{
				renderer_backend.begin(app_state.window_state->handle);
				renderer_backend.submit(app_state.window_state->handle);
				renderer_backend.end(app_state.window_state->handle);
			}
			
			{
				static f64 frame_per_sec = 0;
				frame_per_sec += ms_per_frame;
				if (frame_per_sec >= 1.0f)
				{
					LOG_TRACE("[OS] fps: %.02ff/s, %.02fms/f", (f32)fps, ms_per_frame);
					frame_per_sec = 0;
				}
			}
			
			end_counter = begin_counter;
		}
	}
	
	// Stop Modules
	{
		app_state.game_backend.stop();
	}
	
	os_shutdown();
}