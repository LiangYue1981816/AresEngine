#pragma once
#include "GLES3Renderer.h"


class CGLES3DescriptorLayout : public CGfxDescriptorLayout
{
	friend class CGLES3DescriptorLayoutManager;


private:
	CGLES3DescriptorLayout(CGLES3DescriptorLayoutManager* pManager, uint32_t set);
	virtual ~CGLES3DescriptorLayout(void);
	virtual void Release(void);


public:
	bool Create(void);
	void Destroy(bool bClear = true);

public:
	void SetUniformBlockBinding(uint32_t name, uint32_t binding);
	void SetStorageBlockBinding(uint32_t name, uint32_t binding);
	void SetSampledImageBinding(uint32_t name, uint32_t binding);
	void SetInputAttachmentBinding(uint32_t name, uint32_t binding);

public:
	uint32_t GetSetIndex(void) const;
	uint32_t GetUniformBlockBinding(uint32_t name) const;
	uint32_t GetStorageBlockBinding(uint32_t name) const;
	uint32_t GetSampledImageBinding(uint32_t name) const;
	uint32_t GetInputAttachmentBinding(uint32_t name) const;

public:
	bool IsUniformBlockValid(uint32_t name) const;
	bool IsStorageBlockValid(uint32_t name) const;
	bool IsSampledImageValid(uint32_t name) const;
	bool IsInputAttachmentValid(uint32_t name) const;
	bool IsCompatible(const CGfxDescriptorLayoutPtr ptrLayout) const;


private:
	uint32_t m_set;
	eastl::unordered_map<uint32_t, uint32_t> m_uniformBlockBindings;    // [name, binding]
	eastl::unordered_map<uint32_t, uint32_t> m_storageBlockBindings;    // [name, binding]
	eastl::unordered_map<uint32_t, uint32_t> m_sampledImageBindings;    // [name, binding]
	eastl::unordered_map<uint32_t, uint32_t> m_inputAttachmentBindings; // [name, binding]

private:
	CGLES3DescriptorLayoutManager* m_pManager;
};
