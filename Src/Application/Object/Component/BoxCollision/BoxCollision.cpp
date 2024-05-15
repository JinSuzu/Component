#include "BoxCollision.h"
#include "../../Object/GameObject.h"

#include "../Transform/Transform.h"

void Cp_BoxCollision::Start()
{
	m_wpTransform = m_owner.lock()->GetspTransform();
	COMPONENTLISTINSTANCE(BoxCollision).Set(std::static_pointer_cast<Cp_BoxCollision>(shared_from_this()));

}

void Cp_BoxCollision::InitJson()
{
	Component::InitJson();
	if (m_jsonData.is_object())
	{
		m_rad = JsonToVec3(m_jsonData["rad"]);
		m_bTrigger = m_jsonData["bTrigger"];
	}
}

void Cp_BoxCollision::PreUpdate()
{
	m_postPos = m_wpTransform.lock()->GetPosition();
	m_colliList.clear();
}

void Cp_BoxCollision::Update()
{
}

void Cp_BoxCollision::PostUpdate()
{
	if (m_bHit && m_bTrigger)
	{
		for (auto&& it : m_colliList) 
		{
			if(m_triggerAttach)m_triggerAttach(it);
		}
	}
	
	if (m_bHit)for (auto&& it : m_colliList)
	{
		if (m_hitAttach) m_hitAttach(it);
	}

	m_bHit = false;
}

void Cp_BoxCollision::ImGuiUpdate()
{
	float value[3] = TOARRAY3(m_rad);
	ImGui::DragFloat3("rad", value);
	m_rad = Math::Vector3(value);

	ImGui::Checkbox("trigger", &m_bTrigger);
}

nlohmann::json Cp_BoxCollision::GetJson()
{
	m_jsonData["rad"] = Vec3ToJson(m_rad);
	m_jsonData["bTrigger"] = m_bTrigger;
	return m_jsonData;
}


bool Cp_BoxCollision::WithPoint(std::weak_ptr<Cp_BoxCollision> _pos)
{
	bool flg =  WithPoint(_pos.lock()->m_wpTransform.lock()->GetPosition());
	m_bHit |= flg;
	return flg;
}
bool Cp_BoxCollision::WithPoint(Math::Vector3 _pos)
{
	Math::Vector3 max(m_wpTransform.lock()->GetPosition() + m_rad);
	Math::Vector3 min(m_wpTransform.lock()->GetPosition() - m_rad);

	auto Bigger = [](Math::Vector3 a, Math::Vector3 b) {return a.x >= b.x && a.y >= b.y && a.z >= b.z; };
	return Bigger(max, _pos) && Bigger(_pos, min);
}

bool Cp_BoxCollision::WithSquare(std::weak_ptr<Cp_BoxCollision> _coliSet)
{
	//if (_coliSet.lock()->GetTag() == GetTag())return false;

	bool flg = false;
	auto area = [](Math::Vector3 _rad) {return _rad.x * _rad.y * _rad.z; };
	
	if (area(m_wpTransform.lock()->GetPosition()) 
		>= area(_coliSet.lock()->m_wpTransform.lock()->GetPosition()))
	{
		for(auto&& it:_coliSet.lock()->GetVertex())flg |= WithPoint(it);
		return flg;
	}

	return WithPoint(std::static_pointer_cast<Cp_BoxCollision>(shared_from_this()));
}

void Cp_BoxCollision::PushColliList(ObjectTag _add)
{
	if (!(m_bTrigger && m_bHit))return;
	m_colliList.push_back(_add);
}

void Cp_BoxCollision::TurnPostPos(std::weak_ptr<Cp_BoxCollision> _other)
{
	if (m_bTrigger)return;
	int count = 5;
	Math::Vector3 pos = _other.lock()->GetOwner().lock()->GetspTransform().lock()->GetPosition();
	while (WithPoint(_other) && count > 0)
	{
		if (m_owner.lock()->GetspTransform().lock()->GetPosition() == pos)break;
		m_wpTransform.lock()->SetPosition(pos + (m_postPos * (float)count));
		count--;
	}
}

std::vector<Math::Vector3> Cp_BoxCollision::GetVertex()
{
	Math::Vector3 pos = m_wpTransform.lock()->GetPosition();
	return std::vector<Math::Vector3>(
		{
			pos + m_rad,
			pos + Math::Vector3(m_rad.x,-m_rad.y,m_rad.z),
			pos + Math::Vector3(m_rad.x,m_rad.y,-m_rad.z),
			pos + Math::Vector3(m_rad.x,-m_rad.y,-m_rad.z),
			pos - m_rad,
			pos + Math::Vector3(-m_rad.x,m_rad.y,m_rad.z),
			pos + Math::Vector3(-m_rad.x,-m_rad.y,m_rad.z),
			pos + Math::Vector3(-m_rad.x,m_rad.y,-m_rad.z),
		});
}
