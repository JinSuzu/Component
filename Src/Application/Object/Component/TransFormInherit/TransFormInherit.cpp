#include "TransFormInherit.h"
#include "../../Game/GameObject.h"

#include "../Camera/Camera.h"
#include "../../../RenderManger/RenderManger.h"
#include "../Transform/Transform.h"

#include "../../../ImGuiHelper/ImGuiHelper.h"

void Cp_TransFormInherit::Start()
{
	m_activePos = (UINT)Shaft::X | (UINT)Shaft::Y | (UINT)Shaft::Z;
	m_activeRota = (UINT)Shaft::X | (UINT)Shaft::Y | (UINT)Shaft::Z;
	m_activeScale = (UINT)Shaft::X | (UINT)Shaft::Y | (UINT)Shaft::Z;
}

void Cp_TransFormInherit::ImGuiUpdate()
{
	if (ImGui::Checkbox("Pos", &m_formInheritPos), m_formInheritPos)
	{
		ImGui::DragFloat3("StartPos", &m_startPos.x);
		if (ImGui::Button("ActivePos"))ImGui::OpenPopup("ActivePos");
		if (ImGui::BeginPopup("ActivePos"))
		{
			MyImGui::CheckBoxAllBit<Shaft>(m_activePos);
			ImGui::EndPopup();
		}
	}
	if (ImGui::Checkbox("Rota", &m_formInheritRota), m_formInheritRota)
	{
		ImGui::DragFloat3("StartRota", &m_startRota.x);
		if (ImGui::Button("ActiveRota"))ImGui::OpenPopup("ActiveRota");
		if (ImGui::BeginPopup("ActiveRota"))
		{
			MyImGui::CheckBoxAllBit< Shaft>(m_activeRota);
			ImGui::EndPopup();
		}
	}
	if (ImGui::Checkbox("Scale", &m_formInheritScale), m_formInheritScale)
	{
		ImGui::DragFloat3("StartScale", &m_startScale.x);
		if (ImGui::Button("ActiveScale"))ImGui::OpenPopup("ActiveScale");
		if (ImGui::BeginPopup("ActiveScale"))
		{
			MyImGui::CheckBoxAllBit<Shaft>(m_activeRota);
			ImGui::EndPopup();
		}
	}
}

void Cp_TransFormInherit::LoadJson(nlohmann::json _json)
{
	m_formInheritPos = _json["FormInheritPos"];
	if (_json["ActivePos"].is_number())  m_activePos = _json["ActivePos"];
	m_startPos = MyJson::InPutVec3(_json["StartPos"]);

	m_formInheritRota = _json["FormInheritRota"];
	if (_json["CameraFormInheritRota"].is_boolean())m_cameraFormInherit = _json["CameraFormInheritRota"];
	if (_json["ActiveRota"].is_number()) m_activeRota = _json["ActiveRota"];
	m_startRota = MyJson::InPutVec3(_json["StartRota"]);

	m_formInheritScale = _json["FormInheritScale"];
	if (_json["ActiveScale"].is_number())m_activeScale = _json["ActiveScale"];
	m_startScale = MyJson::InPutVec3(_json["StartScale"]);


	std::weak_ptr<Cp_Transform>mTrans = m_owner.lock()->GetTransform();
	std::weak_ptr<Cp_Transform>pTrans = mTrans.lock()->GetParent();
	Math::Vector3 pos = m_startPos;
	Math::Vector3 rota = m_startRota;
	Math::Vector3 scale = m_startScale;

	if (pTrans.lock())
	{
		pos = (Math::Matrix::CreateTranslation(m_startPos) * pTrans.lock()->GetMatrix()).Translation();
		rota += pTrans.lock()->GetRotation();
		scale = m_startScale * pTrans.lock()->GetScale();
	}

	auto AddCheck = [](UINT add, Math::Vector3 pos)
		{
			if (!(add & (UINT)Shaft::X))pos.x = 0.0f;
			if (!(add & (UINT)Shaft::Y))pos.y = 0.0f;
			if (!(add & (UINT)Shaft::Z))pos.z = 0.0f;
			return pos;
		};

	if (m_formInheritPos)mTrans.lock()->SetPosition(AddCheck(m_activePos, pos));
	if (m_formInheritRota)mTrans.lock()->SetRotation(m_cameraFormInherit ? RenderManager::Instance().GetCamera().lock()->GetOwner().lock()->GetTransform().lock()->GetRotation() : AddCheck(m_activeRota, rota));
	if (m_formInheritScale)mTrans.lock()->SetScale(AddCheck(m_activeScale, scale));

	mTrans.lock()->UnFollow();
}

nlohmann::json Cp_TransFormInherit::GetJson()
{
	nlohmann::json json;
	json["FormInheritPos"] = m_formInheritPos;
	json["ActivePos"] = m_activePos;
	json["StartPos"] = MyJson::OutPutVec3(m_startPos);

	json["FormInheritRota"] = m_formInheritRota;
	json["CameraFormInheritRota"] = m_cameraFormInherit;
	json["ActiveRota"] = m_activeRota;
	json["StartRota"] = MyJson::OutPutVec3(m_startRota);

	json["FormInheritScale"] = m_formInheritScale;
	json["ActiveScale"] = m_activeScale;
	json["StartScale"] = MyJson::OutPutVec3(m_startScale);

	return json;
}
