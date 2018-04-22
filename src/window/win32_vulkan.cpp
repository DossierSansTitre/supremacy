#include <vector>
#define VK_USE_PLATFORM_WIN32_KHR 1
#include <window/win32_vulkan.h>
#include <log.h>

WindowWin32Vulkan::WindowWin32Vulkan(HWND window, VkInstance instance)
: WindowWin32(window)
, _instance(instance)
{
	init_device();
	init_surface();
	init_swapchain();
}

WindowWin32Vulkan::~WindowWin32Vulkan()
{

}

#include <cstdio>

WindowWin32Vulkan* WindowWin32Vulkan::create(HWND window, int major, int minor)
{
	VkInstance instance;

	const char* extensions[] = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
	};

	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Supremacy";
	app_info.apiVersion = VK_MAKE_VERSION(major, minor, 0);

	VkInstanceCreateInfo instance_info = {};
	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.pApplicationInfo = &app_info;
	instance_info.enabledExtensionCount = 2;
	instance_info.ppEnabledExtensionNames = extensions;

	if (vkCreateInstance(&instance_info, nullptr, &instance) != VK_SUCCESS)
	{
		DestroyWindow(window);
		return nullptr;
	}
	return new WindowWin32Vulkan(window, instance);
}

void WindowWin32Vulkan::swap()
{

}

void WindowWin32Vulkan::vsync(bool sync)
{
	Window::vsync(sync);
}

void WindowWin32Vulkan::init_device()
{
	float priority = 1.f;
	uint32_t physical_device_count;
	VkPhysicalDevice physical_devices[1];
	VkPhysicalDeviceProperties properties;

	physical_device_count = 1;
	vkEnumeratePhysicalDevices(_instance, &physical_device_count, physical_devices);

	vkGetPhysicalDeviceProperties(physical_devices[0], &properties);
	log_line(LogLevel::Info, "GPU Properties:");
	log_line(LogLevel::Info, "  VendorID:      0x%08x", properties.vendorID);
	log_line(LogLevel::Info, "  DeviceID:      0x%08x", properties.deviceID);
	log_line(LogLevel::Info, "  DeviceName:    %s", properties.deviceName);
	log_line(LogLevel::Info, "  DriverVersion: %d.%d.%d", VK_VERSION_MAJOR(properties.driverVersion), VK_VERSION_MINOR(properties.driverVersion), VK_VERSION_PATCH(properties.driverVersion));

	VkDeviceQueueCreateInfo device_queue_info{};
	device_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	device_queue_info.queueCount = 1;
	device_queue_info.pQueuePriorities = &priority;

	const char* device_extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkDeviceCreateInfo device_info{};
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.queueCreateInfoCount = 1;
	device_info.pQueueCreateInfos = &device_queue_info;
	device_info.enabledExtensionCount = 1;
	device_info.ppEnabledExtensionNames = device_extensions;

	vkCreateDevice(physical_devices[0], &device_info, nullptr, &_device);
	_physical_device = physical_devices[0];
}

void WindowWin32Vulkan::init_surface()
{
	VkWin32SurfaceCreateInfoKHR surface_info{};
	surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surface_info.hinstance = GetModuleHandle(nullptr);
	surface_info.hwnd = _window;

	vkCreateWin32SurfaceKHR(_instance, &surface_info, nullptr, &_surface);
}

void WindowWin32Vulkan::init_swapchain()
{
	VkSurfaceCapabilitiesKHR caps{};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physical_device, _surface, &caps);

	VkExtent2D extent{};
	if (caps.currentExtent.width == -1 || caps.currentExtent.height == -1)
	{
		extent.width = _width;
		extent.height = _height;
	}
	else
		extent = caps.currentExtent;

	uint32_t present_mode_count = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(_physical_device, _surface, &present_mode_count, nullptr);
	std::vector<VkPresentModeKHR> modes(present_mode_count);
	vkGetPhysicalDeviceSurfacePresentModesKHR(_physical_device, _surface, &present_mode_count, modes.data());

	_present_mode = VK_PRESENT_MODE_FIFO_KHR;
	_present_mode_vsync = VK_PRESENT_MODE_FIFO_KHR;

	for (auto m : modes)
	{
		if (m == VK_PRESENT_MODE_MAILBOX_KHR)
			_present_mode_vsync = VK_PRESENT_MODE_MAILBOX_KHR;
		if (m == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
			_present_mode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
	}

	uint32_t min_image_count;

	min_image_count = caps.minImageCount + 1;
	if (min_image_count > caps.maxImageCount)
		min_image_count = caps.maxImageCount;
	VkSwapchainCreateInfoKHR swapchain_info{};
	swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_info.surface = _surface;
	swapchain_info.minImageCount = min_image_count;
	//swapchain_info.imageFormat = 
}