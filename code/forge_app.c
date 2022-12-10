
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
	
	f32 fps_max     = 60.0f;
	f32 period_max  = 1.0f / fps_max;
	
	f64 begin_ticks = os_get_now_time();
	f64 end_ticks   = 0;
	end_ticks       = os_get_now_time();
	
	LARGE_INTEGER perf_count_frequency_result;
    QueryPerformanceFrequency(&perf_count_frequency_result);
	f64 perf_count_frequency = perf_count_frequency_result.QuadPart;
	
	LARGE_INTEGER last_counter;
	f64 last_cycle_count;
	
	LARGE_INTEGER end_counter;
	QueryPerformanceCounter(&end_counter);
	last_counter = end_counter;
	
	while (!app_state.is_quit)
	{
		f64 end_cycle_count = (f64)__rdtsc();
		QueryPerformanceCounter(&end_counter);
		
		f64 cycles_elapsed  = (f64)end_cycle_count - last_cycle_count;
		f64 counter_elapsed = (f64)end_counter.QuadPart - last_counter.QuadPart;
		f32 ms_per_frame    = (f32)counter_elapsed / (f32)perf_count_frequency;
		f32 fps             = (f32)perf_count_frequency / (f32)counter_elapsed;
		f32 mcpf            = (f32)cycles_elapsed / 1000000.0f;
		
#if 0
		LOG_TRACE("period_max:           %f", (f32)period_max);
		
		LOG_TRACE("perf_count_frequency: %f", (f32)perf_count_frequency);
		
		LOG_TRACE("end_counter:          %f", (f64)end_counter.QuadPart);
		LOG_TRACE("last_counter:         %f", (f64)last_counter.QuadPart);
		
		LOG_TRACE("end_cycle_count:      %f", (f64)end_cycle_count);
		LOG_TRACE("last_cycle_count:     %f", (f64)last_cycle_count);
		
		LOG_TRACE("cycles_elapsed:       %f", (f64)cycles_elapsed);
		LOG_TRACE("counter_elapsed:      %f", (f64)counter_elapsed);
		LOG_TRACE("ms_per_frame:         %f", (f32)ms_per_frame);
		LOG_TRACE("fps:                  %f", (f32)fps);
		LOG_TRACE("mcpf:                 %f", (f32)mcpf);
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
			
			//LOG_TRACE("[OS] fps: %.02ff/s, %.02fms/f, %.02fmc/f, %.02f/ghz/f", fps, ms_per_frame, mcpf, fps * mcpf);
			
			last_counter = end_counter;
			last_cycle_count = end_cycle_count;
		}
	}
	
	// Stop Modules
	{
		app_state.game_backend.stop();
	}
	
	os_shutdown();
}