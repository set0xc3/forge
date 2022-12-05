#ifndef FORGE_RENDERER_VULKAN_H
#define FORGE_RENDERER_VULKAN_H

internal void vk_create_instance();
internal void vk_create_pipeline(String8 vs_path, String8 fs_path);
internal void vk_pick_physical_device();
internal b8   vk_is_device_suitable(VkPhysicalDevice *device);

#endif //FORGE_RENDERER_VULKAN_H