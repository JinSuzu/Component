#include "../../Game/GameObject.h"
#include "HitResult.h"

#include "../Rigidbody/Rigidbody.h"
#include "../Transform/Transform.h"

void Cp_HitResult::Start()
{
	m_trans = m_owner.lock()->GetTransform();
	m_rigid = m_owner.lock()->GetComponent<Cp_Rigidbody>();
	assert(m_rigid.lock() && "リジットボディがないよby HitResultReflect");
}

void Cp_HitResult::ImGuiUpdate()
{
	if (ImGui::Button("ColliderType"))ImGui::OpenPopup(("ColliderType##" + std::to_string(GetInstanceID())).c_str());
	if (ImGui::BeginPopup(("ColliderType##" + std::to_string(GetInstanceID())).c_str()))
	{
		for (int i = 0; i < std::log2((double)KdCollider::Type::TypeMax); i++)
		{
			KdCollider::Type type = static_cast<KdCollider::Type>(1 << i);
			ImGuiCheckBoxBit(magic_enum::enum_name(type).data(), m_colType, (UINT)type);
		}
		ImGui::EndPopup();
	}
}

void Cp_HitResult::InitJson()
{
	if (m_jsonData["ColType"].is_number())m_colType = m_jsonData["ColType"];
}

nlohmann::json Cp_HitResult::GetJson()
{
	m_jsonData["ColType"] = m_colType;
	return m_jsonData;
}
