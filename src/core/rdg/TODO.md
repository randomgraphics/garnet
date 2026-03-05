# RDG TODO

## vk-buffer

Should not create host-visible and mappable buffer, since it slows down GPU read performance.
Instead, create a GPU-visible-only buffer, and use the `setContent` function in rapid-vulkan to set buffer content.
