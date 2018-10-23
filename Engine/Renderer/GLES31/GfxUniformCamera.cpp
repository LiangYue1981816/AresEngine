#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxUniformCamera.h"
#include "GfxUniformBuffer.h"


CGfxUniformCamera::CGfxUniformCamera(void)
	: m_bDirty(false)
	, m_hash(INVALID_VALUE)
	, m_pUniformBuffer(NULL)
{
	m_pUniformBuffer = new CGfxUniformBuffer(sizeof(m_params), true);
}

CGfxUniformCamera::~CGfxUniformCamera(void)
{
	delete m_pUniformBuffer;
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

		uint32_t hash = HashValue((unsigned char *)&m_params, sizeof(m_params));

		if (m_hash != hash) {
			m_hash  = hash;
			m_pUniformBuffer->BufferData(0, sizeof(m_params), &m_params);
		}
	}
}

uint32_t CGfxUniformCamera::GetBuffer(void) const
{
	return m_pUniformBuffer->GetBuffer();
}

uint32_t CGfxUniformCamera::GetSize(void) const
{
	return m_pUniformBuffer->GetSize();
}
