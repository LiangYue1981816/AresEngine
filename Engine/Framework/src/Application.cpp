#include "imgui.h"
#include "imgui_impl_win32.h"

#include "Application.h"
#include "Framework.h"
#include "EngineHeader.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


CApplication::CApplication(void)
	: m_hDC(nullptr)
	, m_width(0)
	, m_height(0)
{

}

CApplication::~CApplication(void)
{

}

void CApplication::Update(void)
{
	/*
	static uint32_t lastTick = 0;
	float deltaTime = (Tick() - lastTick) / 1000000.0f;

	if (deltaTime > 1.0f / 60.0f) {
		static bool bFirstFrame = true;
		{
			if (bFirstFrame == false) {
				UpdateInternal(deltaTime);
			}
		}
		bFirstFrame = false;

		lastTick = Tick();
	}
	/*/
	uint32_t currTick = Tick();
	{
		static uint32_t lastTick = 0;
		UpdateInternal((currTick - lastTick) / 1000000.0f);
		lastTick = currTick;
	}
	//*/
}

int64_t CApplication::WndProc(void* hWnd, uint32_t msg, uint64_t wParam, int64_t lParam)
{
	return ImGui_ImplWin32_WndProcHandler((HWND)hWnd, msg, wParam, lParam);
}

void UpdateInput(void)
{
	// Mouse
	{
		ImVec2 curMousePos = ImGui::GetMousePos();
		int x = (int)curMousePos.x;
		int y = (int)curMousePos.y;

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsAnyWindowHovered() == false) {
			Framework()->OnLButtonDown(x, y);
		}

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
			Framework()->OnLButtonRelease(x, y);
		}

		Framework()->OnMouseMove(x, y);
	}

	// Key
	{
		if (ImGui::IsKeyDown('W')) Framework()->OnKeyDown('W');
		if (ImGui::IsKeyDown('S')) Framework()->OnKeyDown('S');
		if (ImGui::IsKeyDown('A')) Framework()->OnKeyDown('A');
		if (ImGui::IsKeyDown('D')) Framework()->OnKeyDown('D');
		if (ImGui::IsKeyDown('E')) Framework()->OnKeyDown('E');
		if (ImGui::IsKeyDown('Q')) Framework()->OnKeyDown('Q');

		if (ImGui::IsKeyReleased('W')) Framework()->OnKeyRelease('W');
		if (ImGui::IsKeyReleased('S')) Framework()->OnKeyRelease('S');
		if (ImGui::IsKeyReleased('A')) Framework()->OnKeyRelease('A');
		if (ImGui::IsKeyReleased('D')) Framework()->OnKeyRelease('D');
		if (ImGui::IsKeyReleased('E')) Framework()->OnKeyRelease('E');
		if (ImGui::IsKeyReleased('Q')) Framework()->OnKeyRelease('Q');
	}
}

void UpdateRenderSolution(void)
{
	float shRed[9] = { 0.588794f, 0.254506f, 0.082485f, 0.028278f, 0.036788f, 0.086514f, 0.018185f, 0.021567f, 0.006525f };
	float shGreen[9] = { 0.599826f, 0.275154f, 0.076468f, 0.028166f, 0.032798f, 0.082039f, 0.014064f, 0.020624f, 0.001984f };
	float shBlue[9] = { 0.558644f, 0.387983f, 0.073534f, 0.024760f, 0.029393f, 0.076786f, 0.008863f, 0.017515f, -0.004231f };

	static float angle = 0.0f; angle += 0.002f;
	glm::vec4 directLightDirection = glm::rotate(glm::mat4(), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.35f, -1.0f, 0.0f, 0.0f);

	RenderSystem()->SetEnvLightFactor(1.5f);

	RenderSystem()->SetAmbientLightFactor(0.5f);
	RenderSystem()->SetAmbientLightSH(shRed, shGreen, shBlue);

	RenderSystem()->SetMainPointLightFactor(1.0f);

	RenderSystem()->SetMainDirectLightFactor(1.5f);
	RenderSystem()->SetMainDirectLightColor(3.5f, 3.5f, 3.5f);
	RenderSystem()->SetMainDirectLightDirection(directLightDirection.x, directLightDirection.y, directLightDirection.z);

	Settings()->SetValue("RenderSystem.Shadow.Factor", 1.0f);
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor0", exp(-4.0f));
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor1", exp(-3.0f));
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor2", exp(-2.0f));
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor3", exp(-1.0f));
}
