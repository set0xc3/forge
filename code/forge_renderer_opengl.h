#ifndef FORGE_RENDERER_OPENGL_H
#define FORGE_RENDERER_OPENGL_H

internal u32 gl_shader_create(const char *vs_path, const char *fs_path);

internal void gl_os_init(void *window_handle);
internal void gl_os_window_select(void *window_handle);
internal void gl_os_finish(void *window_handle);

#endif //FORGE_RENDERER_OPENGL_H