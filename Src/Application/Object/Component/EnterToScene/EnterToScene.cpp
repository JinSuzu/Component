#include "EnterToScene.h"
#include "../../Game/GameObject.h"

void Cp_EnterToScene::PreUpdateContents()
{
	static bool Pushing = false;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (!Pushing)
		{
			SceneManager::Instance().LoadScene(m_nextScene);
			KdAudioManager::Instance().Play("Asset/Sounds/ToGame.wav");
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

void Cp_EnterToScene::LoadJson(nlohmann::json _json)
{
	m_nextScene = _json["NextScene"];
}

nlohmann::json Cp_EnterToScene::GetJson()
{
	nlohmann::json json;
	json["NextScene"] = m_nextScene;
	return json;
}
