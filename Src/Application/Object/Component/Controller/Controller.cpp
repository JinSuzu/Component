#include "Controller.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"
#include "../Rigidbody/Rigidbody.h"
#include "../Rigidbody/Rigidbody.h"
#include "../../../ImGuiHelper/ImGuiHelper.h"

void Cp_Controller::Start()
{
	m_rigitbody = m_owner.lock()->GetComponent<Cp_Rigidbody>();
	assert(m_rigitbody.lock() && "rigitbodyが取れなかった！byコントローラー");
}

void Cp_Controller::PreUpdateContents()
{
	if (m_rigitbody.expired())
	{
		Destroy();
		return;
	}

	//視点を前としたベクトルを取る
	Math::Vector3 vec = Math::Vector3::Zero;
	Math::Matrix mat = m_owner.lock()->GetTransform().lock()->GetRMat(m_shaft);
	if (GetAsyncKeyState('W') & 0x8000)vec += mat.Backward();
	if (GetAsyncKeyState('S') & 0x8000)vec += mat.Forward();
	if (GetAsyncKeyState('D') & 0x8000)vec += mat.Right();
	if (GetAsyncKeyState('A') & 0x8000)vec += mat.Left();

	m_rigitbody.lock()->AddMove(vec * m_movePow);
}

void Cp_Controller::ImGuiUpdate()
{
	if (ImGui::Button("shaft"))ImGui::OpenPopup("MShafts");
	if (ImGui::BeginPopup("MShafts"))
	{
		MyImGui::CheckBoxAllBit<Shaft>(m_shaft);
		ImGui::EndPopup();
	}

	ImGui::DragFloat("movePow", &m_movePow);
}

void Cp_Controller::LoadJson(nlohmann::json _json)
{
	m_shaft = _json["shaft"];
	m_movePow = _json["movePow"];
}

nlohmann::json Cp_Controller::GetJson()
{
	nlohmann::json json;
	json["shaft"] = m_shaft;
	json["movePow"] = m_movePow;
	return json;
}
