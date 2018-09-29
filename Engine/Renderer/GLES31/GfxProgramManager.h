#pragma once
#include "GfxRenderer.h"


class CGfxProgramManager
{
	friend class CGfxRenderer;


private:
	CGfxProgramManager(void);
	virtual ~CGfxProgramManager(void);


private:
	CGfxProgram* CreateComputeProgram(uint32_t nameComputeShader);
	CGfxProgram* CreateGraphicsProgram(uint32_t nameVertexShader, uint32_t nameFragmentShader);
	void DestroyProgram(CGfxProgram *pProgram);


private:
	eastl::unordered_map<uint64_t, CGfxProgram*> m_pPrograms;
};
