#include "GameScreen.h"
#include "../../../ImGuiHelper/ImGuiEditor.h"
#include "../../../RenderManger/RenderManger.h"

#include "../../Game/Manager/GameObjectManager.h"
#include "../../Game/GameObject.h"
#include "../../Component/Transform/Transform.h"
#include "../../Component/BuildCamera/BuildCamera.h"
#include "../../../ImGuiHelper/ImGuiHelper.h"

#include "../../../main.h"

void GameScreen::UpdateContents()
{
	if (MyImGui::ButtonWindowCenter(Application::Instance().GetBuildFlg() ? "StartRun" : "StartBuild"))
	{
		//SceneManager::Instance().ReLoad();
		Application::Instance().TurnBuildFlg();
	}
	if (GetAsyncKeyState(Application::Instance().GetBuildFlg() ? VK_F5 : VK_ESCAPE) & 0x8000)
	{
		//SceneManager::Instance().ReLoad();
		Application::Instance().TurnBuildFlg();
	}

	if (!m_buildCamera && Application::Instance().GetBuildFlg())
	{
		m_buildCamera = GameObjectManager::CreateObject(std::string("BuildCamera"));
		m_buildCamera->AddComponent("Camera");
		m_cameraController = std::static_pointer_cast<Cp_BuildCamera>(m_buildCamera->AddComponent("BuildCamera"));
	}
	else if (!Application::Instance().GetBuildFlg())m_buildCamera = nullptr;

	ImVec2 size(1280.0f, 720.0f);
	ID3D11ShaderResourceView* rtv = RenderManager::Instance().GetDebugView().m_RTTexture->WorkSRView();
	ImVec2 imgeSize = ImGui::GetContentRegionAvail() / size;
	imgeSize = imgeSize.x < imgeSize.y ? ImVec2(imgeSize.x, imgeSize.x) : ImVec2(imgeSize.y, imgeSize.y);

	MyImGui::ImageWindowCenter(rtv, size * imgeSize, { 0.0f,ImGui::GetItemRectSize().y });
}

void GameScreen::PostUpdate()
{
	if (m_cameraController.expired())return;
	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsKeyDown(ImGuiKey_MouseWheelY))
		{
			m_cameraController.lock()->HoverMove();
		}
		if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
		{
			m_cameraController.lock()->AngleShift();
			Application::Instance().m_log.AddLog("Hovered & Clicked\n");
		}

	}
}
