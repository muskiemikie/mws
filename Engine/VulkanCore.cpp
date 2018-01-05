/*
 * VulkanCore.cpp
 *
 *  Created on: Jan 4, 2018
 *      Author: michaels
 */

#include "VulkanCore.h"

namespace mws {

static VKAPI_ATTR VkBool32 VKAPI_CALL MyDebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object,
    size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData) {
	std::cerr << "Validation layer: " << pMessage << std::endl;

    return VK_FALSE;
}

VulkanCore::VulkanCore() {
}

VulkanCore::~VulkanCore() {
	Cleanup();
}

bool VulkanCore::Initialize(WindowParameters& window) {
	if (!LoadVulkan()) {
		return false;
	}

	if (!InitVulkan(window)) {
		return false;
	}

	if (!SetupDebugCallback()) {
		return false;
	}

	if (!CreateSurface(window)) {
		return false;
	}

	if (!PickPhysicalDevice()) {
		return false;
	}

	if (!CreateLogicalDevice()) {
		return false;
	}

	return true;
}

bool VulkanCore::LoadVulkan() {
	// Dynamically load Vulkan library
    HMODULE vulkan_library = LoadLibrary("vulkan-1.dll");
    if (!vulkan_library) {
    	std::cerr << __FUNCTION__ << " failed in " << __FILE__ << ":" << __LINE__ << "; " << "Could not load Vulkan library!" << std::endl;
    	return false;
    }

    vkCreateInstance = (PFN_vkCreateInstance) GetProcAddress(vulkan_library, "vkCreateInstance");
    vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties) GetProcAddress(vulkan_library, "vkEnumerateInstanceLayerProperties");
    vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties) GetProcAddress(vulkan_library, "vkEnumerateInstanceExtensionProperties");
    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr) GetProcAddress(vulkan_library, "vkGetInstanceProcAddr");

    vkEnumeratePhysicalDevices = reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(GetProcAddress(vulkan_library, "vkEnumeratePhysicalDevices"));
    vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties) GetProcAddress(vulkan_library, "vkGetPhysicalDeviceProperties");
    vkGetPhysicalDeviceFeatures = (PFN_vkGetPhysicalDeviceFeatures) GetProcAddress(vulkan_library, "vkGetPhysicalDeviceFeatures");
    vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties) GetProcAddress(vulkan_library, "vkGetPhysicalDeviceQueueFamilyProperties");
    vkGetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties) GetProcAddress(vulkan_library, "vkGetPhysicalDeviceMemoryProperties");
    vkCreateDevice = (PFN_vkCreateDevice) GetProcAddress(vulkan_library, "vkCreateDevice");
    vkGetDeviceQueue = (PFN_vkGetDeviceQueue) GetProcAddress(vulkan_library, "vkGetDeviceQueue");
    vkCreateCommandPool = (PFN_vkCreateCommandPool) GetProcAddress(vulkan_library, "vkCreateCommandPool");
    vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers) GetProcAddress(vulkan_library, "vkAllocateCommandBuffers");
    vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR) GetProcAddress(vulkan_library, "vkCreateSwapchainKHR");
    vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR) GetProcAddress(vulkan_library, "vkGetSwapchainImagesKHR");
    vkCreateFence = (PFN_vkCreateFence) GetProcAddress(vulkan_library, "vkCreateFence");
    vkCreateSemaphore = (PFN_vkCreateSemaphore) GetProcAddress(vulkan_library, "vkCreateSemaphore");
    vkAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR) GetProcAddress(vulkan_library, "vkAcquireNextImageKHR");
    vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer) GetProcAddress(vulkan_library, "vkBeginCommandBuffer");
    vkCmdPipelineBarrier = (PFN_vkCmdPipelineBarrier) GetProcAddress(vulkan_library, "vkCmdPipelineBarrier");
    vkEndCommandBuffer = (PFN_vkEndCommandBuffer) GetProcAddress(vulkan_library, "vkEndCommandBuffer");
    vkQueueSubmit = (PFN_vkQueueSubmit) GetProcAddress(vulkan_library, "vkQueueSubmit");
    vkWaitForFences = (PFN_vkWaitForFences) GetProcAddress(vulkan_library, "vkWaitForFences");
    vkResetFences = (PFN_vkResetFences) GetProcAddress(vulkan_library, "vkResetFences");
    vkDestroySemaphore = (PFN_vkDestroySemaphore) GetProcAddress(vulkan_library, "vkDestroySemaphore");
    vkResetCommandBuffer = (PFN_vkResetCommandBuffer) GetProcAddress(vulkan_library, "vkResetCommandBuffer");
    vkQueuePresentKHR = (PFN_vkQueuePresentKHR) GetProcAddress(vulkan_library, "vkQueuePresentKHR");
    vkCreateImageView =(PFN_vkCreateImageView) GetProcAddress(vulkan_library, "vkCreateImageView");
    vkCreateImage = (PFN_vkCreateImage) GetProcAddress(vulkan_library, "vkCreateImage");
    vkGetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements) GetProcAddress(vulkan_library, "vkGetImageMemoryRequirements");
    vkAllocateMemory = (PFN_vkAllocateMemory) GetProcAddress(vulkan_library, "vkAllocateMemory");
    vkBindImageMemory = (PFN_vkBindImageMemory) GetProcAddress(vulkan_library, "vkBindImageMemory");
    vkCreateRenderPass = (PFN_vkCreateRenderPass) GetProcAddress(vulkan_library, "vkCreateRenderPass");
    vkCreateFramebuffer = (PFN_vkCreateFramebuffer) GetProcAddress(vulkan_library, "vkCreateFramebuffer");
    vkCreateBuffer = (PFN_vkCreateBuffer) GetProcAddress(vulkan_library, "vkCreateBuffer");
    vkGetBufferMemoryRequirements = (PFN_vkGetBufferMemoryRequirements) GetProcAddress(vulkan_library, "vkGetBufferMemoryRequirements");
    vkMapMemory = (PFN_vkMapMemory) GetProcAddress(vulkan_library, "vkMapMemory");
    vkUnmapMemory = (PFN_vkUnmapMemory) GetProcAddress(vulkan_library, "vkUnmapMemory");
    vkBindBufferMemory = (PFN_vkBindBufferMemory) GetProcAddress(vulkan_library, "vkBindBufferMemory");
    vkCreateShaderModule = (PFN_vkCreateShaderModule) GetProcAddress(vulkan_library, "vkCreateShaderModule");
    vkCreatePipelineLayout = (PFN_vkCreatePipelineLayout) GetProcAddress(vulkan_library, "vkCreatePipelineLayout");
    vkCreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines) GetProcAddress(vulkan_library, "vkCreateGraphicsPipelines");
    vkCmdBeginRenderPass = (PFN_vkCmdBeginRenderPass) GetProcAddress(vulkan_library, "vkCmdBeginRenderPass");
    vkCmdBindPipeline = (PFN_vkCmdBindPipeline) GetProcAddress(vulkan_library, "vkCmdBindPipeline");
    vkCmdSetViewport = (PFN_vkCmdSetViewport) GetProcAddress(vulkan_library, "vkCmdSetViewport");
    vkCmdSetScissor = (PFN_vkCmdSetScissor) GetProcAddress(vulkan_library, "vkCmdSetScissor");
    vkCmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers) GetProcAddress(vulkan_library, "vkCmdBindVertexBuffers");
    vkCmdDraw = (PFN_vkCmdDraw) GetProcAddress(vulkan_library, "vkCmdDraw");
    vkCmdEndRenderPass = (PFN_vkCmdEndRenderPass) GetProcAddress(vulkan_library, "vkCmdEndRenderPass");
    vkDestroyFence = (PFN_vkDestroyFence) GetProcAddress(vulkan_library, "vkDestroyFence");
    vkCreateDescriptorSetLayout = (PFN_vkCreateDescriptorSetLayout) GetProcAddress(vulkan_library, "vkCreateDescriptorSetLayout");
    vkCreateDescriptorPool = (PFN_vkCreateDescriptorPool) GetProcAddress(vulkan_library, "vkCreateDescriptorPool");
    vkAllocateDescriptorSets = (PFN_vkAllocateDescriptorSets) GetProcAddress(vulkan_library, "vkAllocateDescriptorSets");
    vkUpdateDescriptorSets = (PFN_vkUpdateDescriptorSets) GetProcAddress(vulkan_library, "vkUpdateDescriptorSets");
    vkCmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets) GetProcAddress(vulkan_library, "vkCmdBindDescriptorSets");
    vkFlushMappedMemoryRanges = (PFN_vkFlushMappedMemoryRanges) GetProcAddress(vulkan_library, "vkFlushMappedMemoryRanges");
    vkCreateSampler = (PFN_vkCreateSampler) GetProcAddress(vulkan_library, "vkCreateSampler");


    vkDestroyInstance = (PFN_vkDestroyInstance) GetProcAddress(vulkan_library, "vkDestroyInstance");
    vkDestroyDevice = (PFN_vkDestroyDevice) GetProcAddress(vulkan_library, "vkDestroyDevice");


	return true;
}

bool VulkanCore::LoadVulkanExtensions(vulkan_context& context) {
	vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(context.instance, "vkCreateDebugReportCallbackEXT"));
	vkDebugReportMessageEXT = reinterpret_cast<PFN_vkDebugReportMessageEXT>(vkGetInstanceProcAddr(context.instance, "vkDebugReportMessageEXT"));
	vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(context.instance, "vkDestroyDebugReportCallbackEXT"));

	vkCreateWin32SurfaceKHR = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(vkGetInstanceProcAddr(context.instance, "vkCreateWin32SurfaceKHR"));
	vkGetPhysicalDeviceSurfaceSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(vkGetInstanceProcAddr(context.instance, "vkGetPhysicalDeviceSurfaceSupportKHR"));
	vkGetPhysicalDeviceSurfaceFormatsKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(vkGetInstanceProcAddr(context.instance, "vkGetPhysicalDeviceSurfaceFormatsKHR"));
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(vkGetInstanceProcAddr(context.instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
	vkGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(vkGetInstanceProcAddr(context.instance, "vkGetPhysicalDeviceSurfacePresentModesKHR"));

	return true;
}

bool VulkanCore::InitVulkan(WindowParameters& window) {
	// Validation layers
	uint32_t layerCount = 0;

#ifdef ENABLE_DEBUG_LAYERS
	m_validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> layersAvailable(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layersAvailable.data());

	for (const char* layerName : m_validationLayers) {
		bool foundValidation = false;

		for (const auto& layerProperties : layersAvailable) {
			if (strstr(layerProperties.layerName, layerName)) {
				std::cout << "Found validation layer \"" << layerName << "\"" << std::endl;
				foundValidation = true;
				break;
			}
		}

		if (!foundValidation) {
			std::cerr << __FUNCTION__ << " failed in " << __FILE__ << ":" << __LINE__ << "; " <<
					"Could not find validation layer named \"" << layerName << "\"!" << std::endl;
			return false;
		}
	}
#endif

	// Instance extensions
	uint32_t extensionCount = 0;

	m_extensions = {
			VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
			VK_KHR_SURFACE_EXTENSION_NAME,
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME
		};

	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensionsAvailable(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionsAvailable.data());

	for (const char* extensionName : m_extensions) {
		bool foundExtension = false;

		for (const auto& extensionProperties : extensionsAvailable) {
			if (strstr(extensionProperties.extensionName, extensionName)) {
				foundExtension = true;
				std::cout << "Found instance extension \"" << extensionName << "\"" << std::endl;
				break;
			}
		}

		if (!foundExtension) {
			std::cerr << __FUNCTION__ << " failed in " << __FILE__ << ":" << __LINE__ << "; " <<
					"Could not find instance extension named \"" << extensionName << "\"!" << std::endl;
			return false;
		}
	}

	// Create Vulkan instance
	VkApplicationInfo application_info = {};
	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.pNext = nullptr;
	application_info.pApplicationName = window.title;
	application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	application_info.pEngineName = "No Engine";
	application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	application_info.apiVersion = VK_MAKE_VERSION(1, 0, 5);

	VkInstanceCreateInfo instance_create_info = {};
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pNext = nullptr;
	instance_create_info.flags = 0;
	instance_create_info.pApplicationInfo = &application_info;
#ifdef ENABLE_DEBUG_LAYERS
	instance_create_info.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
	instance_create_info.ppEnabledLayerNames = m_validationLayers.data();
#else
	instance_create_info.enabledLayerCount = 0;
	instance_create_info.ppEnabledLayerNames = nullptr;
#endif
	instance_create_info.enabledExtensionCount = static_cast<uint32_t>(m_extensions.size());
	instance_create_info.ppEnabledExtensionNames = m_extensions.data();

	if ((vkCreateInstance(&instance_create_info, nullptr, &m_context.instance) != VK_SUCCESS) || (m_context.instance == nullptr)) {
		std::cerr << __FUNCTION__ << " failed in " << __FILE__ << ":" << __LINE__
			<< "; " << "Could not create Vulkan instance!" << std::endl;
		return false;
	}

	LoadVulkanExtensions(m_context);

	return true;
}

bool VulkanCore::CreateSurface(WindowParameters& window) {
	// Create Windows surface for rendering
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext = nullptr;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.hinstance = window.hInstance;
    surfaceCreateInfo.hwnd = window.hWnd;

    if (vkCreateWin32SurfaceKHR(m_context.instance, &surfaceCreateInfo, nullptr, &m_context.surface) != VK_SUCCESS) {
		std::cerr << __FUNCTION__ << " failed in " << __FILE__ << ":" << __LINE__
			<< "; " << "Could not create window surface!" << std::endl;
		return 0;
	}

	return true;
}

bool VulkanCore::PickPhysicalDevice() {
	// Find a physical device that supports rendering and has a graphics queue
	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(m_context.instance, &physicalDeviceCount, nullptr);
	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(m_context.instance, &physicalDeviceCount, physicalDevices.data());

	for (const auto& physicalDevice : physicalDevices) {
	    VkPhysicalDeviceProperties deviceProperties = {};
	    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

	    uint32_t queueFamilyCount = 0;
	    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
	    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
                                                 &queueFamilyCount,
                                                 queueFamilyProperties.data());

        for(uint32_t i = 0; i < queueFamilyCount; ++i) {
            VkBool32 supportsPresent;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_context.surface, &supportsPresent);

            if(supportsPresent && (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                m_context.physicalDevice = physicalDevice;
                m_context.physicalDeviceProperties = deviceProperties;
                m_context.presentQueueIdx = i;
                vkGetPhysicalDeviceFeatures(physicalDevice, &m_context.physicalDeviceFeatures);
                break;
            }
        }

        if(m_context.physicalDevice) {
        	std::cout << "Physical device name: " << m_context.physicalDeviceProperties.deviceName << std::endl;
        	std::cout << "Physical DeviceID: 0x" << std::hex << std::uppercase << m_context.physicalDeviceProperties.deviceID << std::dec << std::nouppercase << std::endl;
        	std::cout << "Vulkan API Version: " << VK_VERSION_MAJOR(m_context.physicalDeviceProperties.apiVersion) << "."
												<< VK_VERSION_MINOR(m_context.physicalDeviceProperties.apiVersion) << "."
												<< VK_VERSION_PATCH(m_context.physicalDeviceProperties.apiVersion) << std::endl;
    		break;
        }
	}

	if (m_context.physicalDevice == VK_NULL_HANDLE) {
		std::cerr << __FUNCTION__ << " failed in " << __FILE__ << ":" << __LINE__
			<< "; " << "No physical device present that can render and present!" << std::endl;
		return false;
	}

	// Get the physical device memory properties
	vkGetPhysicalDeviceMemoryProperties(m_context.physicalDevice, &m_context.memoryProperties);

	return true;
}

bool VulkanCore::CreateLogicalDevice() {
	// Create logical device (connection to physical device)
	std::vector<float> queuePriority = { 1.0f };

	m_context.physicalDeviceFeatures.shaderClipDistance = VK_TRUE;

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext = nullptr;
    queueCreateInfo.flags = 0;
    queueCreateInfo.queueFamilyIndex = m_context.presentQueueIdx;
    queueCreateInfo.queueCount = static_cast<uint32_t>(queuePriority.size());
    queueCreateInfo.pQueuePriorities = queuePriority.data();

	std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

    VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = nullptr;
    deviceInfo.flags = 0;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
    deviceInfo.ppEnabledLayerNames = m_validationLayers.data();
    deviceInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceInfo.pEnabledFeatures = &m_context.physicalDeviceFeatures;

    if (vkCreateDevice(m_context.physicalDevice, &deviceInfo, nullptr, &m_context.device) != VK_SUCCESS) {
		std::cerr << __FUNCTION__ << " failed in " << __FILE__ << ":" << __LINE__
			<< "; " << "Could not create Vulkan device!" << std::endl;
		return false;
	}

	return true;
}

bool VulkanCore::SetupDebugCallback() {
	VkDebugReportCallbackCreateInfoEXT callbackCreateInfo = {};
	callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	callbackCreateInfo.pNext = nullptr;
	callbackCreateInfo.flags =  VK_DEBUG_REPORT_ERROR_BIT_EXT |
	                            VK_DEBUG_REPORT_WARNING_BIT_EXT |
	                            VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
	callbackCreateInfo.pfnCallback = &MyDebugReportCallback;
	callbackCreateInfo.pUserData = nullptr;

	if (vkCreateDebugReportCallbackEXT(m_context.instance, &callbackCreateInfo, nullptr, &m_context.callback) != VK_SUCCESS) {
		std::cerr << __FUNCTION__ << " failed in " << __FILE__ << ":" << __LINE__
			<< "; " << "Could not create debug callback!" << std::endl;
		return false;
	}

	return true;
}

void VulkanCore::Cleanup() {
    if (m_context.device) {
    	vkDestroyDevice(m_context.device, nullptr);
    }

	vkDestroyDebugReportCallbackEXT(m_context.instance, m_context.callback, nullptr);

    if (m_context.instance) {
    	vkDestroyInstance(m_context.instance, nullptr);
    }
}

} /* namespace mws */

