#include "EnterToSceneComponent.h"
#include "../../../Game/GameObject.h"

void EnterToSceneComponent::PreUpdateContents()
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

void EnterToSceneComponent::ImGuiUpdate()
{
	ImGui::InputText("NextScene", &m_nextScene);
}

void EnterToSceneComponent::LoadJson(nlohmann::json _json)
{
	m_nextScene = _json["NextScene"];
}

nlohmann::json EnterToSceneComponent::GetJson()
{
	nlohmann::json json;
	json["NextScene"] = m_nextScene;
	return json;
}
