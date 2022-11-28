#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <X11/Xutil.h>

#include <GL/glx.h>

#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>
#include <fcntl.h>
#include <dlfcn.h> // Load library

internal void main_init(void);

typedef struct LinuxInternalState
{
	int32 visual_id;
	XVisualInfo visual;
	Display *display;
	struct
	{
		GLXWindow window;
		GLXDrawable drawable;
		GLXFBConfig fb_config;
		GLXContext context;
	}glx;
	
	struct
	{
		xcb_connection_t *connection;
		xcb_screen_t *screen;
		xcb_window_t window;
		xcb_gcontext_t context;
		xcb_colormap_t colormap;
	}xcb;
	
	struct
	{
		String8 title;
		V2S position;
		V2S size;
	}window;
}LinuxInternalState;

global OS_Context *os_ctx;
global OS_EventList *os_events;

FR_API void 
os_init()
{
	os_events = (OS_EventList *)MEMORY_MALLOC(sizeof(OS_EventList));
	MEMORY_ZERO_STRUCT(os_events, sizeof(OS_EventList));
	
	os_ctx = (OS_Context *)MEMORY_MALLOC(sizeof(OS_Context));
	MEMORY_ZERO_STRUCT(os_ctx, sizeof(OS_Context));
}

FR_API OS_Event 
*os_get_next_event()
{
	OS_Event *event = os_events->head;
	if (event)
	{
		lld_remove(os_events, os_events->head);
		return event;
	}
	else
		return 0;
}

FR_API void 
os_delay(i32 ms)
{
	if (ms >= 1000)
		sleep(ms / 1000);
	else
		usleep(ms);
}

FR_API String8 
os_file_read(const char *path)
{
	String8 result = {};
	FILE *file = fopen(path , "r");
	if (file != 0)
	{
		fseek(file, 0, SEEK_END);
		result.size = ftell(file) - 1;
		result.str = malloc(result.size);
		
		fseek(file, 0, SEEK_SET);
		fread(result.str, 1, result.size, file);
		
		fclose(file);
	}
	return result;
}

FR_API void 
*os_library_load(const char *path)
{
	char new_str[256] = {0};
	strcpy(new_str, path);
	strncat(new_str, FR_PLATFORM_EXT, strlen(new_str));
	
	void *result = dlopen(new_str, RTLD_LAZY); 
	return result;
}

FR_API VoidFunction 
*os_library_load_func(void *handle, const char *name)
{
	VoidFunction *result = (VoidFunction *)dlsym(handle, name); 
	return result;
}

FR_API void 
os_library_unload(void *handle)
{
	dlclose(handle);
}

FR_API void 
*os_window_create(String8 title, V2S position, V2S size)
{
	LinuxInternalState *in = MEMORY_MALLOC(sizeof(LinuxInternalState));
	MEMORY_ZERO_STRUCT(in, sizeof(LinuxInternalState));
	
	int32 visual_attribs[] =
	{
		GLX_X_RENDERABLE,  True,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_RENDER_TYPE,   GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE,      8,
		GLX_GREEN_SIZE,    8,
		GLX_BLUE_SIZE,     8,
		GLX_ALPHA_SIZE,    8,
		GLX_DEPTH_SIZE,    24,
		GLX_STENCIL_SIZE,  8,
		GLX_DOUBLEBUFFER,  True,
		//GLX_SAMPLE_BUFFERS, 1,
		//GLX_SAMPLES, 4,
		None
	};
	
	in->window.title    = title;
	in->window.position = position;
	in->window.size     = size;
	
	in->display       = XOpenDisplay(0);
	in->visual.visual = DefaultVisual(in->display, 0);
	in->visual.screen = DefaultScreen(in->display);
	in->visual.depth  = XDefaultDepth(in->display, in->visual.screen);
	
	in->xcb.connection = XGetXCBConnection(in->display);
	in->xcb.colormap   = xcb_generate_id(in->xcb.connection);
	in->xcb.window     = xcb_generate_id(in->xcb.connection);
	in->xcb.context    = xcb_generate_id(in->xcb.connection);
	in->xcb.screen     = xcb_setup_roots_iterator(xcb_get_setup(in->xcb.connection)).data;
	
	// Get GLXFBConfig
	{
		int32 fb_config_count = 0;
		
		in->glx.fb_config = glXChooseFBConfig(in->display, in->visual.screen, visual_attribs, &fb_config_count)[0];
		glXGetFBConfigAttrib(in->display, in->glx.fb_config, GLX_VISUAL_ID , &in->visual_id);
		
		in->glx.context = glXCreateNewContext(in->display, in->glx.fb_config, GLX_RGBA_TYPE, 0, True);
	}
	
	xcb_create_colormap(in->xcb.connection,
											XCB_COLORMAP_ALLOC_NONE,
											in->xcb.colormap,
											in->xcb.screen->root,
											in->visual_id);
	
	i32 event_mask = 
		XCB_EVENT_MASK_STRUCTURE_NOTIFY |
		XCB_EVENT_MASK_ENTER_WINDOW |
		XCB_EVENT_MASK_LEAVE_WINDOW |
		XCB_EVENT_MASK_FOCUS_CHANGE |
		XCB_EVENT_MASK_EXPOSURE | 
		XCB_EVENT_MASK_BUTTON_PRESS | 
		XCB_EVENT_MASK_BUTTON_RELEASE | 
		XCB_EVENT_MASK_OWNER_GRAB_BUTTON | 
		XCB_EVENT_MASK_KEY_PRESS | 
		XCB_EVENT_MASK_KEY_RELEASE | 
		XCB_EVENT_MASK_POINTER_MOTION;
	i32 value_list[] = { event_mask, in->xcb.colormap, 0 };
	i32 value_mask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;
	
	xcb_create_window(in->xcb.connection, 
										XCB_COPY_FROM_PARENT, 
										in->xcb.window, 
										in->xcb.screen->root,
										in->window.position.x, in->window.position.y,
										in->window.size.x, in->window.size.y,
										0,
										XCB_WINDOW_CLASS_INPUT_OUTPUT, 
										in->visual_id,
										value_mask, 
										value_list);
	
	{
		// Set window name
		xcb_change_property(in->xcb.connection, 
												XCB_PROP_MODE_REPLACE, 
												in->xcb.window,
												XCB_ATOM_WM_NAME, 
												XCB_ATOM_STRING, 
												8,
												strlen((char *)in->window.title.str), 
												(char *)in->window.title.str);
		
		xcb_intern_atom_cookie_t protocols_cookie = xcb_intern_atom(in->xcb.connection, 0, strlen("WM_PROTOCOLS"), "WM_PROTOCOLS");
		xcb_intern_atom_reply_t* protocols_reply  = xcb_intern_atom_reply(in->xcb.connection, protocols_cookie, 0);
		
		xcb_intern_atom_cookie_t del_cookie = xcb_intern_atom(in->xcb.connection, 0, strlen("WM_DELETE_WINDOW"), "WM_DELETE_WINDOW");
		xcb_intern_atom_reply_t* del_reply  = xcb_intern_atom_reply(in->xcb.connection, del_cookie, 0);
		
		xcb_change_property(in->xcb.connection, 
												XCB_PROP_MODE_REPLACE,
												in->xcb.window,
												protocols_reply->atom, 
												XCB_ATOM_ATOM, 
												32,
												1,
												&del_reply->atom);
		MEMORY_FREE(protocols_reply);
		MEMORY_FREE(del_reply);
	}
	
	in->glx.drawable = in->glx.window = glXCreateWindow(in->display, in->glx.fb_config, in->xcb.window, 0);
	
	xcb_map_window(in->xcb.connection, in->xcb.window);
	xcb_flush(in->xcb.connection);
	
	// Make OpenGL context current
	glXMakeContextCurrent(in->display, in->glx.drawable, in->glx.drawable, in->glx.context);
	
	// TODO
	{
		//const static i32_t values[] = { 800, 600 };
    //xcb_configure_window(in->xcb.connection, in->xcb.window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, values);
	}
	
	LOG_INFO("Create window: '%s', [%i, %i], [%i, %i]", 
					 in->window.title.str, 
					 in->window.position.x, in->window.position.y, 
					 in->window.size.x, in->window.size.y);
	
	return (void *)in;
}

FR_API void 
os_window_destroy(void *handle)
{
	LinuxInternalState *in = (LinuxInternalState *)handle;
	
	XCloseDisplay(in->display);
	xcb_destroy_window(in->xcb.connection, in->xcb.window);
	
	LOG_DEBUG("Destroy window: '%s', [%i, %i], [%i, %i]", 
						in->window.title.str, 
						in->window.position.x, in->window.position.y, 
						in->window.size.x, in->window.size.y);
}

FR_API void 
os_window_visibility(void *handle, bool32 state)
{
	LinuxInternalState *in = (LinuxInternalState *)handle;
	if (state)
	{
		xcb_map_window(in->xcb.connection, in->xcb.window);
		xcb_flush(in->xcb.connection);
	}
	else
	{
		xcb_unmap_window(in->xcb.connection, in->xcb.window);
	}
}

FR_API bool32 
os_window_process_event(void *handle)
{
	LinuxInternalState *in = (LinuxInternalState *)handle;
	
	for(xcb_generic_event_t *xcb_event = xcb_poll_for_event(in->xcb.connection); 
			xcb_event != 0;
			xcb_event = xcb_poll_for_event(in->xcb.connection))
	{
		OS_Event *os_event = 0;
		bool32 is_press = false;
		
		switch (xcb_event->response_type & ~0x80)
		{
			case XCB_CLIENT_MESSAGE:
			{
				LOG_INFO("System shutdown...");
				
				os_event = (OS_Event *)MEMORY_MALLOC(sizeof(OS_Event));
				MEMORY_ZERO_STRUCT(os_event, sizeof(OS_Event));
				
				os_event->code = OS_EVENT_CODE_WINDOW_CLOSE;
				break;
			}
			
#if 1
			case XCB_CONFIGURE_NOTIFY:
			{
				xcb_configure_notify_event_t *event = (xcb_configure_notify_event_t *)xcb_event;
				
				os_event = (OS_Event *)MEMORY_MALLOC(sizeof(OS_Event));
				MEMORY_ZERO_STRUCT(os_event, sizeof(OS_Event));
				
				os_event->code = OS_EVENT_CODE_WINDOW_CONFIGURE;
				os_event->window.x = event->x;
				os_event->window.y = event->y;
				os_event->window.width = event->width;
				os_event->window.height = event->height;
				
				os_ctx->window.position.y = event->y;
				os_ctx->window.position.x = event->x;
				os_ctx->window.size.x = event->width;
				os_ctx->window.size.y = event->height;
				
				break;
			}
#endif
			
#if 1
			case XCB_MOTION_NOTIFY:
			{
				xcb_motion_notify_event_t *event = (xcb_motion_notify_event_t *)xcb_event;
				
				os_event = (OS_Event *)MEMORY_MALLOC(sizeof(OS_Event));
				MEMORY_ZERO_STRUCT(os_event, sizeof(OS_Event));
				
				os_event->code = OS_EVENT_CODE_MOUSE_MOVED;
				os_event->mouse.position[0] = event->event_x;
				os_event->mouse.position[1] = event->event_y;
				break;
			}
#endif
			
#if 0
			case XCB_EXPOSE:
			{
				xcb_expose_event_t *event = (xcb_expose_event_t *)xcb_event;
				
				os_event = (OS_Event *)MEMORY_MALLOC(sizeof(OS_Event));
				MEMORY_ZERO_STRUCT(os_event, sizeof(OS_Event));
				
				LOG_DEBUG("(%i,%i) (%i,%i)", event->x, event->y, event->width, event->height);
				break;
			}
#endif
			
#if 1
			/* Keyboard Event */
			case XCB_EVENT_MASK_KEY_PRESS:
			is_press = true;
			case XCB_EVENT_MASK_KEY_RELEASE:
			{
				xcb_key_release_event_t *key = (xcb_key_release_event_t *)xcb_event;
				
				os_event = (OS_Event *)MEMORY_MALLOC(sizeof(OS_Event));
				MEMORY_ZERO_STRUCT(os_event, sizeof(OS_Event));
				
				XKeyEvent x11_key;
				x11_key.display = in->display;
				x11_key.keycode = key->detail;
				x11_key.state = key->state;
				
				char buffer[32] = {};
				if (XLookupString(&x11_key, buffer, ARRAY_COUNT(buffer), 0, 0)) 
				{
					LOG_DEBUG("key_raw: %i, %i", x11_key.keycode, x11_key.state);
					LOG_DEBUG("key: %c, %u, 0x%X", *buffer, *buffer, *buffer);
				}
				
				if ((int32)*buffer == 97) os_event->key.state = is_press ? 1 : 0; os_event->key.code = OS_KEY_CODE_A; os_event->code = is_press ? OS_EVENT_CODE_KEY_PRESSED : OS_EVENT_CODE_KEY_RELEASED;;
				
				break;
			}
#endif
			
			/* Mouse Event */
			case XCB_BUTTON_PRESS:
			is_press = true;
			case XCB_BUTTON_RELEASE:
			{
				xcb_button_release_event_t *button = (xcb_button_release_event_t *)xcb_event;
				
				os_event = (OS_Event *)MEMORY_MALLOC(sizeof(OS_Event));
				MEMORY_ZERO_STRUCT(os_event, sizeof(OS_Event));
				
				switch (button->detail)
				{
					case Button1: os_event->button.state = is_press ? 1 : 0; os_event->button.code = OS_MOUSE_BUTTON_LEFT;   os_event->code = is_press ? OS_EVENT_CODE_BUTTON_PRESSED : OS_EVENT_CODE_BUTTON_RELEASED; break;
					case Button2: os_event->button.state = is_press ? 1 : 0; os_event->button.code = OS_MOUSE_BUTTON_MIDDLE; os_event->code = is_press ? OS_EVENT_CODE_BUTTON_PRESSED : OS_EVENT_CODE_BUTTON_RELEASED; break;
					case Button3: os_event->button.state = is_press ? 1 : 0; os_event->button.code = OS_MOUSE_BUTTON_RIGHT;  os_event->code = is_press ? OS_EVENT_CODE_BUTTON_PRESSED : OS_EVENT_CODE_BUTTON_RELEASED; break;
					
					case Button4:
					case Button5:
					{
						if (is_press)
						{
							os_event->code = OS_EVENT_CODE_MOUSE_SCROLLED;
							os_event->mouse.scroll[1] = (button->detail == 4) ? 1 : -1;
						}
						else
						{
							MEMORY_FREE(os_event);
							os_event = 0;
						}
						
						break;
					}
					default: break;
				}
				break;
			}
			default: break;
		}
		
		if (os_event)
		{
			lld_push_back(os_events, os_event);
			os_event = 0;
		}
		
		MEMORY_FREE(xcb_event);
	}
	
	return true;
}