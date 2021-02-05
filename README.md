# Vulkan-Portability

This repository provides a focus and tools to help application developers 
understand and provide input to the *Vulkan Portability initiative*, 
including the [VK_KHR_portability_subset](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VK_KHR_portability_subset.html)
Vulkan extension.


## VK_KHR_portability_subset Extension

The `VK_KHR_portability_subset` extension is now included in the [Vulkan specification](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/chap48.html#VK_KHR_portability_subset).

This extension allows a non-conformant Vulkan implementation to be built on top of another 
non-Vulkan graphics API, and identifies differences between that implementation and a 
fully-conformant native Vulkan implementation.

This extension provides Vulkan implementations with the ability to mark otherwise-required 
capabilities as unsupported, or to establish additional properties and limits that the 
application should adhere to in order to guarantee portable behaviour and operation across 
platforms, including platforms where Vulkan is not natively supported.

The goal of this specification is to document, and make queryable, capabilities which are 
required to be supported by a fully-conformant Vulkan 1.0 implementation, but may be optional 
for an implementation of the Vulkan 1.0 Portability Subset.

The intent is that this extension will be advertised only on implementations of the Vulkan 1.0 
Portability Subset, and not on conformant implementations of Vulkan 1.0. Fully-conformant Vulkan 
implementations provide all the required capabilies, and so will not provide this extension. 
Therefore, the existence of this extension can be used to determine that an implementation is 
likely not fully conformant with the Vulkan spec.

**If this extension is supported by the Vulkan implementation, the application must enable this extension.**

This extension defines several new structures that can be chained to the existing structures 
used by certain standard Vulkan calls, in order to query for non-conformant portable behavior.


## Provide Your Feedback!

The `VK_KHR_portability_subset` extension is **_provisional_**, which means it is being
released to the developer community with the intention of collecting feedback for the final
version. Help us evolve this provisional extension specification by providing feedback and 
suggestions, or submitting any issues you encounterto the Vulkan Portability 
[issues list](https://github.com/KhronosGroup/Vulkan-Portability/issues) in this repository.






