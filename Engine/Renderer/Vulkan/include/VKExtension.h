#pragma once
#include "VKRenderer.h"


extern VkShaderStageFlagBits vkGetShaderStageFlagBits(shader_kind kind);

extern VkResult vkBeginCommandBufferPrimary(VkCommandBuffer vkCommandBuffer, VkCommandBufferUsageFlags flags);
extern VkResult vkBeginCommandBufferSecondary(VkCommandBuffer vkCommandBuffer, VkCommandBufferUsageFlags flags, VkFramebuffer vkFrameBuffer, VkRenderPass vkRenderPass, uint32_t indexSubpass, VkBool32 occlusionQueryEnable, VkQueryControlFlags queryFlags, VkQueryPipelineStatisticFlags pipelineStatistics);

extern VkResult vkCmdBindVertexBuffer(VkCommandBuffer vkCommandBuffer, uint32_t binding, VkBuffer vkBuffer, VkDeviceSize offset);

extern VkResult vkCmdBufferMemoryBarrier(VkCommandBuffer vkCommandBuffer, VkBuffer vkBuffer, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags, VkDeviceSize offset, VkDeviceSize size);
extern VkResult vkCmdBufferMemoryBarrierRead(VkCommandBuffer vkCommandBuffer, VkBuffer vkBuffer, VkPipelineStageFlags dstPipelineStageFlags, VkDeviceSize offset, VkDeviceSize size);
extern VkResult vkCmdBufferMemoryBarrierWrite(VkCommandBuffer vkCommandBuffer, VkBuffer vkBuffer, VkPipelineStageFlags dstPipelineStageFlags, VkDeviceSize offset, VkDeviceSize size);

extern VkResult vkCmdImageMemoryBarrier(VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageLayout srcLayout, VkImageLayout dstLayout, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags, VkImageSubresourceRange range);
extern VkResult vkCmdImageMemoryBarrier(VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageLayout srcLayout, VkImageLayout dstLayout, VkImageSubresourceRange range);

extern VkResult vkSubmitCommandBuffer(VkQueue vkQueue, VkCommandBuffer vkCommandBuffer, VkFence vkFence);
extern VkResult vkSubmitCommandBuffer(VkQueue vkQueue, VkCommandBuffer vkCommandBuffer, VkFence vkFence, VkSemaphore vkWaitSemaphore, VkPipelineStageFlags waitStageFlags, VkSemaphore vkSignalSemaphore);
