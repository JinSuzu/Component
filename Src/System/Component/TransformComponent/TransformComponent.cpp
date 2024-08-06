#include "TransformComponent.h"
#include "../../../Application/Object/Game/GameObject.h"

void TransformComponent::Start()
{
	m_getMatrix['S'] = [this]() {return GetSMat(); };
	m_getMatrix['R'] = [this]() {return GetRMat(); };
	m_getMatrix['T'] = [this]() {return GetTMat(); };

	std::weak_ptr<GameObject>parent = m_owner.lock()->GetParent();
	if (parent.expired())return;
	SetParent(parent.lock()->GetTransform());
}

void TransformComponent::ImGuiUpdate()
{
	ImGui::DragFloat3("Position", &m_position.x);
	ImGui::DragFloat3("Rotation", &m_rotation.x);
	ImGui::DragFloat3("Scale", &m_scale.x);


	ImGui::InputText("myMatTag", &m_myMatTag);

	if (!ImGui::TreeNode("Property"))return;
	{
		Math::Vector3 vec = GetMatrix().Translation();
		ImGui::InputFloat3("Mat", &vec.x);

		ImGui::Checkbox("Follow", &m_bFollow);
	}
	ImGui::TreePop();

}

void TransformComponent::LoadJson(nlohmann::json _json)
{
	m_myMatTag = _json["matTag"];
	m_bFollow = _json["Follow"];

	m_position = Utility::JsonHelper::InPutVec3(_json["position"]);
	m_rotation = Utility::JsonHelper::InPutVec3(_json["rotation"]);
	m_scale = Utility::JsonHelper::InPutVec3(_json["scale"]);
}

nlohmann::json TransformComponent::GetJson()
{
	nlohmann::json json;
	json["position"] = Utility::JsonHelper::OutPutVec3(m_position);
	json["rotation"] = Utility::JsonHelper::OutPutVec3(m_rotation);
	json["scale"] = Utility::JsonHelper::OutPutVec3(m_scale);

	json["matTag"] = m_myMatTag;

	json["Follow"] = m_bFollow;
	return json;
}

void TransformComponent::MatrixUpdata()
{
	if (m_myMatTag.empty())
	{
		m_mWorld = GetSMat() * GetRMat() * GetTMat();
		if (m_parent.lock() && m_bFollow)m_mWorld *= m_parent.lock()->GetMatrix();
		return;
	}

	m_mWorld = Math::Matrix::Identity;

	std::unordered_map<char, std::function<Math::Matrix()>>::iterator Fn;
	for (auto& it : m_myMatTag)
	{
		Fn = m_getMatrix.find(it);
		if (Fn == m_getMatrix.end())
		{
			m_mWorld = GetSMat() * GetRMat() * GetTMat();
			if (m_parent.lock() && m_bFollow)m_mWorld *= m_parent.lock()->GetMatrix();
			return;
		}
		else
		{
			m_mWorld *= Fn->second();
		}
	}

	if (m_parent.lock() && m_bFollow)m_mWorld *= m_parent.lock()->GetMatrix();
}
