#include "EnterToScene.h"
#include "../../Game/GameObject.h"
#include "../../../SceneBase/Manager/SceneManager.h"

void Cp_EnterToScene::PreUpdateContents()
{
	static bool Pushing = false;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (!Pushing)
		{
			SceneManager::Instance().SetScenePhase(m_nextScene);
			Pushing = true;
		}
		return;
	}

	Pushing = false;
}

void Cp_EnterToScene::ImGuiUpdate()
{
	ImGui::InputText("NextScene", &m_nextScene);
}

void Cp_EnterToScene::InitJson()
{
	m_nextScene = m_jsonData["NextScene"];
}

nlohmann::json Cp_EnterToScene::GetJson()
{
	m_jsonData["NextScene"] = m_nextScene;
	return m_jsonData;
}
