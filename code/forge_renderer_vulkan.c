#include <forge.h>

#include <vulkan/vulkan.h>
#include "forge_renderer_vulkan.h"

global VkInstance        vk_instance;
global_const char      **vk_available_extensions;
global_const char       *vk_enabled_extensions[2];
global_const char       *vk_validation_layers[1];
global VkPhysicalDevice *vk_physical_devices;
global u32               vk_physical_devices_count;

#ifdef FR_BUILD_SLOW
global_const b8 enable_validation_layers = true;
#else
global_const b8 enable_validation_layers = false;
#endif

internal void 
vk_create_instance(void)
{
    VkApplicationInfo app_info  = {0};
    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName   = "None";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName        = "None";
    app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion         = VK_API_VERSION_1_3;
	
	VkInstanceCreateInfo create_info = {0};
	create_info.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo     = &app_info;
	
	vk_enabled_extensions[0] = "VK_KHR_surface";
	vk_enabled_extensions[1] = "VK_KHR_win32_surface";
	
	create_info.enabledExtensionCount   = ARRAY_COUNT(vk_enabled_extensions);
	create_info.ppEnabledExtensionNames = vk_enabled_extensions;
	
	vk_validation_layers[0] = "VK_LAYER_KHRONOS_validation";
	
	create_info.enabledLayerCount = ARRAY_COUNT(vk_validation_layers);
    create_info.ppEnabledLayerNames = vk_validation_layers;
	
	u32 version = 0;
	if (vkEnumerateInstanceVersion(&version) != VK_SUCCESS)
	{
		ASSERT(true);
	}
	LOG_INFO("vk.version: v%u.%u", 
			 VK_VERSION_MAJOR(version), 
			 VK_VERSION_MINOR(version));
	
	VkExtensionProperties* ep = 0;
	u32 ext_count = 0;
	
	if (vkEnumerateInstanceExtensionProperties(0, &ext_count, 0) != VK_SUCCESS)
	{
		ASSERT(true);
	}
	u32 ep_size = sizeof(VkExtensionProperties) * ext_count;
	ep = os_memory_reserve(ep_size);
	
	if (vkEnumerateInstanceExtensionProperties(0, &ext_count, ep) != VK_SUCCESS)
	{
		ASSERT(true);
	}
	LOG_INFO("vk.extension_count: %i", ext_count);
	
	vk_available_extensions = os_memory_reserve(sizeof(char **) * ext_count);
	for (u32 i = 0;  i < ext_count;  i++)
    {
		vk_available_extensions[i] = ep[i].extensionName;
		LOG_INFO("vk.extension_name: %s : v%u", vk_available_extensions[i], ep[i].specVersion);
    }
	
	if (vkCreateInstance(&create_info, 0, &vk_instance) != VK_SUCCESS)
	{
		ASSERT(true);
	}
	
	os_memory_release(ep);
}

internal void 
vk_create_pipeline(String8 vs_path, String8 fs_path)
{
	FileInfo vs_dump = os_file_read(vs_path);
	FileInfo fs_dump = os_file_read(fs_path);
	
	LOG_INFO("vs_dump.size: %u", vs_dump.size);
	LOG_INFO("fs_dump.size: %u", fs_dump.size);
	
	os_memory_release(vs_dump.data);
	os_memory_release(fs_dump.data);
}

internal void 
vk_pick_physical_device(void)
{
	vkEnumeratePhysicalDevices(vk_instance, &vk_physical_devices_count, 0);
	LOG_INFO("vk.physical.device.count: %u", vk_physical_devices_count);
	
	vk_physical_devices = os_memory_reserve(sizeof(VkPhysicalDevice) * vk_physical_devices_count);
	vkEnumeratePhysicalDevices(vk_instance, &vk_physical_devices_count, vk_physical_devices);
	
	for (u32 i = 0; i < vk_physical_devices_count; i++)
	{
		if (vk_is_device_suitable(&vk_physical_devices[i]))
		{
			LOG_INFO("vk.physical.device: true");
		}
	}
}

internal b8 
vk_is_device_suitable(VkPhysicalDevice *device)
{
	VkPhysicalDeviceProperties device_properties;
	vkGetPhysicalDeviceProperties(*device, &device_properties);
	
	VkPhysicalDeviceFeatures device_features;
	vkGetPhysicalDeviceFeatures(*device, &device_features);
	
	return device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		device_features.geometryShader;
}

FR_API void
init(void *window_handle)
{
	LOG_INFO("[VULKAN] init");
	
	vk_create_instance();
	vk_pick_physical_device();
	
	vk_create_pipeline(str8_lit("assets/shaders/shader.vert.spv"),
					   str8_lit("assets/shaders/shader.frag.spv"));
}

FR_API void 
begin(void *window_handle)
{
}

FR_API void 
submit(void *window_handle)
{
}

FR_API void 
end(void *window_handle)
{
}
