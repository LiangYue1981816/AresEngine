#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxProgramManager.h"


CGfxProgramManager::CGfxProgramManager(void)
{

}

CGfxProgramManager::~CGfxProgramManager(void)
{
	for (const auto &itProgram : m_pPrograms) {
		delete itProgram.second;
	}

	m_pPrograms.clear();
}

CGfxProgram* CGfxProgramManager::CreateComputeProgram(uint32_t nameComputeShader)
{
	uint64_t name = (uint64_t)0xffffffff00000000 | (uint64_t)nameComputeShader;

	if (m_pPrograms[name] == NULL) {
		m_pPrograms[name] = new CGfxProgram(name);
	}

	return m_pPrograms[name];
}

CGfxProgram* CGfxProgramManager::CreateGraphicsProgram(uint32_t nameVertexShader, uint32_t nameFragmentShader)
{
	uint64_t name = ((uint64_t)nameVertexShader << 32) | (uint64_t)nameFragmentShader;

	if (m_pPrograms[name] == NULL) {
		m_pPrograms[name] = new CGfxProgram(name);
	}

	return m_pPrograms[name];
}

void CGfxProgramManager::DestroyProgram(CGfxProgram *pProgram)
{
	if (pProgram) {
		m_pPrograms.erase(pProgram->GetName());
		delete pProgram;
	}
}
