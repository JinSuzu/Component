#include "Transform.h"
#include "../../Game/GameObject.h"

void Cp_Transform::InitJson()
{
	Component::InitJson();
	if (!m_jsonData.is_null())
	{
		m_position = JsonToVec3(m_jsonData["position"]);
		m_rotation = JsonToVec3(m_jsonData["rotation"]);
		m_scale = JsonToVec3(m_jsonData["scale"]);

		m_matTag = m_jsonData["matTag"];
	}
}

void Cp_Transform::ImGuiUpdate()
{
	ImGui::DragFloat3("Position", &m_position.x);
	ImGui::DragFloat3("Rotation", &m_rotation.x);	
	ImGui::DragFloat3("Scale"	, &m_scale.x);

	static char matTag[50] = "";
	ImGui::InputText("matTag", matTag, sizeof(matTag));
	ImGui::SameLine(); if (ImGui::Button("Set"))m_matTag = matTag;
}

nlohmann::json Cp_Transform::GetJson()
{
	m_jsonData["position"]	= Vec3ToJson(m_position);
	m_jsonData["rotation"]	= Vec3ToJson(m_rotation);
	m_jsonData["scale"]		= Vec3ToJson(m_scale);

	m_jsonData["matTag"]	= m_matTag;
	return m_jsonData;
}

Math::Matrix Cp_Transform::GetMatrix()
{
	auto it = m_matTag.begin();

	switch (*it)
	{
	case 'T':
		m_mWorld = GetTMat();
		break;
	case 'R':
		m_mWorld = GetRMat();
		break;
	case 'S':
		m_mWorld = GetSMat();
		break;
	default:
		return GetMatrix();
	}

	it++;

	while (it != m_matTag.end())
	{
		switch (*it)
		{
		case 'T':
			m_mWorld *= GetTMat();
			break;
		case 'R':
			m_mWorld *= GetRMat();
			break;
		case 'S':
			m_mWorld *= GetSMat();
			break;
		default:
			return GetMatrix();
		}
		it++;
	}

	if(m_parent.expired())return m_mWorld;
	return m_mWorld * m_parent.lock()->GetMatrix();
}

Math::Matrix Cp_Transform::GetTMat()
{
	Math::Matrix mat(Math::Matrix::CreateTranslation(m_position));
	return mat;
}

Math::Matrix Cp_Transform::GetSMat()
{
	return Math::Matrix::CreateScale(m_scale);
}

Math::Matrix Cp_Transform::GetRMat()
{
	return 
		Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_rotation.x))
		* Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotation.y))
		* Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_rotation.z));
}
