#include "TransFormInherit.h"
#include "../../Game/GameObject.h"

#include "../Camera/Camera.h"
#include "../../../RenderManger/RenderManger.h"
#include "../Transform/Transform.h"

void Cp_TransFormInherit::Start()
{
	m_activePos = Shaft::X | Shaft::Y | Shaft::Z;
	m_activeRota = Shaft::X | Shaft::Y | Shaft::Z;
	m_activeScale = Shaft::X | Shaft::Y | Shaft::Z;
}

void Cp_TransFormInherit::ImGuiUpdate()
{
	if (ImGui::Checkbox("Pos", &m_formInheritPos), m_formInheritPos)
	{
		ImGui::DragFloat3("StartPos", &m_startPos.x);
		if (ImGui::Button("ActivePos"))ImGui::OpenPopup("ActivePos");
		if (ImGui::BeginPopup("ActivePos"))
		{
			ImGuiCheckBoxBit("X", m_activePos, Shaft::X);
			ImGuiCheckBoxBit("Y", m_activePos, Shaft::Y);
			ImGuiCheckBoxBit("Z", m_activePos, Shaft::Z);
			ImGui::EndPopup();
		}
	}
	if (ImGui::Checkbox("Rota", &m_formInheritRota), m_formInheritRota)
	{
		ImGui::DragFloat3("StartRota", &m_startRota.x);
		if (ImGui::Button("ActiveRota"))ImGui::OpenPopup("ActiveRota");
		if (ImGui::BeginPopup("ActiveRota"))
		{
			ImGuiCheckBoxBit("X", m_activeRota, Shaft::X);
			ImGuiCheckBoxBit("Y", m_activeRota, Shaft::Y);
			ImGuiCheckBoxBit("Z", m_activeRota, Shaft::Z);
			ImGui::EndPopup();
		}
	}
	if (ImGui::Checkbox("Scale", &m_formInheritScale), m_formInheritScale)
	{
		ImGui::DragFloat3("StartScale", &m_startScale.x);
		if (ImGui::Button("ActiveScale"))ImGui::OpenPopup("ActiveScale");
		if (ImGui::BeginPopup("ActiveScale"))
		{
			ImGuiCheckBoxBit("X", m_activeScale, Shaft::X);
			ImGuiCheckBoxBit("Y", m_activeScale, Shaft::Y);
			ImGuiCheckBoxBit("Z", m_activeScale, Shaft::Z);
			ImGui::EndPopup();
		}
	}
}

void Cp_TransFormInherit::InitJson()
{

	m_formInheritPos = m_jsonData["FormInheritPos"];
	if (m_jsonData["ActivePos"].is_number())  m_activePos = m_jsonData["ActivePos"];
	m_startPos = JsonToVec3(m_jsonData["StartPos"]);

	m_formInheritRota = m_jsonData["FormInheritRota"];
	if (m_jsonData["CameraFormInheritRota"].is_boolean())m_cameraFormInherit = m_jsonData["CameraFormInheritRota"];
	if (m_jsonData["ActiveRota"].is_number()) m_activeRota = m_jsonData["ActiveRota"];
	m_startRota = JsonToVec3(m_jsonData["StartRota"]);

	m_formInheritScale = m_jsonData["FormInheritScale"];
	if (m_jsonData["ActiveScale"].is_number())m_activeScale = m_jsonData["ActiveScale"];
	m_startScale = JsonToVec3(m_jsonData["StartScale"]);


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
			if (!(add & Shaft::X))pos.x = 0.0f;
			if (!(add & Shaft::Y))pos.y = 0.0f;
			if (!(add & Shaft::Z))pos.z = 0.0f;
			return pos;
		};

	if (m_formInheritPos)mTrans.lock()->SetPosition(AddCheck(m_activePos, pos));
	if (m_formInheritRota)mTrans.lock()->SetRotation(m_cameraFormInherit ? RenderManager::Instance().GetCamera().lock()->GetOwner().lock()->GetTransform().lock()->GetRotation() : AddCheck(m_activeRota, rota));
	if (m_formInheritScale)mTrans.lock()->SetScale(AddCheck(m_activeScale, scale));

	mTrans.lock()->UnFollow();
}

nlohmann::json Cp_TransFormInherit::GetJson()
{
	m_jsonData["FormInheritPos"] = m_formInheritPos;
	m_jsonData["ActivePos"] = m_activePos;
	m_jsonData["StartPos"] = Vec3ToJson(m_startPos);

	m_jsonData["FormInheritRota"] = m_formInheritRota;
	m_jsonData["CameraFormInheritRota"] = m_cameraFormInherit;
	m_jsonData["ActiveRota"] = m_activeRota;
	m_jsonData["StartRota"] = Vec3ToJson(m_startRota);

	m_jsonData["FormInheritScale"] = m_formInheritScale;
	m_jsonData["ActiveScale"] = m_activeScale;
	m_jsonData["StartScale"] = Vec3ToJson(m_startScale);

	return m_jsonData;
}
