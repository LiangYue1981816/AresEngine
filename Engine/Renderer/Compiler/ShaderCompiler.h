#pragma once
#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include <shaderc/shaderc.hpp>
#include <glslc/file_includer.h>


#define ShaderCompiler() CShaderCompiler::GetInstance()


class CShaderCompiler
{
public:
	static CShaderCompiler* GetInstance(void);


private:
	CShaderCompiler(void);
	virtual ~CShaderCompiler(void);


public:
	void SetCachePath(const char *szPath);

	void AddIncludePath(const char *szPath);
	void AddMacroDefinition(const char *szName);
	void AddMacroDefinition(const char *szName, const char *szValue);
	void ClearMacroDefinition(void);

	std::string Preprocess(const char *szFileName, shaderc_shader_kind kind);
	std::vector<uint32_t> Compile(const char *szInputFileName, const char *szOutputFileName, shaderc_shader_kind kind);

public:
	std::string LoadShader(const char *szFileName);
	bool LoadShaderBinary(const char *szFileName, std::vector<uint32_t> &words, uint32_t hash);
	bool SaveShaderBinary(const char *szFileName, const std::vector<uint32_t> &words, uint32_t hash);


private:
	char m_szCachePath[_MAX_STRING];

private:
	std::vector<std::string> m_strMacroDefinitionNames;
	std::map<std::string, std::string> m_strMacroDefinitionNameAndValues;

private:
	shaderc::Compiler m_compiler;
	shaderc::CompileOptions m_options;

	shaderc_util::FileFinder m_fileFinder;
	std::unique_ptr<glslc::FileIncluder> m_fileIncluder;
};
