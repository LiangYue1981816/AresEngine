#pragma once
#include "GLES3Renderer.h"


class CGLES3Shader : public CGfxShader
{
	friend class CGLES3ShaderManager;


private:
	CGLES3Shader(uint32_t name);
	virtual ~CGLES3Shader(void);


public:
	uint32_t GetName(void) const;

public:
	HANDLE GetShader(void) const;

public:
	uint32_t GetKind(void) const;

public:
	bool IsValid(void) const;
	bool IsTextureValid(uint32_t name) const;
	bool IsUniformValid(uint32_t name) const;
	bool IsUniformBlockValid(uint32_t name) const;

public:
	uint32_t GetTextureLocation(uint32_t name) const;
	uint32_t GetUniformLocation(uint32_t name) const;
	uint32_t GetUniformBlockBinding(uint32_t name) const;

public:
	bool Create(const uint32_t *words, size_t numWords, shader_kind kind);
	void Destroy(void);

private:
	bool CreateLayouts(void);
	void SetUniformLocation(const char *szName);
	void SetUniformBlockBinding(const char *szName, uint32_t binding);
	void SetSampledImageLocation(const char *szName);


private:
	uint32_t m_name;

private:
	uint32_t m_kind;
	uint32_t m_program;

private:
	CGfxSprivCross m_spriv;
	eastl::unordered_map<uint32_t, uint32_t> m_uniformLocations;      // [name, location]
	eastl::unordered_map<uint32_t, uint32_t> m_uniformBlockBindings;  // [name, binding]
	eastl::unordered_map<uint32_t, uint32_t> m_sampledImageLocations; // [name, location]
};
