#include "Transform.h"
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
	m_myMatTag = m_jsonData["matTag"];
	m_parentMatTag = m_jsonData["parentMatTag"];
	m_bFollow = m_jsonData["Follow"];

	m_position = MyJson::InPutVec3(m_jsonData["position"]);
	m_rotation = MyJson::InPutVec3(m_jsonData["rotation"]);
	m_scale = MyJson::InPutVec3(m_jsonData["scale"]);
}
nlohmann::json Cp_Transform::GetJson()
{
	m_jsonData["position"] = MyJson::OutPutVec3(m_position);
	m_jsonData["rotation"] = MyJson::OutPutVec3(m_rotation);
	m_jsonData["scale"] = MyJson::OutPutVec3(m_scale);

	m_jsonData["matTag"] = m_myMatTag;
	m_jsonData["parentMatTag"] = m_parentMatTag;

	m_jsonData["Follow"] = m_bFollow;
	return m_jsonData;
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

Math::Matrix Cp_Transform::GetMatrix(std::string _matTag, bool _PushFollow
	,const Math::Matrix& _offsetT, const Math::Matrix& _offsetR, const Math::Matrix& _offsetS)
{
	std::string::iterator it = (_matTag.empty() ? m_myMatTag : _matTag).begin();
	std::string::iterator end = (_matTag.empty() ? m_myMatTag : _matTag).end();

	auto ReturnMat = [&]()
		{
			m_mWorld = GetSMat() * _offsetS * GetRMat() * _offsetR * GetTMat() * _offsetT;
			if (m_parent.lock() && (m_bFollow || _PushFollow))return m_mWorld * m_parent.lock()->GetMatrix(m_parentMatTag);
			return m_mWorld;
		};

	if (it == end) return ReturnMat();

	switch (*it)
	{
	case 'T':
		m_mWorld = GetTMat() * _offsetT;
		break;
	case 'R':
		m_mWorld = GetRMat() * _offsetR;
		break;
	case 'S':
		m_mWorld = GetSMat() * _offsetS;
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
			m_mWorld *= GetTMat() * _offsetT;
			break;
		case 'R':
			m_mWorld *= GetRMat() * _offsetR;
			break;
		case 'S':
			m_mWorld *= GetSMat() * _offsetS;
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

Math::Matrix Cp_Transform::GetMatrix(const Math::Matrix& _offsetT, const Math::Matrix& _offsetR, const Math::Matrix& _offsetS)
{
	return GetMatrix(std::string(), false, _offsetT, _offsetR, _offsetS);
}

Math::Matrix Cp_Transform::GetRMat(UINT _shafts)
{
	if (_shafts == 0)
		return
		Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_rotation.x))
		* Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotation.y))
		* Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_rotation.z));

	Math::Matrix rMat;
	if (_shafts & (UINT)Shaft::X)rMat *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_rotation.x));
	if (_shafts & (UINT)Shaft::Y)rMat *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotation.y));
	if (_shafts & (UINT)Shaft::Z)rMat *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_rotation.z));
	return rMat;
}
