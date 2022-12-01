#ifndef FORGE_RENDERER_OPENGL_H
#define FORGE_RENDERER_OPENGL_H

internal u32 gl_create_shader(const char *vs_path, const char *fs_path);

internal void gl_draw_rectangle(Vector2f min, Vector2f max, Vector4f color);

internal void gl_os_init(void *window_handle);
internal void gl_os_select_window(void *window_handle);
internal void gl_os_finish(void *window_handle);

#endif //FORGE_RENDERER_OPENGL_H