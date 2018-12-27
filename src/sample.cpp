
#include <string>
#include "vulkan/vk_extx_portability_subset.h"

#if defined(MOCKDRIVER)

// A "mock driver" example implementation ////////////////////////////////////////////////////////////////////////////////////////

static void mock_vkGetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures) {
    for (auto data = reinterpret_cast<VkBaseOutStructure*>(pFeatures); data; data = data->pNext) {
        switch ((int)data->sType) {
            case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_EXTX: {
                auto port_features = reinterpret_cast<VkPhysicalDevicePortabilitySubsetFeaturesEXTX*>(data);
                // The implementation sets its results...
                port_features->triangleFans = VK_FALSE;
                port_features->separateStencilMaskRef = VK_FALSE;
                port_features->events = VK_FALSE;
                port_features->standardImageViews = VK_FALSE;
                port_features->samplerMipLodBias = VK_FALSE;
                break;
            }
            default:
                break;
        }
    }
}

static void mock_vkGetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties) {
    for (auto data = reinterpret_cast<VkBaseOutStructure*>(pProperties); data; data = data->pNext) {
        switch ((int)data->sType) {
            case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_PROPERTIES_EXTX: {
                auto port_properties = reinterpret_cast<VkPhysicalDevicePortabilitySubsetPropertiesEXTX*>(data);
                // The implementation sets its results...
                port_properties->minVertexInputBindingStrideAlignment = 4;
                break;
            }
            default:
                break;
        }
    }
}

static VkResult mock_vkGetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice,
                                                                  const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo,
                                                                  VkImageFormatProperties2* pImageFormatProperties) {
    VkResult result = VK_SUCCESS;
    for (auto data = reinterpret_cast<VkBaseOutStructure*>(pImageFormatProperties); data; data = data->pNext) {
        switch ((int)data->sType) {
            case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_VIEW_SUPPORT_EXTX: {
                // The implementation sets its results...
                result = VK_ERROR_FORMAT_NOT_SUPPORTED;
                break;
            }
            default:
                break;
        }
    }
    return result;
}

PFN_vkVoidFunction vkGetInstanceProcAddr(VkInstance instance, const char* pName) {
    const std::string name(pName);
    if (name.compare("vkGetPhysicalDeviceFeatures2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(mock_vkGetPhysicalDeviceFeatures2KHR);
    if (name.compare("vkGetPhysicalDeviceProperties2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(mock_vkGetPhysicalDeviceProperties2KHR);
    if (name.compare("vkGetPhysicalDeviceImageFormatProperties2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(mock_vkGetPhysicalDeviceImageFormatProperties2KHR);
    return NULL;
}

#endif  // defined(MOCKDRIVER)

// Examples of application query functions ///////////////////////////////////////////////////////////////////////////////////////

static void testVkPhysicalDevicePortabilitySubsetFeaturesEXTX(VkInstance instance, VkPhysicalDevice physical_device) {
    VkPhysicalDevicePortabilitySubsetFeaturesEXTX port_features = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_EXTX, .pNext = NULL};

    VkPhysicalDeviceFeatures2 features2 = {.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, .pNext = &port_features};

    auto pfn_vkGetPhysicalDeviceFeatures2KHR =
        (PFN_vkGetPhysicalDeviceFeatures2KHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFeatures2KHR");
    pfn_vkGetPhysicalDeviceFeatures2KHR(physical_device, &features2);

    // Use results from port_features.
}

static void testVkPhysicalDevicePortabilitySubsetPropertiesEXTX(VkInstance instance, VkPhysicalDevice physical_device) {
    VkPhysicalDevicePortabilitySubsetPropertiesEXTX port_properties = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_PROPERTIES_EXTX, .pNext = NULL};

    VkPhysicalDeviceProperties2 properties2 = {.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, .pNext = &port_properties};

    auto pfn_vkGetPhysicalDeviceProperties2KHR =
        (PFN_vkGetPhysicalDeviceProperties2KHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties2KHR");
    pfn_vkGetPhysicalDeviceProperties2KHR(physical_device, &properties2);

    // Use results from port_properties.
}

static VkResult testVkPhysicalDeviceImageViewSupportEXTX(VkInstance instance, VkPhysicalDevice physical_device) {
    VkImageFormatProperties2 format_properties2 = {.sType = VK_STRUCTURE_TYPE_IMAGE_FORMAT_PROPERTIES_2, .pNext = NULL};

    VkPhysicalDeviceImageViewSupportEXTX port_image_view_support = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_VIEW_SUPPORT_EXTX,
        .pNext = NULL,
        .flags = 0,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_UNORM,
        .components =
            {
                .r = VK_COMPONENT_SWIZZLE_R,
                .g = VK_COMPONENT_SWIZZLE_A,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY,
            },
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT};

    VkPhysicalDeviceImageFormatInfo2KHR format_info2 = {.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_FORMAT_INFO_2,
                                                        .pNext = &port_image_view_support,
                                                        .format = VK_FORMAT_R8G8B8A8_UNORM,
                                                        .type = VK_IMAGE_TYPE_2D,
                                                        .tiling = VK_IMAGE_TILING_OPTIMAL,
                                                        .usage = VK_IMAGE_USAGE_SAMPLED_BIT,
                                                        .flags = 0};

    auto pfn_vkGetPhysicalDeviceImageFormatProperties2KHR = (PFN_vkGetPhysicalDeviceImageFormatProperties2KHR)vkGetInstanceProcAddr(
        instance, "vkGetPhysicalDeviceImageFormatProperties2KHR");
    VkResult result = pfn_vkGetPhysicalDeviceImageFormatProperties2KHR(physical_device, &format_info2, &format_properties2);
    return result;
}

// Examples of application query functions ///////////////////////////////////////////////////////////////////////////////////////

int main(void) {
#if defined(MOCKDRIVER)
    // The mock driver doesn't need real handles.
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    bool isPortabilitySupported = true;
#else
    // Your real driver initialization goes here.
    VkInstance instance = TODO;
    VkPhysicalDevice physical_device = TODO;

    // isPortabilitySupported is true if vkEnumerateDeviceExtensionProperties() returns a VkExtensionProperties entry for
    // VK_EXTX_PORTABILITY_SUBSET_EXTENSION_NAME
    bool isPortabilitySupported = TODO;
#endif  // defined(MOCKDRIVER)

    if (isPortabilitySupported) {
        testVkPhysicalDevicePortabilitySubsetFeaturesEXTX(instance, physical_device);
        testVkPhysicalDevicePortabilitySubsetPropertiesEXTX(instance, physical_device);
        testVkPhysicalDeviceImageViewSupportEXTX(instance, physical_device);
    }

    return 0;
}
