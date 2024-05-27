#include "Controller.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"
#include "../Rigidbody/Rigidbody.h"

void Cp_Controller::Start()
{
	m_rigitbody = m_owner.lock()->GetComponent<Cp_Rigidbody>();
	assert(m_rigitbody.lock() && "RigidBodyが見つかりませんでした");
}

void Cp_Controller::PreUpdateContents()
{
	//視点を前にしたベクトルを取る
	Math::Matrix mat = m_owner.lock()->GetTransform().lock()->GetMatrix();
	Math::Vector3 vec = Math::Vector3::Zero;
	if(GetAsyncKeyState('W') & 0x8000)vec += mat.Backward();	
	if(GetAsyncKeyState('S') & 0x8000)vec += mat.Forward();
	if(GetAsyncKeyState('D') & 0x8000)vec += mat.Right();
	if(GetAsyncKeyState('A') & 0x8000)vec += mat.Left();

	vec.y += GetAsyncKeyState(VK_SPACE) & 0x80f00;
	vec.y -= GetAsyncKeyState(VK_SHIFT) & 0x8000;
	
	vec.x *= m_moveShaft & Shaft::X;
	vec.y *= m_moveShaft & Shaft::Y;
	vec.z *= m_moveShaft & Shaft::Z;

	vec.Normalize();

	m_rigitbody.lock()->SetMove(vec * m_movePow);
}

void Cp_Controller::ImGuiUpdate()
{
	if (ImGui::Button("MoveShaft"))ImGui::OpenPopup("MShafts");
	if (ImGui::BeginPopup("MShafts"))
	{
		ImGuiCheckBoxBit("X", m_moveShaft, Shaft::X);
		ImGuiCheckBoxBit("Y", m_moveShaft, Shaft::Y);
		ImGuiCheckBoxBit("Z", m_moveShaft, Shaft::Z);
		ImGui::EndPopup();
	}
	
	if (ImGui::SameLine(); ImGui::Button("RotationShaft"))ImGui::OpenPopup("RShafts");
	if (ImGui::BeginPopup("RShafts"))
	{
		ImGuiCheckBoxBit("X", m_rotationShaft, Shaft::X);
		ImGuiCheckBoxBit("Y", m_rotationShaft, Shaft::Y);
		ImGuiCheckBoxBit("Z", m_rotationShaft, Shaft::Z);
		ImGui::EndPopup();
	}

	ImGui::DragFloat("movePow", &m_movePow);
}

void Cp_Controller::InitJson()
{
	if (!m_jsonData.is_null())
	{
		m_rotationShaft = m_jsonData["RotationShaft"];
		m_movePow = m_jsonData["movePow"];
	}
}

nlohmann::json Cp_Controller::GetJson()
{
	m_jsonData["RotationShaft"] = m_rotationShaft;
	m_jsonData["movePow"] = m_movePow;
	return m_jsonData;
}
