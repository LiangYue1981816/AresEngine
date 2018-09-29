#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxShaderCompiler.h"


static std::string LoadShader(const char *szFileName)
{
	if (FILE *pFile = fopen(szFileName, "rb")) {
		static char szSource[128 * 1024];
		size_t size = fsize(pFile);

		fread(szSource, 1, size, pFile);
		fclose(pFile);

		return szSource;
	}

	return "";
}

static bool LoadShaderBinary(const char *szFileName, std::vector<uint32_t> &words)
{
	FILE *pFile = fopen(szFileName, "rb");
	if (pFile == NULL) return false;

	uint32_t dwHashValue;

	words.clear();
	words.resize((fsize(pFile) - sizeof(dwHashValue)) / sizeof(uint32_t));

	fread(&dwHashValue, sizeof(dwHashValue), 1, pFile);
	fread(words.data(), sizeof(uint32_t), words.size(), pFile);
	fclose(pFile);

	return HashValue((uint8_t *)words.data(), sizeof(uint32_t) * words.size()) == dwHashValue ? true : false;
}

static bool SaveShaderBinary(const char *szFileName, const std::vector<uint32_t> &words)
{
	FILE *pFile = fopen(szFileName, "wb");
	if (pFile == NULL) return false;

	uint32_t dwHashValue = HashValue((uint8_t *)words.data(), sizeof(uint32_t) * words.size());

	fwrite(&dwHashValue, sizeof(dwHashValue), 1, pFile);
	fwrite(words.data(), sizeof(uint32_t), words.size(), pFile);
	fclose(pFile);

	return true;
}

static std::string PreprocessShader(std::string &source, shaderc_shader_kind kind, const shaderc::Compiler &compiler, const shaderc::CompileOptions &options)
{
	shaderc::PreprocessedSourceCompilationResult module = compiler.PreprocessGlsl(source, kind, "SPIR-V Compiler", options);

	if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
		LogOutput("GfxRenderer", "Preprocess Fail: %s\n", module.GetErrorMessage().c_str());
		return "";
	}

	return { module.cbegin(), module.cend() };
}

static bool CompileShader(std::string &source, shaderc_shader_kind kind, const shaderc::Compiler &compiler, const shaderc::CompileOptions &options, std::vector<uint32_t> &words)
{
	shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source.c_str(), source.size(), kind, "SPIR-V Compiler", options);

	if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
		LogOutput("GfxRenderer", "Compile Fail: %s\n", module.GetErrorMessage().c_str());
		return false;
	}

	words = { module.cbegin(), module.cend() };
	return true;
}


CGfxShaderCompiler::CGfxShaderCompiler(const char *szShaderCachePath)
	: m_szShaderCachePath{ 0 }
	, m_fileIncluder(new glslc::FileIncluder(&m_fileFinder))
{
	strcpy(m_szShaderCachePath, szShaderCachePath);

	m_options.SetIncluder(std::move(m_fileIncluder));
	m_options.SetWarningsAsErrors();
	m_options.SetSourceLanguage(shaderc_source_language_glsl);
	m_options.SetForcedVersionProfile(310, shaderc_profile_es);
	m_options.AddMacroDefinition("GLES");
}

CGfxShaderCompiler::~CGfxShaderCompiler(void)
{

}

void CGfxShaderCompiler::AddIncludePath(const char *szPath)
{
	m_fileFinder.search_path().emplace_back(szPath);
}

void CGfxShaderCompiler::AddMacroDefinition(const char *szName)
{
	m_strMacroDefinitionNames.push_back(szName);
}

void CGfxShaderCompiler::AddMacroDefinition(const char *szName, const char *szValue)
{
	m_strMacroDefinitionNameAndValues[szName] = szValue;
}

void CGfxShaderCompiler::ClearMacroDefinition(void)
{
	m_strMacroDefinitionNames.clear();
	m_strMacroDefinitionNameAndValues.clear();
}

std::string CGfxShaderCompiler::Preprocess(const char *szFileName, shaderc_shader_kind kind)
{
	shaderc::CompileOptions options(m_options);

	for (const auto &itMacroDefinition : m_strMacroDefinitionNames) {
		options.AddMacroDefinition(itMacroDefinition);
	}

	for (const auto &itMacroDefinition : m_strMacroDefinitionNameAndValues) {
		options.AddMacroDefinition(itMacroDefinition.first, itMacroDefinition.second);
	}

	std::string source = LoadShader(szFileName);
	if (source.empty()) return "";

	return PreprocessShader(source, kind, m_compiler, options);
}

std::vector<uint32_t> CGfxShaderCompiler::Compile(const char *szFileName, shaderc_shader_kind kind)
{
	std::vector<uint32_t> words;

	do {
		shaderc::CompileOptions options(m_options);

		for (const auto &itMacroDefinition : m_strMacroDefinitionNames) {
			options.AddMacroDefinition(itMacroDefinition);
		}

		for (const auto &itMacroDefinition : m_strMacroDefinitionNameAndValues) {
			options.AddMacroDefinition(itMacroDefinition.first, itMacroDefinition.second);
		}

		std::string source = LoadShader(szFileName);
		if (source.empty()) break;

		std::string preprocess = PreprocessShader(source, kind, m_compiler, options);
		if (preprocess.empty()) break;

		char szBinFileName[_MAX_STRING];
		sprintf(szBinFileName, "%s/%x", m_szShaderCachePath, HashValue(preprocess.c_str()));

		if (LoadShaderBinary(szBinFileName, words) == false) {
			if (CompileShader(source, kind, m_compiler, options, words) == false) {
				break;
			}

			if (SaveShaderBinary(szBinFileName, words) == false) {
				break;
			}
		}
	} while (false);

	return words;
}
