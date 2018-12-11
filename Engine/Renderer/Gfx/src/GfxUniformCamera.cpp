#include "GfxHeader.h"


CGfxUniformCamera::CGfxUniformCamera(bool bDynamic)
	: m_bDirty(false)
	, m_hash(INVALID_HASHVALUE)
{
	m_ptrUniformBuffer = GfxRenderer()->NewUniformBuffer(sizeof(m_params), bDynamic);
}

CGfxUniformCamera::~CGfxUniformCamera(void)
{

}

void CGfxUniformCamera::SetScreen(float width, float height)
{
	m_bDirty = true;
	m_params.screen = glm::vec4(width, height, 1.0f + 1.0f / width, 1.0f + 1.0f / height);
}

void CGfxUniformCamera::SetPerspective(float fovy, float aspect, float zNear, float zFar)
{
	// [-1 1]
	//float x = (1.0f - zFar / zNear) / 2.0f;
	//float y = (1.0f + zFar / zNear) / 2.0f;

	// [0 1]
	float x = 1.0f - zFar / zNear;
	float y = zFar / zNear;

	m_bDirty = true;
	m_params.zbuffer = glm::vec4(x, y, x / zFar, y / zFar);
	m_params.projection = glm::vec4(1.0f, zNear, zFar, 1.0f / zFar);
	m_params.projectionMatrix = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	m_params.projectionViewMatrix = m_params.projectionMatrix * m_params.viewMatrix;
}

void CGfxUniformCamera::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	// [-1 1]
	//float x = (1.0f - zFar / zNear) / 2.0f;
	//float y = (1.0f + zFar / zNear) / 2.0f;

	// [0 1]
	float x = 1.0f - zFar / zNear;
	float y = zFar / zNear;

	m_bDirty = true;
	m_params.zbuffer = glm::vec4(x, y, x / zFar, y / zFar);
	m_params.projection = glm::vec4(1.0f, zNear, zFar, 1.0f / zFar);
	m_params.projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
	m_params.projectionViewMatrix = m_params.projectionMatrix * m_params.viewMatrix;
}

void CGfxUniformCamera::SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_bDirty = true;
	m_params.viewMatrix = glm::lookAt(glm::vec3(eyex, eyey, eyez), glm::vec3(centerx, centery, centerz), glm::vec3(upx, upy, upz));
	m_params.viewInverseMatrix = glm::inverse(m_params.viewMatrix);
	m_params.viewInverseTransposeMatrix = glm::transpose(m_params.viewInverseMatrix);
	m_params.projectionViewMatrix = m_params.projectionMatrix * m_params.viewMatrix;
}

void CGfxUniformCamera::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;

		uint32_t hash = HashValue((uint8_t*)&m_params, sizeof(m_params), 2);

		if (m_hash != hash) {
			m_hash  = hash;
			m_ptrUniformBuffer->BufferData(0, sizeof(m_params), &m_params);
		}
	}
}

const CGfxUniformBufferPtr& CGfxUniformCamera::GetUniformBuffer(void) const
{
	return m_ptrUniformBuffer;
}
