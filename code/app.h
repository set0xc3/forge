#ifndef APP_H
#define APP_H

typedef struct AppState
{
	b8 is_quit;
	WindowState *window_state;
	
	Clock clock;
}AppState;

internal void app_entry_point();

#endif //APP_H