#include "TransformLimit.h"
#include "../Transform/Transform.h"
#include "../../Game/GameObject.h"

void Cp_TransformLimit::PostUpdateContents()
{
	auto trans = m_owner.lock()->GetTransform().lock();

	Math::Vector3 pos = trans->GetPosition();
	Clamp(pos,m_posLimit);
	trans->SetPosition(pos);
	
	Math::Vector3 rota = trans->GetRotation();
	Clamp(rota,m_rotaLimit);
	trans->SetRotation(rota);
	
	Math::Vector3 scale = trans->GetScale();
	Clamp(scale,m_scaleLimit);
	trans->SetScale(scale);
}

void Cp_TransformLimit::ImGuiUpdate()
{
	m_posLimit.ImGuiUpdate("pos");
	m_rotaLimit.ImGuiUpdate("rotation");
	m_scaleLimit.ImGuiUpdate("scale");
}

void Cp_TransformLimit::InitJson()
{
	if (m_jsonData.is_null())return;
	m_posLimit.InitJson(m_jsonData["posLimit"]);
	m_rotaLimit.InitJson(m_jsonData["rotationLimit"]);
	m_scaleLimit.InitJson(m_jsonData["scaleLimit"]);
}

nlohmann::json Cp_TransformLimit::GetJson()
{
	m_jsonData["posLimit"]		= m_posLimit.GetJson();
	m_jsonData["rotationLimit"] = m_rotaLimit.GetJson();
	m_jsonData["scaleLimit"]	= m_scaleLimit.GetJson();
	return m_jsonData;
}

void Cp_TransformLimit::Clamp(Math::Vector3& _vec3, LimitSet _limitSet)
{
	if (_limitSet.shaft == Shaft::Null || !_limitSet.bActive)return;
	Math::Vector3 contrast = _vec3;
	if(_limitSet.bOffset) contrast += _limitSet.Offset;

	if (_limitSet.bMax)
	{
		if (_limitSet.shaft & Shaft::X)
		{
			if(contrast.x > _limitSet.Max.x)_vec3.x = _limitSet.Max.x;
		}
		
		if (_limitSet.shaft & Shaft::Y)
		{
			if (contrast.y > _limitSet.Max.y)_vec3.y = _limitSet.Max.y;
		}
		
		if (_limitSet.shaft & Shaft::Z)
		{
			if(contrast.z > _limitSet.Max.z)_vec3.z = _limitSet.Max.z;
		}
	}
	
	if (_limitSet.bMin)
	{
		if (_limitSet.shaft & Shaft::X)
		{
			if(contrast.x < _limitSet.Min.x)_vec3.x = _limitSet.Min.x;
		}
		
		if (_limitSet.shaft & Shaft::Y)
		{
			if (contrast.y < _limitSet.Min.y)_vec3.y = _limitSet.Min.y;
		}
		
		if (_limitSet.shaft & Shaft::Z)
		{
			if(contrast.z < _limitSet.Min.z)_vec3.z = _limitSet.Min.z;
		}
	}
}

void Cp_TransformLimit::LimitSet::ImGuiUpdate(std::string _name)
{
	ImGui::SeparatorText(_name.c_str());
	ImGui::Checkbox(("Active##" + _name).c_str(), &bActive);
	if (!ImGui::TreeNode(("##" + _name).c_str()))return;
	if (bActive)
	{

		ImGui::SameLine(); if (ImGui::SmallButton(("SelectShaft##" + _name).c_str()))ImGui::OpenPopup(("Shaft##" + _name).c_str());
		if (ImGui::BeginPopup(("Shaft##" + _name).c_str()))
		{
			ImGuiCheckBoxBit("X", shaft, Shaft::X);
			ImGui::SameLine(); ImGuiCheckBoxBit("Y", shaft, Shaft::Y);
			ImGui::SameLine(); ImGuiCheckBoxBit("Z", shaft, Shaft::Z);
			ImGui::EndPopup();
		}

		ImGui::Checkbox(("bMax##" + _name).c_str(), &bMax);
		if (bMax)
		{
			ImGui::SameLine();
			ImGui::DragFloat3(("##Max" + _name).c_str(), &Max.x);
		}

		ImGui::Checkbox(("bMin##" + _name).c_str(), &bMin);
		if (bMin)
		{
			ImGui::SameLine();
			ImGui::DragFloat3(("##Min" + _name).c_str(), &Min.x);
		}

		ImGui::Checkbox(("bOffset##" + _name).c_str(), &bOffset);
		if (bMin)
		{
			ImGui::SameLine();
			ImGui::DragFloat3(("##Offset" + _name).c_str(), &Offset.x);
		}

	}
	ImGui::TreePop();
}

void Cp_TransformLimit::LimitSet::InitJson(nlohmann::json _json)
{
	bActive = _json["ActiveFlg"];
	shaft	= _json["shaft"];

	bMax = _json["MaxFlg"];
	Max = JsonToVec3(_json["Max"]);

	bMin = _json["MinFlg"];
	Min = JsonToVec3(_json["Min"]);
	
	bOffset = _json["OffsetFlg"];
	Offset = JsonToVec3(_json["Offset"]);
}

nlohmann::json Cp_TransformLimit::LimitSet::GetJson()
{
	nlohmann::json json;
	json["ActiveFlg"] = bActive;
	json["shaft"] = shaft;

	json["MaxFlg"] = bMax;
	json["Max"] = Vec3ToJson(Max);

	json["MinFlg"] = bMin;
	json["Min"] = Vec3ToJson(Min);

	json["OffsetFlg"] = bOffset;
	json["Offset"] = Vec3ToJson(Offset);

	return json;
}
