# VK_EXTX_portability_subset

Please note:  This extension is currently defined as "EXTX", meaning "multivendor experimental".
That means the definition of this extension is in active development, and may break compatibility
between point releases.
You are free to explore the extension and provide feedback,
but it is not recommended to use this for shipping applications.

This specification provides Vulkan implementations with the ability to mark otherwise-required
capabilities as unsupported, or to establish additional properties and limits that the app should
adhere to in order to guarantee portable behaviour and operation across platforms.

This extension allows the vast majority of Vulkan behaviour to be implemented on top of APIs and
platforms that are missing just a few pieces of required Vulkan capability.

The goal of this specification is to document, and make queryable, capabilities which are required
to be supported by a fully-conformant Vulkan 1.0 implementation, but may be optional for a "Portable"
Vulkan 1.0 implementation.

This extension defines several new structures that can be chained to the existing structures used by
certain standard Vulkan calls, in order to query for non-conformant portable behaviour.



# Extension and Version Dependencies

- Requires Vulkan 1.0
- Requires `VK_KHR_get_physical_device_properties2`


# New Structures


## VkPhysicalDevicePortabilitySubsetFeaturesEXTX

To query for features that may not be supported by a portable Vulkan implementation,
include a `VkPhysicalDevicePortabilitySubsetFeaturesEXTX` structure in the `pNext` chain
of a call to `vkGetPhysicalDeviceFeatures2KHR`. The implementation will populate the
`VkPhysicalDevicePortabilitySubsetFeaturesEXTX` structure to indicate which Vulkan
features are supported.

        typedef struct VkPhysicalDevicePortabilitySubsetFeaturesEXTX {
             VkStructureType    sType;
             void*              pNext;
             VkBool32           triangleFans;
             VkBool32           separateStencilMaskRef;
             VkBool32           events;
             VkBool32           standardImageViews;
             VkBool32           samplerMipLodBias;
        } VkPhysicalDevicePortabilitySubsetFeaturesEXTX;

- `sType` is the type of this structure.
- `pNext` is `NULL` or a pointer to an extension-specific structure.
- `triangleFans` indicates whether this implementation supports rendering triangle fans in draw calls.
- `separateStencilMaskRef` indicates whether this implementation supports separate stencil reference
  mask values in the `VkStencilOpState` structure.
- `events` indicates whether this implementation supports `VkEvent` functionality.
- `standardImageViews` indicates whether this implementation supports all standard Vulkan configuration
  behaviour for `VkImageViews`. If this value is `VK_FALSE`, the application can query for non-standard
  `VkImageViews` behaviour using the `VkPhysicalDeviceImageViewSupportEXTX` structure as described below.
- `samplerMipLodBias` indicates whether the implementation will make use of the value populated in the
  `mipLodBias` member of the `VkSamplerCreateInfo` structure when creating a sampler.


### Valid Usage
- `sType` must be `VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_EXTX`

- Each `pNext` member of any structure (including this one) in the `pNext` chain **must** be either
  `NULL` or a pointer to another structure permitted in the `vkGetPhysicalDeviceFeatures2KHR` call.

- If the implementation sets `triangleFans` to `VK_FALSE`, the implementation does not support rendering triangle fans,
  and `VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN` **must** not be used.

- If the implementation sets `separateStencilMaskRef` to `VK_FALSE`, the implementation does not support separate
  reference values for front and back stencil comparisons, and the `reference` members of
  the `VkStencilOpState` structures in both the `front` and `back` members of the
  `VkPipelineDepthStencilStateCreateInfo` struct in the `pDepthStencilState` member of the
  `VkGraphicsPipelineCreateInfo` structure passed as `pCreateInfo` in `vkCreateGraphicsPipeline`
  **must** be set to the same stencil reference value.

- If the implementation sets `events` to `VK_FALSE`, the implementation does not support Vulkan event
  functionality, and calls to the following functions **must** not be made.

            vkCreateEvent
            vkDestroyEvent
            vkGetEventStatus
            vkSetEvent
            vkResetEvent
            vkCmdSetEvent
            vkCmdResetEvent
            vkCmdWaitEvents

  If `events` is `VK_FALSE`, and calls are made to these functions, the implementation **must**
  return a `VK_ERROR_FEATURE_NOT_PRESENT` error if applicable, or ignore the call if not.

- If the implementation sets `standardImageViews` to `VK_FALSE`, the user is expected to query
  the capability of the portable implementation to create image views with particular properties by
  chaining a `VkPhysicalDeviceImageViewSupportEXTX` structure to the `VkPhysicalDeviceImageFormatInfo2KHR`
  structure in a `vkGetPhysicalDeviceImageFormatProperties2` query, which will return a value of
  `VK_ERROR_FORMAT_NOT_SUPPORTED` if the implementation is unable to create a `VkImageView` with
  those properties.

- If the implementation sets `samplerMipLodBias` to `VK_FALSE`, the `mipLodBias` member of the
  `VkSamplerCreateInfo` structure passed as `pCreateInfo` in `vkCreateSampler` will be ignored
  by the implementation.



## VkPhysicalDevicePortabilitySubsetPropertiesEXTX

To query for additional properties and limits that may be defined by a portable Vulkan implementation,
include a `VkPhysicalDevicePortabilitySubsetPropertiesEXTX` structure in the `pNext` chain of a call to
`vkGetPhysicalDeviceProperties2KHR`. The implementation will populate the
`VkPhysicalDevicePortabilitySubsetPropertiesEXTX` structure to indicate which Vulkan features are supported.

        typedef struct VkPhysicalDevicePortabilitySubsetPropertiesEXTX {
             VkStructureType    sType;
             void*              pNext;
             uint32_t           minVertexInputBindingStrideAlignment;
        } VkPhysicalDevicePortabilitySubsetPropertiesEXTX;

- `sType` is the type of this structure.
- `pNext` is `NULL` or a pointer to an extension-specific structure.
- `minVertexInputBindingStrideAlignment` indicates the minimum alignment for the vertex input strides.
  This is both the minimum value and the required multiple of a stride in vertex input binding.


### Valid Usage
- `sType` must be `VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_PROPERTIES_EXTX `

- Each `pNext` member of any structure (including this one) in the `pNext` chain **must** be either
  `NULL` or a pointer to another structure permitted in the `vkGetPhysicalDeviceProperties2KHR` call.

- If the implementation sets `minVertexInputBindingStrideAlignment` to a non-zero value, the `stride`
  member of the `VkVertexInputBindingDescription` structure in each element of the `pVertexBindingDescriptions`
  array member of the `VkPipelineVertexInputStateCreateInfo` structure in the `pVertexInputState` member
  of the `VkGraphicsPipelineCreateInfo` structure passed as `pCreateInfos` in `vkCreateGraphicsPipeline`
  **must** be set to a non-zero multiple of `minVertexInputBindingStrideAlignment`.


## VkPhysicalDeviceImageViewSupportEXTX

To query whether a `VkImageView` can be created for a specific set of properties,
include a `VkPhysicalDeviceImageViewSupportEXTX` structure in the `pNext` chain of
a call to `vkGetPhysicalDeviceImageFormatProperties2KHR`. The implementation **must**
return `VK_ERROR_FORMAT_NOT_SUPPORTED` if the creation of a `VkImageView` with those
properties is not supported.

        typedef struct VkPhysicalDeviceImageViewSupportEXTX {
             VkStructureType        sType;
             void*                  pNext;
             VkImageViewCreateFlags flags;
             VkImageViewType        viewType;
             VkFormat               format;
             VkComponentMapping     components;
             VkImageAspectFlags     aspectMask;
        } VkPhysicalDeviceImageViewSupportEXTX;

- `sType` is the type of this structure.
- `pNext` is `NULL` or a pointer to an extension-specific structure.
- `flags` Corresponds to the same member of `VkImageViewCreateInfo`.
- `viewType` Corresponds to the same member of `VkImageViewCreateInfo`.
- `format` Corresponds to the same member of `VkImageViewCreateInfo`.
- `components` Corresponds to the same member of `VkImageViewCreateInfo`.
- `aspectMask` Corresponds to the same member of the `VkImageSubresourceRange`
  structure in the `subresourceRange` member of `VkImageViewCreateInfo`.


### Valid Usage

- `sType` must be `VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_VIEW_SUPPORT_EXTX `

- Each `pNext` member of any structure (including this one) in the `pNext` chain **must** be either `NULL`
  or a pointer to another structure permitted in the `vkGetPhysicalDeviceImageFormatProperties2KHR` call.

- `flags` is reserved for future use and **must** be `0`.

- `viewType` **must** be a valid `VkImageViewType` value.

- `format` **must** be a valid `VkFormat` value.

- `components` **must** be a valid `VkComponentMapping` structure.

- `aspectMask` **must** be a valid combination of `VkImageAspectFlagBits` values, and must not be `0`.

- If `VK_ERROR_FORMAT_NOT_SUPPORTED` is returned from a `vkGetPhysicalDeviceImageFormatProperties2`
  query with a particular set of properties set in the chained `VkPhysicalDeviceImageViewSupportEXTX`
  structure, a subsequent call to `vkCreateImageView` **must** not include those same properties,
  which will return an error code of `VK_ERROR_FORMAT_NOT_SUPPORTED` in that case.
