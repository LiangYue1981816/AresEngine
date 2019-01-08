#pragma once
#include "VKRenderer.h"


extern VkShaderStageFlagBits vkGetShaderStageFlagBits(shader_kind kind);

extern VkResult vkBeginCommandBufferPrimary(VkCommandBuffer vkCommandBuffer, VkCommandBufferUsageFlags flags);
extern VkResult vkBeginCommandBufferSecondary(VkCommandBuffer vkCommandBuffer, VkCommandBufferUsageFlags flags, VkFramebuffer vkFrameBuffer, VkRenderPass vkRenderPass, uint32_t indexSubPass, VkBool32 occlusionQueryEnable, VkQueryControlFlags queryFlags, VkQueryPipelineStatisticFlags pipelineStatistics);

extern void vkCmdSetImageLayout(VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageSubresourceRange range);

extern void vkCmdTransferImage(VkCommandBuffer vkCommandBuffer, VkImage vkSrcImage, VkImage vkDstImage, VkImageCopy region);
extern void vkCmdTransferImage(VkCommandBuffer vkCommandBuffer, VkBuffer vkSrcBuffer, VkImage vkDstImage, uint32_t levels, uint32_t layers, const VkBufferImageCopy *regions, uint32_t count);
extern void vkCmdTransferBuffer(VkCommandBuffer vkCommandBuffer, VkBuffer vkSrcBuffer, VkBuffer vkDstBuffer, VkAccessFlags dstAccessMask, VkPipelineStageFlags dstStageMask, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size);

extern void vkCmdBindVertexBuffer(VkCommandBuffer vkCommandBuffer, uint32_t firstBinding, VkBuffer vkBuffer, VkDeviceSize offset);

extern VkResult vkSubmitCommandBuffer(VkQueue vkQueue, VkCommandBuffer vkCommandBuffer, VkFence vkFence);
extern VkResult vkSubmitCommandBuffer(VkQueue vkQueue, VkCommandBuffer vkCommandBuffer, VkFence vkFence, VkSemaphore vkWaitSemaphore, VkPipelineStageFlags waitStageFlags, VkSemaphore vkSignalSemaphore);
