#include "TransFormInherit.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"

void Cp_TransFormInherit::ImGuiUpdate()
{
	if (ImGui::Checkbox("Pos", &m_formInheritPos), m_formInheritPos)ImGui::DragFloat3("StartPos", &m_startPos.x);
	if (ImGui::Checkbox("Rota", &m_formInheritRota), m_formInheritRota)ImGui::DragFloat3("StartRota", &m_startRota.x);
	if (ImGui::Checkbox("Scale", &m_formInheritScale), m_formInheritScale)ImGui::DragFloat3("StartScale", &m_startScale.x);
}

void Cp_TransFormInherit::InitJson()
{

	m_formInheritPos = m_jsonData["FormInheritPos"];
	m_startPos = JsonToVec3(m_jsonData["StartPos"]);

	m_formInheritRota = m_jsonData["FormInheritRota"];
	m_startRota = JsonToVec3(m_jsonData["StartRota"]);

	m_formInheritScale = m_jsonData["FormInheritScale"];
	m_startScale = JsonToVec3(m_jsonData["StartScale"]);

	std::weak_ptr<Cp_Transform>mTrans = m_owner.lock()->GetTransform();
	std::weak_ptr<Cp_Transform>pTrans = mTrans.lock()->GetParent().lock();
	Math::Vector3 pos = m_startPos;
	Math::Vector3 rota = m_startRota;
	Math::Vector3 scale = m_startScale;

	if ( pTrans.lock())
	{
		pos = (Math::Matrix::CreateTranslation(m_startPos) * pTrans.lock()->GetMatrix()).Translation();
		rota += pTrans.lock()->GetRotation(true);
		scale = m_startScale * pTrans.lock()->GetScale(true);
	}

	if (m_formInheritPos)mTrans.lock()->SetPosition(pos);
	if (m_formInheritRota)mTrans.lock()->SetRotation(rota);
	if (m_formInheritScale)mTrans.lock()->SetScale(scale);

	mTrans.lock()->UnFollow();
}

nlohmann::json Cp_TransFormInherit::GetJson()
{
	m_jsonData["FormInheritPos"] = m_formInheritPos;
	m_jsonData["StartPos"] = Vec3ToJson(m_startPos);

	m_jsonData["FormInheritRota"] = m_formInheritRota;
	m_jsonData["StartRota"] = Vec3ToJson(m_startRota);

	m_jsonData["FormInheritScale"] = m_formInheritScale;
	m_jsonData["StartScale"] = Vec3ToJson(m_startScale);
	return m_jsonData;
}
