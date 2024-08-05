#include "TransformLimitComponent.h"
#include "../../../Game/GameObject.h"

void TransformLimitComponent::PostUpdateContents()
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

void TransformLimitComponent::ImGuiUpdate()
{
	m_posLimit.ImGuiUpdate("pos");
	m_rotaLimit.ImGuiUpdate("rotation");
	m_scaleLimit.ImGuiUpdate("scale");
}

void TransformLimitComponent::LoadJson(nlohmann::json _json)
{
	if (_json.is_null())return;
	m_posLimit.InitJson(_json["posLimit"]);
	m_rotaLimit.InitJson(_json["rotationLimit"]);
	m_scaleLimit.InitJson(_json["scaleLimit"]);
}

nlohmann::json TransformLimitComponent::GetJson()
{
	nlohmann::json json;
	json["posLimit"]		= m_posLimit.GetJson();
	json["rotationLimit"] = m_rotaLimit.GetJson();
	json["scaleLimit"]	= m_scaleLimit.GetJson();
	return json;
}

void TransformLimitComponent::Clamp(Math::Vector3& _vec3, LimitSet _limitSet)
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

void TransformLimitComponent::LimitSet::ImGuiUpdate(std::string _name)
{
	ImGui::SeparatorText(_name.c_str());
	ImGui::Checkbox(("Active##" + _name).c_str(), &bActive);
	if (!ImGui::TreeNode(("##" + _name).c_str()))return;
	if (bActive)
	{

		ImGui::SameLine(); if (ImGui::SmallButton(("SelectShaft##" + _name).c_str()))ImGui::OpenPopup(("Shaft##" + _name).c_str());
		if (ImGui::BeginPopup(("Shaft##" + _name).c_str()))
		{
			Utility::ImGuiHelper::CheckBoxAllBit<Shaft>(shaft);
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

void TransformLimitComponent::LimitSet::InitJson(nlohmann::json _json)
{
	bActive = _json["ActiveFlg"];
	shaft	= _json["shaft"];

	bMax	= _json["MaxFlg"];
	Max		= Utility::JsonHelper::InPutVec3(_json["Max"]);

	bMin	= _json["MinFlg"];
	Min		= Utility::JsonHelper::InPutVec3(_json["Min"]);
	
	bOffset = _json["OffsetFlg"];
	Offset	= Utility::JsonHelper::InPutVec3(_json["Offset"]);
}

nlohmann::json TransformLimitComponent::LimitSet::GetJson()
{
	nlohmann::json json;
	json["ActiveFlg"]	= bActive;
	json["shaft"]		= shaft;

	json["MaxFlg"]		= bMax;
	json["Max"]			= Utility::JsonHelper::OutPutVec3(Max);

	json["MinFlg"]		= bMin;
	json["Min"]			= Utility::JsonHelper::OutPutVec3(Min);

	json["OffsetFlg"]	= bOffset;
	json["Offset"]		= Utility::JsonHelper::OutPutVec3(Offset);

	return json;
}
