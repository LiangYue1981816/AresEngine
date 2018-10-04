#pragma once
#include "GfxRenderer.h"


class CGfxShaderCompiler
{
	friend class CGfxRenderer;


private:
	CGfxShaderCompiler(const char *szShaderCachePath);
	virtual ~CGfxShaderCompiler(void);


public:
	void AddIncludePath(const char *szPath);
	void AddMacroDefinition(const char *szName);
	void AddMacroDefinition(const char *szName, const char *szValue);
	void ClearMacroDefinition(void);

	std::string Preprocess(const char *szFileName, shaderc_shader_kind kind);
	std::vector<uint32_t> Compile(const char *szFileName, shaderc_shader_kind kind);

public:
	static std::string LoadShader(const char *szFileName);
	static bool LoadShaderBinary(const char *szFileName, std::vector<uint32_t> &words);
	static bool SaveShaderBinary(const char *szFileName, const std::vector<uint32_t> &words);


private:
	char m_szShaderCachePath[_MAX_STRING];

private:
	shaderc_util::FileFinder m_fileFinder;
	std::unique_ptr<glslc::FileIncluder> m_fileIncluder;

	std::vector<std::string> m_strMacroDefinitionNames;
	std::map<std::string, std::string> m_strMacroDefinitionNameAndValues;

private:
	shaderc::Compiler m_compiler;
	shaderc::CompileOptions m_options;
};
