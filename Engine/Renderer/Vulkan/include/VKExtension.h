#pragma once
#include "VKRenderer.h"


extern VkShaderStageFlagBits vkGetShaderStageFlagBits(shader_kind kind);

extern VkResult vkBeginCommandBufferPrimary(VkCommandBuffer vkCommandBuffer, VkCommandBufferUsageFlags flags);
extern VkResult vkBeginCommandBufferSecondary(VkCommandBuffer vkCommandBuffer, VkCommandBufferUsageFlags flags, VkFramebuffer vkFrameBuffer, VkRenderPass vkRenderPass, uint32_t indexSubpass, VkBool32 occlusionQueryEnable, VkQueryControlFlags queryFlags, VkQueryPipelineStatisticFlags pipelineStatistics);

extern VkResult vkCmdBindVertexBuffer(VkCommandBuffer vkCommandBuffer, uint32_t binding, VkBuffer vkBuffer, VkDeviceSize offset);

extern VkResult vkCmdBufferMemoryBarrier(VkCommandBuffer vkCommandBuffer, VkBuffer vkBuffer, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags, VkDeviceSize offset, VkDeviceSize size);
extern VkResult vkCmdBufferMemoryBarrier(VkCommandBuffer vkCommandBuffer, VkBuffer vkBuffer, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkDeviceSize offset, VkDeviceSize size);

extern VkResult vkCmdImageMemoryBarrier(VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageLayout srcLayout, VkImageLayout dstLayout, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags, VkImageSubresourceRange range);
extern VkResult vkCmdImageMemoryBarrier(VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageLayout srcLayout, VkImageLayout dstLayout, VkImageSubresourceRange range);

extern VkResult vkCmdBufferData(VkCommandBuffer vkCommandBuffer, VkBuffer vkSrcBuffer, VkDeviceSize srcOffset, VkBuffer vkDstBuffer, VkDeviceSize dstOffset, VkDeviceSize size);
extern VkResult vkCmdTextureData(VkCommandBuffer vkCommandBuffer, VkBuffer vkSrcBuffer, VkDeviceSize bufferOffset, VkImage vkDstImage, VkImageAspectFlags vkAspectMask, int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height, uint32_t level, uint32_t layer);

extern VkResult vkSubmitCommandBuffer(VkQueue vkQueue, VkCommandBuffer vkCommandBuffer, VkFence vkFence);
extern VkResult vkSubmitCommandBuffer(VkQueue vkQueue, VkCommandBuffer vkCommandBuffer, VkFence vkFence, VkSemaphore vkWaitSemaphore, VkPipelineStageFlags waitStageFlags, VkSemaphore vkSignalSemaphore);
