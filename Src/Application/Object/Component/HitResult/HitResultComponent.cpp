#include "HitResultComponent.h"
#include "../../Game/GameObject.h"

void Cp_HitResult::Start()
{
	m_trans = m_owner.lock()->GetTransform();
	m_rigid = m_owner.lock()->GetComponent<RigidbodyComponent>();
	assert(m_rigid.lock() && "リジットボディがないよby HitResultReflect");
}

void Cp_HitResult::ImGuiUpdate()
{
	if (ImGui::Button("ColliderType"))ImGui::OpenPopup(("ColliderType##" + std::to_string(GetInstanceID())).c_str());
	if (ImGui::BeginPopup(("ColliderType##" + std::to_string(GetInstanceID())).c_str()))
	{
		Utility::ImGuiHelper::CheckBoxAllBit<KdCollider::Type>(m_colType);
		ImGui::EndPopup();
	}
}

void Cp_HitResult::LoadJson(nlohmann::json _json)
{
	if (_json["ColType"].is_number())m_colType = _json["ColType"];
}

nlohmann::json Cp_HitResult::Serialize()
{
	nlohmann::json json;
	json["ColType"] = m_colType;
	return json;
}
