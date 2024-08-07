#include "PositionLimitComponent.h"
#include "../../../Game/GameObject.h"

void PositionLimitComponent::PostUpdateContents()
{
	Math::Vector3 pos = m_trans.lock()->GetLocalPosition();
	if (m_enableShaft & (UINT)Shaft::X) 
	{
		if(m_enableMax)Utility::MathHelper::MaxBorder(pos.x,m_max.x);
		if(m_enableMin)Utility::MathHelper::MinBorder(pos.x,m_min.x);
	}

	if (m_enableShaft & (UINT)Shaft::Y) 
	{
		if(m_enableMax)Utility::MathHelper::MaxBorder(pos.y,m_max.y);
		if(m_enableMin)Utility::MathHelper::MinBorder(pos.y,m_min.y);
	}

	if (m_enableShaft & (UINT)Shaft::Z) 
	{
		if(m_enableMax)Utility::MathHelper::MaxBorder(pos.z,m_max.z);
		if(m_enableMin)Utility::MathHelper::MinBorder(pos.z,m_min.z);
	}
	m_trans.lock()->SetLoaclPosition(pos);
}

void PositionLimitComponent::ImGuiUpdate()
{
	if (ImGui::Checkbox("EnableMax", &m_enableMax); m_enableMax) 
	{
		ImGui::SameLine();
		ImGui::DragFloat3("MaxBorder", &m_max.x);
	}
	if (ImGui::Checkbox("EnableMin", &m_enableMin); m_enableMin) 
	{
		ImGui::SameLine();
		ImGui::DragFloat3("MinBorder", &m_min.x);
	}

	if (ImGui::Button("EnableShaft"))ImGui::OpenPopup(("EnableShaft" + std::to_string((int)this)).c_str());
	if (ImGui::BeginPopup(("EnableShaft" + std::to_string((int)this)).c_str()))
	{
		Utility::ImGuiHelper::CheckBoxAllBit<Shaft>(m_enableShaft);
		ImGui::EndPopup();
	}
}

void PositionLimitComponent::LoadJson(nlohmann::json _json)
{
	m_max = Utility::JsonHelper::InPutVec3(_json["MaxBorder"]);
	m_min = Utility::JsonHelper::InPutVec3(_json["MinBorder"]);

	m_enableMax = _json["EnableMax"];
	m_enableMin = _json["EnableMin"];

	m_enableShaft = _json["EnableShaft"];
}

nlohmann::json PositionLimitComponent::Serialize()
{
	nlohmann::json json;
	json["MaxBorder"] = Utility::JsonHelper::OutPutVec3(m_max);
	json["MinBorder"] = Utility::JsonHelper::OutPutVec3(m_min);

	json["EnableMax"] = m_enableMax;
	json["EnableMin"] = m_enableMin;

	json["EnableShaft"] = m_enableShaft;
	return json;
}