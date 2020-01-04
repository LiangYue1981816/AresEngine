#pragma once


#if defined (_DEBUG) || defined (DEBUG)
#  define CHECK_GL_ERROR_ASSERT()                \
   {                                             \
      GLenum err = glGetError();                 \
      ASSERT(err == GL_NO_ERROR);                \
   }
#else
#  define CHECK_GL_ERROR_ASSERT()
#endif


class CGLES3Renderer;

class CGLES3SwapChain;
class CGLES3RenderPass;
class CGLES3RenderPassManager;
class CGLES3RenderTexture;
class CGLES3RenderTextureManager;
class CGLES3FrameBuffer;
class CGLES3FrameBufferManager;

class CGLES3Shader;
class CGLES3ShaderManager;
class CGLES3Pipeline;
class CGLES3PipelineCompute;
class CGLES3PipelineComputeManager;
class CGLES3PipelineGraphics;
class CGLES3PipelineGraphicsManager;

class CGLES3Buffer;

class CGLES3IndexBuffer;
class CGLES3VertexBuffer;
class CGLES3InstanceBuffer;
class CGLES3IndirectBuffer;

class CGLES3MultiIndexBuffer;
class CGLES3MultiVertexBuffer;
class CGLES3MultiInstanceBuffer;
class CGLES3MultiIndirectBuffer;

class CGLES3VertexArrayObject;
class CGLES3MultiVertexArrayObject;

class CGLES3UniformBuffer;
class CGLES3UniformBufferManager;
class CGLES3StorageBuffer;
class CGLES3StorageBufferManager;

class CGLES3Sampler;
class CGLES3SamplerManager;
class CGLES3Texture;
class CGLES3Texture2D;
class CGLES3Texture2DManager;
class CGLES3Texture2DArray;
class CGLES3Texture2DArrayManager;
class CGLES3TextureCubemap;
class CGLES3TextureCubemapManager;

class CGLES3Mesh;
class CGLES3MeshManager;
class CGLES3MeshDraw;
class CGLES3MeshDrawManager;

class CGLES3Material;
class CGLES3MaterialPass;
class CGLES3MaterialManager;

class CGLES3DescriptorSet;
class CGLES3DescriptorSetManager;
class CGLES3DescriptorLayout;
class CGLES3DescriptorLayoutManager;

class CGLES3Semaphore;
class CGLES3CommandBuffer;
class CGLES3CommandBufferManager;
