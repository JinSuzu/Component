#include "Transform.h"
#include "../../Object/Object.h"

void Cp_Transform::Start(std::weak_ptr<Object> _owner)
{
	Component::Start(_owner);
	if (Json.is_object())
	{
		m_position = JsonToVec3(Json["position"]);
		m_rotation = JsonToVec3(Json["rotation"]);
		m_scale = JsonToVec3(Json["scale"]);
	}
}
void Cp_Transform::Update()
{
}
void Cp_Transform::ImGuiUpdate()
{
	float vol1[3]TOARRAY3(m_position);
	ImGui::DragFloat3("Position", vol1);
	m_position = Math::Vector3(vol1);

	float vol2[3]TOARRAY3(m_rotation);
	ImGui::DragFloat3("Rotation", vol2);
	m_rotation = Math::Vector3(vol2);
	
	float vol3[3]TOARRAY3(m_scale);
	ImGui::DragFloat3("Scale", vol3);
	m_scale = Math::Vector3(vol3);
}

nlohmann::json Cp_Transform::GetJson()
{
	Json["position"]	= Vec3ToJson(m_position);
	Json["rotation"]	= Vec3ToJson(m_rotation);
	Json["scale"]		= Vec3ToJson(m_scale);

	return Json;
}

Math::Matrix Cp_Transform::GetMatrix(std::string _tag)
{
	Math::Matrix mat;
	auto it = _tag.begin();

	switch (*it)
	{
	case 'T':
		mat = GetTMat();
		break;
	case 'R':
		mat = GetRMat();
		break;
	case 'S':
		mat = GetSMat();
		break;
	default:
		return GetMatrix();
	}

	it++;

	while (it != _tag.end())
	{
		switch (*it)
		{
		case 'T':
			mat *= GetTMat();
			break;
		case 'R':
			mat *= GetRMat();
			break;
		case 'S':
			mat *= GetSMat();
			break;
		default:
			return GetMatrix();
		}
		it++;
	}
	return mat;
}

Math::Matrix Cp_Transform::GetMatrix()
{
	Math::Matrix mat(
		Math::Matrix::CreateScale(m_scale)
		*Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_rotation.x))
		* Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotation.y))
		* Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_rotation.z))
		* Math::Matrix::CreateTranslation(m_position)
	);
	mat._43 = 0;
	return mat;
}

Math::Matrix Cp_Transform::GetTMat()
{
	Math::Matrix mat(Math::Matrix::CreateTranslation(m_position));
	mat._43 = 0;
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
