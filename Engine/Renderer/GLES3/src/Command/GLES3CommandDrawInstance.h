#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDrawInstance : public CGfxCommandBase
{
public:
	CGLES3CommandDrawInstance(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size)
		: m_ptrMeshDraw(ptrMeshDraw)
	{
		m_buffer.assign(pInstanceBuffer, pInstanceBuffer + size);
	}
	virtual ~CGLES3CommandDrawInstance(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INSTANCE, "CommandDrawInstance");
		{
			if (m_ptrMeshDraw.IsValid()) {
				((CGLES3MeshDraw *)m_ptrMeshDraw.GetPointer())->InstanceBufferData(m_buffer.size(), m_buffer.data());
				((CGLES3MeshDraw *)m_ptrMeshDraw.GetPointer())->Bind();

				glDrawElementsInstanced(
					GL_TRIANGLES, 
					m_ptrMeshDraw->GetIndexCount(), 
					CGLES3Helper::TranslateIndexType(m_ptrMeshDraw->GetIndexType()), 
					(const void *)m_ptrMeshDraw->GetIndexOffset(), 
					m_ptrMeshDraw->GetInstanceCount());
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
	eastl::vector<uint8_t> m_buffer;
};
