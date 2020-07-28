#pragma once


#if defined (_DEBUG) || defined (DEBUG)
#  define CHECK_GL_ERROR_ASSERT()                \
   {                                             \
      GLenum err = glGetError();                 \
      if (err != GL_NO_ERROR) {                  \
         LogOutput(LOG_TAG_ERROR, "\nglGetError=0x%x\n", err); \
         ASSERT(err == GL_NO_ERROR);             \
	  }                                          \
   }
#else
#  define CHECK_GL_ERROR_ASSERT()
#endif


class CGLES3Renderer;

class CGLES3SwapChain;
class CGLES3RenderPass;
class CGLES3RenderTexture;
class CGLES3FrameBuffer;

class CGLES3Shader;
class CGLES3Pipeline;
class CGLES3PipelineCompute;
class CGLES3PipelineGraphics;

class CGLES3Buffer;

class CGLES3IndexBuffer;
class CGLES3VertexBuffer;
class CGLES3InstanceBuffer;
class CGLES3IndirectBuffer;

class CGLES3MultiIndexBuffer;
class CGLES3MultiVertexBuffer;
class CGLES3MultiInstanceBuffer;
class CGLES3MultiIndirectBuffer;

class CGLES3UniformBuffer;
class CGLES3StorageBuffer;

class CGLES3Sampler;
class CGLES3Texture;
class CGLES3Texture2D;
class CGLES3Texture2DArray;
class CGLES3TextureCubemap;

class CGLES3Mesh;
class CGLES3MeshDraw;

class CGLES3Material;
class CGLES3MaterialPass;

class CGLES3DescriptorSet;
class CGLES3DescriptorLayout;

class CGLES3Semaphore;
class CGLES3CommandBuffer;
