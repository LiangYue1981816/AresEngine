#include "GfxHeader.h"
#include "SceneHeader.h"
#include "ResourceLoader.h"


bool CResourceLoader::LoadShader(const char* szFileName, CGfxShader* pShader, shader_kind kind)
{
	pShader->Destroy();
	{
		do {
			std::vector<uint32_t> words;
			if (LoadShaderStream(szFileName, words, INVALID_HASHVALUE) == false) break;
			if (pShader->Create(words.data(), words.size(), kind) == false) break;
			return true;
		} while (false);
	}
	pShader->Destroy();
	return false;
}
