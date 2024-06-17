﻿#include "Transform.h"
#include "../../Game/GameObject.h"

void Cp_Transform::Start()
{
	std::weak_ptr<GameObject>parent = m_owner.lock()->GetParent();
	if (parent.expired())return;
	SetParent(parent.lock()->GetTransform());
}

void Cp_Transform::ImGuiUpdate()
{
	ImGui::DragFloat3("Position", &m_position.x);
	ImGui::DragFloat3("Rotation", &m_rotation.x);
	ImGui::DragFloat3("Scale", &m_scale.x);


	ImGui::InputText("myMatTag", &m_myMatTag);
	if (m_parent.lock())ImGui::InputText("parentMatTag", &m_parentMatTag);

	if (!ImGui::TreeNode("Property"))return;
	{
		Math::Vector3 vec = GetMatrix().Translation();
		ImGui::InputFloat3("Mat", &vec.x);

		ImGui::Checkbox("Follow", &m_bFollow);
	}
	ImGui::TreePop();
}

void Cp_Transform::InitJson()
{
	m_position = JsonToVec3(m_jsonData["position"]);
	m_rotation = JsonToVec3(m_jsonData["rotation"]);
	m_scale = JsonToVec3(m_jsonData["scale"]);

	m_myMatTag = m_jsonData["matTag"];
	m_parentMatTag = m_jsonData["parentMatTag"];
	m_bFollow = m_jsonData["Follow"];
}
nlohmann::json Cp_Transform::GetJson()
{
	m_jsonData["position"] = Vec3ToJson(m_position);
	m_jsonData["rotation"] = Vec3ToJson(m_rotation);
	m_jsonData["scale"] = Vec3ToJson(m_scale);

	m_jsonData["matTag"] = m_myMatTag;
	m_jsonData["parentMatTag"] = m_parentMatTag;

	m_jsonData["Follow"] = m_bFollow;
	return m_jsonData;
}

Math::Vector3 Cp_Transform::GetPosition(bool _PushFollow) const
{
	if (m_parent.lock())
	{
		if (m_bFollow || _PushFollow)
		{
			return m_position + m_parent.lock()->GetPosition();
		}
	}
	return m_position;
}

Math::Vector3 Cp_Transform::GetRotation(bool _PushFollow) const
{
	if (m_parent.lock())
	{
		if (m_bFollow || _PushFollow)
		{
			return m_rotation + m_parent.lock()->GetRotation();
		}
	}
	return m_rotation;
}

Math::Vector3 Cp_Transform::GetScale(bool _PushFollow) const
{
	if (m_parent.lock())
	{
		if (m_bFollow || _PushFollow)
		{
			return m_scale * m_parent.lock()->GetScale();
		}
	}
	return m_scale;
}

Math::Matrix Cp_Transform::GetMatrix(std::string _matTag, bool _PushFollow)
{
	std::string::iterator it = (_matTag.empty() ? m_myMatTag : _matTag).begin();
	std::string::iterator end = (_matTag.empty() ? m_myMatTag : _matTag).end();

	auto ReturnMat = [&]()
		{
			m_mWorld = GetSMat() * GetRMat() * GetTMat();
			if (m_parent.lock() && (m_bFollow || _PushFollow))return m_mWorld * m_parent.lock()->GetMatrix(m_parentMatTag);
			return m_mWorld;
		};

	if (it == end) return ReturnMat();

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
		return m_mWorld = ReturnMat();
	}

	it++;
	while (it != end)
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
			return ReturnMat();
		}
		it++;
	}

	if (m_parent.lock() && (m_bFollow || _PushFollow))return m_mWorld * m_parent.lock()->GetMatrix(m_parentMatTag);
	return m_mWorld;
}

Math::Matrix Cp_Transform::GetMatrix(bool _PushFollow)
{
	return GetMatrix(std::string(), _PushFollow);
}

Math::Matrix Cp_Transform::GetTMat() const
{
	Math::Matrix mat(Math::Matrix::CreateTranslation(m_position));
	return mat;
}

Math::Matrix Cp_Transform::GetSMat() const
{
	return Math::Matrix::CreateScale(m_scale);
}

Math::Matrix Cp_Transform::GetRMat(UINT _shafts)
{
	if (_shafts == 0)
		return
		Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_rotation.x))
		* Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotation.y))
		* Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_rotation.z));

	Math::Matrix rMat;
	if (_shafts & Shaft::X)rMat *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_rotation.x));
	if (_shafts & Shaft::Y)rMat *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotation.y));
	if (_shafts & Shaft::Z)rMat *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_rotation.z));
	return rMat;
}
