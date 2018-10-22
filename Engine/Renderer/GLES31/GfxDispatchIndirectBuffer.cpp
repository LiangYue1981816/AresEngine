#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxDispatchIndirectBuffer.h"


CGfxDispatchIndirectBuffer::CGfxDispatchIndirectBuffer(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ)
	: m_buffer(0)
{
	typedef struct {
		uint32_t numGroupsX;
		uint32_t numGroupsY;
		uint32_t numGroupsZ;
	} DispatchIndirectCommand;

	DispatchIndirectCommand command;
	command.numGroupsX = numGroupsX;
	command.numGroupsY = numGroupsY;
	command.numGroupsZ = numGroupsZ;

	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, m_buffer);
	glBufferData(GL_DISPATCH_INDIRECT_BUFFER, sizeof(command), &command, GL_STATIC_DRAW);
	glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, 0);
}

CGfxDispatchIndirectBuffer::~CGfxDispatchIndirectBuffer(void)
{
	glDeleteBuffers(1, &m_buffer);
}

void CGfxDispatchIndirectBuffer::Release(void)
{
	Renderer()->DestroyDispatchIndirectBuffer(this);
}

void CGfxDispatchIndirectBuffer::Bind(void) const
{
	glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, m_buffer);
}
