#include "Editor.h"


CEditor::CEditor(int width, int height)
	: m_pMainCamera(nullptr)
	, m_cameraAngleX(0.0f)
	, m_cameraAngleY(135.0f)
	, m_cameraMoveSpeed(1.5f)

	, m_bLButtonDown(false)
	, m_bMoveForward(false)
	, m_bMoveBackward(false)
	, m_bMoveLeft(false)
	, m_bMoveRight(false)
	, m_bMoveUp(false)
	, m_bMoveDown(false)
	, m_ptMousePosition{ -1, -1 }
{
	m_pMainCamera = new CCamera;
	m_pMainCamera->SetScissor(0.0f, 0.0f, 1.0f * width, 1.0f * height);
	m_pMainCamera->SetViewport(0.0f, 0.0f, 1.0f * width, 1.0f * height);
	m_pMainCamera->SetPerspective(45.0f, 1.0f * width / height, 0.01f, 150.0f);
}

CEditor::~CEditor(void)
{
	delete m_pMainCamera;
}

void CEditor::OnLButtonDown(int x, int y)
{
	m_bLButtonDown = true;

	m_ptMousePosition.x = x;
	m_ptMousePosition.y = y;
}

void CEditor::OnLButtonRelease(int x, int y)
{
	m_bLButtonDown = false;
	m_bMoveForward = false;
	m_bMoveBackward = false;
	m_bMoveLeft = false;
	m_bMoveRight = false;
	m_bMoveUp = false;
	m_bMoveDown = false;

	m_ptMousePosition.x = -1;
	m_ptMousePosition.y = -1;
}

void CEditor::OnMouseMove(int x, int y, int ppi)
{
	if (m_bLButtonDown) {
		if (m_ptMousePosition.x != -1 && m_ptMousePosition.y != -1) {
			m_cameraAngleX += 0.5f * (y - m_ptMousePosition.y) * 100.0f / ppi;
			m_cameraAngleY += 0.5f * (m_ptMousePosition.x - x) * 100.0f / ppi;

			m_ptMousePosition.x = x;
			m_ptMousePosition.y = y;
		}
	}
}

void CEditor::OnKeyDown(int key)
{
	switch (key) {
	case 'W': m_bMoveForward = true; break;
	case 'S': m_bMoveBackward = true; break;
	case 'A': m_bMoveLeft = true; break;
	case 'D': m_bMoveRight = true; break;
	case 'E': m_bMoveUp = true; break;
	case 'Q': m_bMoveDown = true; break;
	}
}

void CEditor::OnKeyRelease(int key)
{
	switch (key) {
	case 'W': m_bMoveForward = false; break;
	case 'S': m_bMoveBackward = false; break;
	case 'A': m_bMoveLeft = false; break;
	case 'D': m_bMoveRight = false; break;
	case 'E': m_bMoveUp = false; break;
	case 'Q': m_bMoveDown = false; break;
	}
}

void CEditor::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void CEditor::UpdateCamera(float deltaTime)
{
	static glm::vec3 position(-2.09197688f, 1.52254741f, 1.16247618f);
	static glm::vec3 forward(0.0f, 0.0f, 1.0f);
	static glm::vec3 up(0.0f, 1.0f, 0.0f);
	static glm::vec3 left(1.0f, 0.0f, 0.0f);

	// Rotate
	{
		if (m_cameraAngleX < -85.0f) m_cameraAngleX = -85.0f;
		if (m_cameraAngleX > 85.0f) m_cameraAngleX = 85.0f;

		glm::mat4 rotatex = glm::rotate(glm::mat4(), glm::radians(m_cameraAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotatey = glm::rotate(glm::mat4(), glm::radians(m_cameraAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotation = rotatey * rotatex;

		forward = glm::mat3(rotation) * glm::vec3(0.0f, 0.0f, 1.0f);
		left = glm::cross(up, forward);

		m_pMainCamera->SetLookat(position.x, position.y, position.z, position.x + forward.x, position.y + forward.y, position.z + forward.z, 0.0f, 1.0f, 0.0f);
	}

	// Translate
	{
		if (m_bMoveForward) position += forward * m_cameraMoveSpeed * deltaTime;
		if (m_bMoveBackward) position -= forward * m_cameraMoveSpeed * deltaTime;
		if (m_bMoveLeft) position += left * m_cameraMoveSpeed * deltaTime;
		if (m_bMoveRight) position -= left * m_cameraMoveSpeed * deltaTime;
		if (m_bMoveUp) position += up * m_cameraMoveSpeed * deltaTime;
		if (m_bMoveDown) position -= up * m_cameraMoveSpeed * deltaTime;

		m_pMainCamera->SetLookat(position.x, position.y, position.z, position.x + forward.x, position.y + forward.y, position.z + forward.z, 0.0f, 1.0f, 0.0f);
	}
}
