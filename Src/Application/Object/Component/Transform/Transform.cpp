#include "Transform.h"
#include "../../Game/GameObject.h"

void Cp_Transform::Start()
{
	m_getMatrix['S'] = [this]() {return GetSMat(); };
	m_getMatrix['R'] = [this]() {return GetRMat(); };
	m_getMatrix['T'] = [this]() {return GetTMat(); };

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

void Cp_Transform::LoadJson(nlohmann::json _json)
{
	m_myMatTag		= _json["matTag"];
	m_parentMatTag	= _json["parentMatTag"];
	m_bFollow		= _json["Follow"];

	m_position = Utility::JsonHelper::InPutVec3(_json["position"]);
	m_rotation = Utility::JsonHelper::InPutVec3(_json["rotation"]);
	m_scale	   = Utility::JsonHelper::InPutVec3(_json["scale"]);
}

nlohmann::json Cp_Transform::GetJson()
{
	nlohmann::json json;
	json["position"] = Utility::JsonHelper::OutPutVec3(m_position);
	json["rotation"] = Utility::JsonHelper::OutPutVec3(m_rotation);
	json["scale"]    = Utility::JsonHelper::OutPutVec3(m_scale);

	json["matTag"]		 = m_myMatTag;
	json["parentMatTag"] = m_parentMatTag;

	json["Follow"]		 = m_bFollow;
	return json;
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
	
	std::unordered_map<char, std::function<Math::Matrix()>>::iterator matrix = m_getMatrix.find(*it);
	if (matrix != m_getMatrix.end())
	{
		m_mWorld = matrix->second();
	}
	else 
	{
		return ReturnMat();
	}

	it++;
	while (it != end)
	{
		matrix = m_getMatrix.find(*it);
		if (matrix != m_getMatrix.end())
		{
			m_mWorld *= matrix->second();
		}
		else
		{
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
