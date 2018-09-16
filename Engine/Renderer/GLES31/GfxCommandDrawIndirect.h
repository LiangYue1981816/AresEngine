#pragma once
#include <gl31.h>
#include "GfxCommandBuffer.h"


class CGfxCommandDrawIndirect : public CGfxCommandBase
{
public:
	CGfxCommandDrawIndirect(GLenum mode, GLenum type, GLsizei count, GLsizei baseIndex, GLsizei baseVertex, GLsizei instanceCount)
		: m_mode(mode)
		, m_type(type)
		, m_count(count)
		, m_baseIndex(baseIndex)
		, m_baseVertex(baseVertex)
		, m_instanceCount(instanceCount)
	{

	}
	virtual ~CGfxCommandDrawIndirect(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		struct {
			uint32_t count;
			uint32_t instanceCount;
			uint32_t baseIndex;
			uint32_t baseVertex;
			uint32_t reserved;
		} indirect;

		indirect.count = m_count;
		indirect.baseIndex = m_baseIndex;
		indirect.baseVertex = m_baseVertex;
		indirect.instanceCount = m_instanceCount;
		indirect.reserved = 0;

		glDrawElementsIndirect(m_mode, m_type, (const void *)&indirect);
	}


private:
	GLenum m_mode;
	GLenum m_type;
	GLsizei m_count;
	GLsizei m_baseIndex;
	GLsizei m_baseVertex;
	GLsizei m_instanceCount;
};
