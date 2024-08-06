#include "BoxCollisionComponent.h"
#include "../../Game/GameObject.h"

void BoxCollisionComponent::Start()
{
	m_wpTransform = m_owner.lock()->GetTransform();
	//COMPONENTLISTINSTANCE(BoxCollision).Set(std::static_pointer_cast<Cp_BoxCollision>(shared_from_this()));
}



void BoxCollisionComponent::PreUpdateContents()
{
	m_postPos = m_wpTransform.lock()->GetLocalPosition();
	m_colliList.clear();
}

void BoxCollisionComponent::UpdateContents()
{
}

void BoxCollisionComponent::PostUpdateContents()
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

void BoxCollisionComponent::ImGuiUpdate()
{
	ImGui::DragFloat3("rad", &m_rad.x);
	ImGui::Checkbox("trigger", &m_bTrigger);
}

void BoxCollisionComponent::LoadJson(nlohmann::json _json)
{
		m_rad = Utility::JsonHelper::InPutVec3(_json["rad"]);
		m_bTrigger = _json["bTrigger"];
}

nlohmann::json BoxCollisionComponent::GetJson()
{
	nlohmann::json json;
	json["rad"]		 = Utility::JsonHelper::OutPutVec3(m_rad);
	json["bTrigger"] = m_bTrigger;
	return json;
}


bool BoxCollisionComponent::WithPoint(std::weak_ptr<BoxCollisionComponent> _pos)
{
	bool flg =  WithPoint(_pos.lock()->m_wpTransform.lock()->GetLocalPosition());
	m_bHit |= flg;
	return flg;
}
bool BoxCollisionComponent::WithPoint(Math::Vector3 _pos)
{
	Math::Vector3 max(m_wpTransform.lock()->GetLocalPosition() + m_rad);
	Math::Vector3 min(m_wpTransform.lock()->GetLocalPosition() - m_rad);

	auto Bigger = [](Math::Vector3 a, Math::Vector3 b) {return a.x >= b.x && a.y >= b.y && a.z >= b.z; };
	return Bigger(max, _pos) && Bigger(_pos, min);
}

bool BoxCollisionComponent::WithSquare(std::weak_ptr<BoxCollisionComponent> _coliSet)
{
	//if (_coliSet.lock()->GetTag() == GetTag())return false;

	bool flg = false;
	auto area = [](Math::Vector3 _rad) {return _rad.x * _rad.y * _rad.z; };
	
	if (area(m_wpTransform.lock()->GetLocalPosition()) 
		>= area(_coliSet.lock()->m_wpTransform.lock()->GetLocalPosition()))
	{
		for(auto&& it:_coliSet.lock()->GetVertex())flg |= WithPoint(it);
		return flg;
	}

	return WithPoint(std::static_pointer_cast<BoxCollisionComponent>(shared_from_this()));
}

void BoxCollisionComponent::PushColliList(ObjectTag _add)
{
	if (!(m_bTrigger && m_bHit))return;
	m_colliList.push_back(_add);
}

void BoxCollisionComponent::TurnPostPos(std::weak_ptr<BoxCollisionComponent> _other)
{
	if (m_bTrigger)return;
	int count = 5;
	Math::Vector3 pos = _other.lock()->GetOwner().lock()->GetTransform().lock()->GetLocalPosition();
	while (WithPoint(_other) && count > 0)
	{
		if (m_owner.lock()->GetTransform().lock()->GetLocalPosition() == pos)break;
		m_wpTransform.lock()->SetPosition(pos + (m_postPos * (float)count));
		count--;
	}
}

std::vector<Math::Vector3> BoxCollisionComponent::GetVertex()
{
	Math::Vector3 pos = m_wpTransform.lock()->GetLocalPosition();
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
