#pragma once
#include "GfxRenderer.h"


class CGfxShaderCompiler
{
	friend class CGfxRenderer;


private:
	CGfxShaderCompiler(void);
	virtual ~CGfxShaderCompiler(void);


public:
	void SetCachePath(const char *szPath);

	void AddIncludePath(const char *szPath);
	void AddMacroDefinition(const char *szName);
	void AddMacroDefinition(const char *szName, const char *szValue);
	void ClearMacroDefinition(void);

	std::vector<uint32_t> Compile(const char *szFileName, shaderc_shader_kind kind);


private:
	char m_szCachePath[_MAX_STRING];

	shaderc_util::FileFinder m_fileFinder;
	std::unique_ptr<glslc::FileIncluder> m_fileIncluder;

	std::vector<std::string> m_strMacroDefinitionNames;
	std::map<std::string, std::string> m_strMacroDefinitionNameAndValues;

	shaderc::Compiler m_compiler;
};
