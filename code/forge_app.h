#ifndef FORGE_APP_H
#define FORGE_APP_H

typedef void GameStartFunction(void);
typedef void GameUpdateFunction(f32 delta_time);
typedef void GameStopFunction(void);

typedef struct GameBackend
{
	void               *libray_handle;
	GameStartFunction  *start;
	GameUpdateFunction *update;
	GameStopFunction   *stop;
}GameBackend;

typedef struct AppState
{
	b8 is_quit;
	WindowState *window_state;
	GameBackend  game_backend;
}AppState;

internal void app_run(void);

#endif //FORGE_APP_H