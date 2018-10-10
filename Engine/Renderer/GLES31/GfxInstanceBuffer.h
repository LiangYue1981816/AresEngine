#pragma once
#include "GfxRenderer.h"


class CGfxInstanceBuffer
{
	friend class CGfxMesh;
	friend class CGfxVertexArrayObject;


private:
	CGfxInstanceBuffer(uint32_t format);
	virtual ~CGfxInstanceBuffer(void);


private:
	void AddInstance(const glm::mat4 &mtxTransform);
	void SetInstance(const eastl::vector<glm::mat4> &mtxTransforms);
	void ClearInstance(void);
	void UpdateInstance(void);

private:
	uint32_t GetInstanceCount(void) const;
	uint32_t GetInstanceFormat(void) const;
	uint32_t GetInstanceBuffer(void) const;
	uint32_t GetSize(void) const;

private:
	void Bind(void) const;
	void SetupFormat(void) const;


private:
	uint32_t m_instanceFormat;
	uint32_t m_instanceBuffer;
	uint32_t m_size;

private:
	bool m_bDirty;
	uint32_t m_hash;
	eastl::vector<glm::mat4> m_instances;
};
