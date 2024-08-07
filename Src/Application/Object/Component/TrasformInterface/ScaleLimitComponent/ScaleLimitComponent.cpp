#include "ScaleLimitComponent.h"
#include "../../../Game/GameObject.h"

void  ScaleLimitComponent::PostUpdateContents()
{
	Math::Vector3 scale = m_trans.lock()->GetLocalScale();
	if (m_enableShaft & (UINT)Shaft::X) 
	{
		if(m_enableMax)Utility::MathHelper::MaxBorder(scale.x,m_max.x);
		if(m_enableMin)Utility::MathHelper::MinBorder(scale.x,m_min.x);
	}

	if (m_enableShaft & (UINT)Shaft::Y) 
	{
		if(m_enableMax)Utility::MathHelper::MaxBorder(scale.y,m_max.y);
		if(m_enableMin)Utility::MathHelper::MinBorder(scale.y,m_min.y);
	}

	if (m_enableShaft & (UINT)Shaft::Z) 
	{
		if(m_enableMax)Utility::MathHelper::MaxBorder(scale.z,m_max.z);
		if(m_enableMin)Utility::MathHelper::MinBorder(scale.z,m_min.z);
	}
	m_trans.lock()->SetLocalScale(scale);
}

void  ScaleLimitComponent::ImGuiUpdate()
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

void  ScaleLimitComponent::LoadJson(nlohmann::json _json)
{
	m_max = Utility::JsonHelper::InPutVec3(_json["MaxBorder"]);
	m_min = Utility::JsonHelper::InPutVec3(_json["MinBorder"]);

	m_enableMax = _json["EnableMax"];
	m_enableMin = _json["EnableMin"];

	m_enableShaft = _json["EnableShaft"];
}

nlohmann::json  ScaleLimitComponent::Serialize()
{
	nlohmann::json json;
	json["MaxBorder"] = Utility::JsonHelper::OutPutVec3(m_max);
	json["MinBorder"] = Utility::JsonHelper::OutPutVec3(m_min);

	json["EnableMax"] = m_enableMax;
	json["EnableMin"] = m_enableMin;

	json["EnableShaft"] = m_enableShaft;
	return json;
}