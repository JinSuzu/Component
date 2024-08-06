#include "TransFormInheritComponent.h"
#include "../../../Game/GameObject.h"

void TransFormInheritComponent::Start()
{
	m_activePos = (UINT)Shaft::X | (UINT)Shaft::Y | (UINT)Shaft::Z;
	m_activeRota = (UINT)Shaft::X | (UINT)Shaft::Y | (UINT)Shaft::Z;
	m_activeScale = (UINT)Shaft::X | (UINT)Shaft::Y | (UINT)Shaft::Z;
}

void TransFormInheritComponent::ImGuiUpdate()
{
	if (ImGui::Checkbox("Pos", &m_formInheritPos), m_formInheritPos)
	{
		ImGui::DragFloat3("StartPos", &m_startPos.x);
		if (ImGui::Button("ActivePos"))ImGui::OpenPopup("ActivePos");
		if (ImGui::BeginPopup("ActivePos"))
		{
			Utility::ImGuiHelper::CheckBoxAllBit<Shaft>(m_activePos);
			ImGui::EndPopup();
		}
	}
	if (ImGui::Checkbox("Rota", &m_formInheritRota), m_formInheritRota)
	{
		ImGui::DragFloat3("StartRota", &m_startRota.x);
		if (ImGui::Button("ActiveRota"))ImGui::OpenPopup("ActiveRota");
		if (ImGui::BeginPopup("ActiveRota"))
		{
			Utility::ImGuiHelper::CheckBoxAllBit< Shaft>(m_activeRota);
			ImGui::EndPopup();
		}
	}
	if (ImGui::Checkbox("Scale", &m_formInheritScale), m_formInheritScale)
	{
		ImGui::DragFloat3("StartScale", &m_startScale.x);
		if (ImGui::Button("ActiveScale"))ImGui::OpenPopup("ActiveScale");
		if (ImGui::BeginPopup("ActiveScale"))
		{
			Utility::ImGuiHelper::CheckBoxAllBit<Shaft>(m_activeRota);
			ImGui::EndPopup();
		}
	}
}

void TransFormInheritComponent::LoadJson(nlohmann::json _json)
{
	m_formInheritPos = _json["FormInheritPos"];
	if (_json["ActivePos"].is_number())  m_activePos = _json["ActivePos"];
	m_startPos = Utility::JsonHelper::InPutVec3(_json["StartPos"]);

	m_formInheritRota = _json["FormInheritRota"];
	if (_json["CameraFormInheritRota"].is_boolean())m_cameraFormInherit = _json["CameraFormInheritRota"];
	if (_json["ActiveRota"].is_number()) m_activeRota = _json["ActiveRota"];
	m_startRota = Utility::JsonHelper::InPutVec3(_json["StartRota"]);

	m_formInheritScale = _json["FormInheritScale"];
	if (_json["ActiveScale"].is_number())m_activeScale = _json["ActiveScale"];
	m_startScale = Utility::JsonHelper::InPutVec3(_json["StartScale"]);


	std::weak_ptr<TransformComponent>mTrans = m_owner.lock()->GetTransform();
	Math::Vector3 pos	= m_startPos + mTrans.lock()->GetWorldPosition();
	Math::Vector3 rota	= m_startRota + mTrans.lock()->GetWorldRotation();
	Math::Vector3 scale = m_startScale * mTrans.lock()->GetWorldScale();

	auto AddCheck = [](UINT add, Math::Vector3 pos)
		{
			if (!(add & (UINT)Shaft::X))pos.x = 0.0f;
			if (!(add & (UINT)Shaft::Y))pos.y = 0.0f;
			if (!(add & (UINT)Shaft::Z))pos.z = 0.0f;
			return pos;
		};

	if (m_formInheritPos)mTrans.lock()->SetPosition(AddCheck(m_activePos, pos));
	if (m_formInheritRota)mTrans.lock()->SetRotation(m_cameraFormInherit ? CameraManager::Instance().GetCamera().lock()->GetCameraMatrix().ToEuler() : AddCheck(m_activeRota, rota));
	if (m_formInheritScale)mTrans.lock()->SetScale(AddCheck(m_activeScale, scale));

	mTrans.lock()->UnFollow();
}

nlohmann::json TransFormInheritComponent::GetJson()
{
	nlohmann::json json;
	json["FormInheritPos"] = m_formInheritPos;
	json["ActivePos"] = m_activePos;
	json["StartPos"] = Utility::JsonHelper::OutPutVec3(m_startPos);

	json["FormInheritRota"] = m_formInheritRota;
	json["CameraFormInheritRota"] = m_cameraFormInherit;
	json["ActiveRota"] = m_activeRota;
	json["StartRota"] = Utility::JsonHelper::OutPutVec3(m_startRota);

	json["FormInheritScale"] = m_formInheritScale;
	json["ActiveScale"] = m_activeScale;
	json["StartScale"] = Utility::JsonHelper::OutPutVec3(m_startScale);

	return json;
}
