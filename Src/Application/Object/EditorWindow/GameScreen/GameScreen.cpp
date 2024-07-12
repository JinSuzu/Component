#include "GameScreen.h"
#include "../../../ImGuiHelper/ImGuiEditor.h"
#include "../../../RenderManger/RenderManger.h"

#include "../../Game/Manager/GameObjectManager.h"
#include "../../Game/GameObject.h"
#include "../../Component/Transform/Transform.h"

#include "../../../main.h"

void GameScreen::Update()
{
	if (!m_buildCamera && Application::Instance().GetBuildFlg())m_buildCamera = GameObjectManager::CreateObject(MyJson::InPutJson("Asset/Data/Prefab/BuildCamera"), std::weak_ptr<GameObject>(), false);
	else if (!Application::Instance().GetBuildFlg())m_buildCamera = nullptr;

	ImVec2 size(1280.0f, 720.0f);

	ImGui::BeginChild(std::to_string((int)this).c_str());
	{
		ID3D11ShaderResourceView* rtv = RenderManager::Instance().GetDebugView().m_RTTexture->WorkSRView();
		ImGui::Image(rtv, size * (ImGui::GetContentRegionAvail() / size));
	}
	ImGui::EndChild();

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
