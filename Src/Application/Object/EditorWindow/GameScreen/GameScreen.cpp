#include "GameScreen.h"
#include "../../../ImGuiHelper/ImGuiEditor.h"
#include "../../../RenderManger/RenderManger.h"

#include "../../Game/Manager/GameObjectManager.h"
#include "../../Game/GameObject.h"
#include "../../Component/Transform/Transform.h"
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

	if (!m_buildCamera && Application::Instance().GetBuildFlg())m_buildCamera = GameObjectManager::CreateObject(MyJson::InputJson("Asset/Data/Prefab/BuildCamera"), std::weak_ptr<GameObject>(), false);
	else if (!Application::Instance().GetBuildFlg())m_buildCamera = nullptr;

	ImVec2 size(1280.0f, 720.0f);
	ID3D11ShaderResourceView* rtv = RenderManager::Instance().GetDebugView().m_RTTexture->WorkSRView();
	ImGui::Image(rtv, size * (ImGui::GetContentRegionAvail() / size));
}

void GameScreen::PostUpdate()
{
	static Math::Vector2 postMousePos = Application::Instance().GetMouse();
	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			//カメラ視点移動
		}

		if (ImGui::IsKeyDown(ImGuiKey_MouseWheelY))
		{
			//カメラ移動
		}
	}
	Application::Instance().GetMouse();
}
