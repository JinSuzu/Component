#include "Transform.h"
#include "../../Game/GameObject.h"

void Cp_Transform::ImGuiUpdate()
{
	ImGui::DragFloat3("Position", &m_position.x);
	ImGui::DragFloat3("Rotation", &m_rotation.x);	
	ImGui::DragFloat3("Scale"	, &m_scale.x);

;
	ImGui::InputText("myMatTag", &m_myMatTag);
	if(m_parent.lock())ImGui::InputText("parentMatTag", &m_parentMatTag);

	if (!ImGui::TreeNode("Property"))return;
	{
		Math::Vector3 vec = GetMatrix().Translation();
		ImGui::InputFloat3("Mat",&vec.x);
	}
	ImGui::TreePop();
}

void Cp_Transform::InitJson()
{
	Component::InitJson();
	if (!m_jsonData.is_null())
	{
		m_position = JsonToVec3(m_jsonData["position"]);
		m_rotation = JsonToVec3(m_jsonData["rotation"]);
		m_scale = JsonToVec3(m_jsonData["scale"]);

		m_myMatTag = m_jsonData["matTag"];
		m_parentMatTag = m_jsonData["parentMatTag"];
	}
}
nlohmann::json Cp_Transform::GetJson()
{
	m_jsonData["position"]	= Vec3ToJson(m_position);
	m_jsonData["rotation"]	= Vec3ToJson(m_rotation);
	m_jsonData["scale"]		= Vec3ToJson(m_scale);

	m_jsonData["matTag"]	= m_myMatTag;
	m_jsonData["parentMatTag"]	= m_parentMatTag;
	return m_jsonData;
}

Math::Matrix Cp_Transform::GetMatrix(std::string _matTag)
{
	std::string::iterator it = (_matTag.empty() ? m_myMatTag : _matTag).begin();
	std::string::iterator end = (_matTag.empty() ? m_myMatTag : _matTag).end();

	auto ReturnMat = [&]()
		{
			if (m_mWorld = GetRMat() * GetSMat() * GetTMat(); m_parent.lock())return m_mWorld * m_parent.lock()->GetMatrix(m_parentMatTag);
			else return  m_mWorld;
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

	if(m_parent.expired())return m_mWorld;
	return m_mWorld * m_parent.lock()->GetMatrix(m_parentMatTag);
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
