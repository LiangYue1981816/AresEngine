#include "Utils.h"
#include "Stream.h"
#include "FileManager.h"
#include "ShaderCompiler.h"


#ifdef PLATFORM_WINDOWS

CShaderCompiler* CShaderCompiler::pInstance = nullptr;
CShaderCompiler* CShaderCompiler::GetInstance(void)
{
	return pInstance;
}

CShaderCompiler::CShaderCompiler(void)
	: m_szCachePath{ 0 }
	, m_fileIncluder(new glslc::FileIncluder(&m_fileFinder))
{
	pInstance = this;

	m_options.SetIncluder(std::move(m_fileIncluder));
	m_options.SetWarningsAsErrors();
	m_options.SetAutoBindUniforms(true);
	m_options.SetSourceLanguage(shaderc_source_language_glsl);
	m_options.SetForcedVersionProfile(310, shaderc_profile_es);
//	m_options.SetOptimizationLevel(shaderc_optimization_level_performance);
}

CShaderCompiler::~CShaderCompiler(void)
{
	pInstance = nullptr;
}

void CShaderCompiler::SetCachePath(const char* szPath)
{
	strcpy(m_szCachePath, szPath);
}

void CShaderCompiler::AddIncludePath(const char* szPath)
{
	m_fileFinder.search_path().emplace_back(szPath);
}

void CShaderCompiler::AddMacroDefinition(const char* szName)
{
	m_strMacroDefinitionNames.emplace_back(szName);
}

void CShaderCompiler::AddMacroDefinition(const char* szName, const char* szValue)
{
	m_strMacroDefinitionNameAndValues[szName] = szValue;
}

void CShaderCompiler::ClearMacroDefinition(void)
{
	m_strMacroDefinitionNames.clear();
	m_strMacroDefinitionNameAndValues.clear();
}

static std::string PreprocessShader(std::string& source, shaderc_shader_kind kind, const shaderc::Compiler& compiler, const shaderc::CompileOptions& options)
{
	shaderc::PreprocessedSourceCompilationResult module = compiler.PreprocessGlsl(source, kind, "SPIR-V Preprocess", options);

	if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
		LogOutput(nullptr, "\n");
		LogOutput(nullptr, "Preprocess Fail:\n");
		LogOutput(nullptr, "%s\n", module.GetErrorMessage().c_str());
		return "";
	}

	return { module.cbegin(), module.cend() };
}

std::string CShaderCompiler::Preprocess(const char* szFileName, shaderc_shader_kind kind)
{
	shaderc::CompileOptions options(m_options);

	for (const auto& itMacroDefinition : m_strMacroDefinitionNames) {
		options.AddMacroDefinition(itMacroDefinition);
	}

	for (const auto& itMacroDefinition : m_strMacroDefinitionNameAndValues) {
		options.AddMacroDefinition(itMacroDefinition.first, itMacroDefinition.second);
	}

	const char szKindDefine[3][_MAX_STRING] = { "VERTEX_SHADER", "FRAGMENT_SHADER", "COMPUTE_SHADER" };
	options.AddMacroDefinition(szKindDefine[kind]);

	std::string source = LoadShader(szFileName);
	if (source.empty()) return "";

	return PreprocessShader(source, kind, m_compiler, options);
}

static bool CompileShader(std::string& source, shaderc_shader_kind kind, const shaderc::Compiler& compiler, const shaderc::CompileOptions& options, std::vector<uint32_t>& words)
{
	shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, kind, "SPIR-V Compiler", options);

	if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
		LogOutput(nullptr, "Source:\n");
		LogOutput(nullptr, "%s\n", source.c_str());
		LogOutput(nullptr, "Compile Fail:\n");
		LogOutput(nullptr, "%s\n", module.GetErrorMessage().c_str());
		return false;
	}

	words = { module.cbegin(), module.cend() };
	return true;
}

std::vector<uint32_t> CShaderCompiler::Compile(const char* szInputFileName, const char* szOutputFileName, shaderc_shader_kind kind)
{
	std::vector<uint32_t> words;
	{
		do {
			shaderc::CompileOptions options(m_options);

			for (const auto& itMacroDefinition : m_strMacroDefinitionNames) {
				options.AddMacroDefinition(itMacroDefinition);
			}

			for (const auto& itMacroDefinition : m_strMacroDefinitionNameAndValues) {
				options.AddMacroDefinition(itMacroDefinition.first, itMacroDefinition.second);
			}

			const char szKindDefine[3][_MAX_STRING] = { "VERTEX_SHADER", "FRAGMENT_SHADER", "COMPUTE_SHADER" };
			options.AddMacroDefinition(szKindDefine[kind]);

			std::string source = LoadShader(szInputFileName);
			if (source.empty()) break;

			std::string preprocess = PreprocessShader(source, kind, m_compiler, options);
			if (preprocess.empty()) break;

			uint32_t hash = HashValue(preprocess.c_str());

			char szBinFileName[_MAX_STRING] = { 0 };
			sprintf(szBinFileName, "%s/%s", m_szCachePath, szOutputFileName);

			if (LoadShaderBinary(szBinFileName, words, hash) == false) {
				if (CompileShader(source, kind, m_compiler, options, words) == false) {
					break;
				}

				if (SaveShaderBinary(szBinFileName, words, hash) == false) {
					break;
				}

				FileManager()->SetFile(szOutputFileName, szBinFileName);
			}
		} while (false);
	}
	return words;
}

#endif


std::string LoadShader(const char* szFileName)
{
	if (FILE* pFile = fopen(szFileName, "rb")) {
		size_t size = fsize(pFile);
		char szSource[128 * 1024] = { 0 };

		fread(szSource, sizeof(char), size, pFile);
		fclose(pFile);

		return szSource;
	}

	return "";
}

bool LoadShaderStream(const char* szFileName, std::vector<uint32_t>& words, uint32_t hash)
{
	CStream stream;
	if (FileManager()->LoadStream(szFileName, &stream)) {
		uint32_t dwHashValue;

		words.clear();
		words.resize((stream.GetFullSize() - sizeof(uint32_t)) / sizeof(uint32_t));

		stream.Read(&dwHashValue, sizeof(uint32_t), 1);
		stream.Read(words.data(), sizeof(uint32_t), words.size());

#ifdef PLATFORM_WINDOWS
		if (hash != INVALID_HASHVALUE) {
			return dwHashValue == hash;
		}
#endif

		return true;
	}

	return false;
}

bool LoadShaderBinary(const char* szFileName, std::vector<uint32_t>& words, uint32_t hash)
{
	if (FILE* pFile = fopen(szFileName, "rb")) {
		uint32_t dwHashValue;

		words.clear();
		words.resize((fsize(pFile) - sizeof(uint32_t)) / sizeof(uint32_t));

		fread(&dwHashValue, sizeof(uint32_t), 1, pFile);
		fread(words.data(), sizeof(uint32_t), words.size(), pFile);
		fclose(pFile);

#ifdef PLATFORM_WINDOWS
		if (hash != INVALID_HASHVALUE) {
			return dwHashValue == hash;
		}
#endif

		return true;
	}

	return false;
}

bool SaveShaderBinary(const char* szFileName, const std::vector<uint32_t>& words, uint32_t hash)
{
	if (FILE* pFile = fopen(szFileName, "wb")) {
		fwrite(&hash, sizeof(uint32_t), 1, pFile);
		fwrite(words.data(), sizeof(uint32_t), words.size(), pFile);
		fclose(pFile);

		return true;
	}

	return false;
}
