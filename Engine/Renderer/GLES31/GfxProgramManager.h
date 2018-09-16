#pragma once
#include "GfxRenderer.h"


class CGfxProgramManager
{
	friend class CGfxRenderer;


private:
	CGfxProgramManager(void);
	virtual ~CGfxProgramManager(void);


private:
	CGfxProgram* CreateProgram(const char *szVertexFileName, const char *szFragmentFileName);


private:
	eastl::unordered_map<GLuint64, CGfxProgram*> m_pPrograms;
};
