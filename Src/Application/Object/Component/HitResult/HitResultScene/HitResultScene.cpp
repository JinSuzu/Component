#include "../../../Game/GameObject.h"
#include "HitResultScene.h"

#include "../../Rigidbody/Rigidbody.h"
#include "../../Transform/Transform.h"

#include "../../../../../System/SceneMnager/SceneManager.h"



void Cp_HitResultScene::UpdateContents()
{
	for (auto& result : m_rigid.lock()->GetHitResult())
	{
		if (!((UINT)KdCollider::Type::TypeScene & result.m_type))continue;
		SceneManager::Instance().LoadScene(m_nextScene);
		KdAudioManager::Instance().Play("Asset/Sounds/Clear.wav");
		break;
	}
}

void Cp_HitResultScene::ImGuiUpdate()
{
	ImGui::InputText("NextScene", &m_nextScene);
}

void Cp_HitResultScene::LoadJson(nlohmann::json _json)
{
	m_nextScene = _json["NextScene"];
}

nlohmann::json Cp_HitResultScene::GetJson()
{
	nlohmann::json json;
	json["NextScene"] = m_nextScene;
	return json;
}

