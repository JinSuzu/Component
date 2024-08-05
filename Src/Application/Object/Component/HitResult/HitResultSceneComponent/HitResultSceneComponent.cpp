#include "HitResultSceneComponent.h"
#include "../../../Game/GameObject.h"

void HitResultSceneComponent::UpdateContents()
{
	for (auto& result : m_rigid.lock()->GetHitResult())
	{
		if (!((UINT)KdCollider::Type::TypeScene & result.m_type))continue;
		SceneManager::Instance().LoadScene(m_nextScene);
		KdAudioManager::Instance().Play("Asset/Sounds/Clear.wav");
		break;
	}
}

void HitResultSceneComponent::ImGuiUpdate()
{
	ImGui::InputText("NextScene", &m_nextScene);
}

void HitResultSceneComponent::LoadJson(nlohmann::json _json)
{
	m_nextScene = _json["NextScene"];
}

nlohmann::json HitResultSceneComponent::GetJson()
{
	nlohmann::json json;
	json["NextScene"] = m_nextScene;
	return json;
}

