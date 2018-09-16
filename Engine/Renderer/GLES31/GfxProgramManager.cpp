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

CGfxProgram* CGfxProgramManager::CreateProgram(const char *szVertexFileName, const char *szFragmentFileName)
{
	GLuint64 nameVertex = HashValue(szVertexFileName);
	GLuint64 nameFragment = HashValue(szFragmentFileName);
	GLuint64 name = nameVertex << 32 | nameFragment;

	if (m_pPrograms[name] == NULL) {
		m_pPrograms[name] = new CGfxProgram;
		m_pPrograms[name]->Load(szVertexFileName, szFragmentFileName);
	}

	return m_pPrograms[name];
}
