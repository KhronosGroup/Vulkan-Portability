# Vulkan-Portability

This repository contains the spec description and header files for the Vulkan Portability extension:
**VK_EXTX_portability_subset**.

>**_Please Note_**:  This extension is currently defined as "EXTX", meaning "multivendor experimental". 
That means the definition of this extension is in active development, and may break compatibility
between point releases (defined as any increment of `VK_EXTX_PORTABILITY_SUBSET_SPEC_VERSION`).
You are free to explore the extension and provide feedback, but it is not recommended to use this
extension for shipping applications, particularly applications that require the driver implementing this
extension to be linked dynamically and potentially "dropped-in" to the application execution environment.

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

---------------------------

The `src/sample.cpp` file contains sample code for probing Vulkan portability extension behaviour.
Code snippets from this file can be copied to applications that need to query the portability
behaviour of a Vulkan implementation that implements the Vulkan Portability extension.
