#include "../../../Game/GameObject.h"
#include "HitResultScene.h"

#include "../../Rigidbody/Rigidbody.h"
#include "../../Transform/Transform.h"

#include "../../../../SceneBase/Manager/SceneManager.h"



void Cp_HitResultScene::UpdateContents()
{
	for (auto& result : m_rigid.lock()->GetHitResult())
	{
		if (!((UINT)KdCollider::Type::TypeScene & result.m_type))continue;
		SceneManager::Instance().SetScenePhase(m_nextScene);
		break;
	}
}

void Cp_HitResultScene::ImGuiUpdate()
{
	ImGui::InputText("NextScene", &m_nextScene);
}

void Cp_HitResultScene::InitJson()
{
	m_nextScene = m_jsonData["NextScene"];
}

nlohmann::json Cp_HitResultScene::GetJson()
{
	m_jsonData["NextScene"] = m_nextScene;
	return m_jsonData;
}

