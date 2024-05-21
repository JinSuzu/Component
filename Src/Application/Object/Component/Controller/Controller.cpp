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
	Math::Vector3 vec = Math::Vector3::Zero;
	vec.z += (GetAsyncKeyState('W') & 0x8000);
	vec.z -= (GetAsyncKeyState('S') & 0x8000);
	vec.x += (GetAsyncKeyState('D') & 0x8000);
	vec.x -= (GetAsyncKeyState('A') & 0x8000);
	vec.Normalize();
	Math::Matrix vecMat = Math::Matrix::CreateTranslation(vec);

	Math::Vector3 deg = m_owner.lock()->GetTransform().lock()->GetRotation();
	if (m_shaft & RotationShaft::X)vecMat *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(deg.x));
	if (m_shaft & RotationShaft::Y)vecMat *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(deg.y));
	if (m_shaft & RotationShaft::Z)vecMat *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(deg.z));
	
	m_rigitbody.lock()->SetMove(vecMat.Translation() * m_movePow);
}

void Cp_Controller::ImGuiUpdate()
{
	if (ImGui::Button("RotationShaft"))ImGui::OpenPopup("Shafts");
	ImGui::DragFloat("movePow", &m_movePow);
	if (ImGui::BeginPopup("Shafts"))
	{
		ImGuiCheckBoxBit("X", m_shaft, RotationShaft::X);
		ImGuiCheckBoxBit("Y", m_shaft, RotationShaft::Y);
		ImGuiCheckBoxBit("Z", m_shaft, RotationShaft::Z);
		ImGui::EndPopup();
	}
}

void Cp_Controller::InitJson()
{
	if (!m_jsonData.is_null())
	{
		m_shaft = m_jsonData["RotationShaft"];
		m_movePow = m_jsonData["movePow"];
	}
}

nlohmann::json Cp_Controller::GetJson()
{
	m_jsonData["RotationShaft"] = m_shaft;
	m_jsonData["movePow"] = m_movePow;
	return m_jsonData;
}
