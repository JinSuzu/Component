#include "Enemy.h"
#include "../../Object/Object.h"
#include "../Transform/Transform.h"
#include "../BoxCollision/BoxCollision.h"

void Cp_Enemy::Start(std::weak_ptr<Object> _owner)
{
	Component::Start(_owner);
	_owner.lock()->SetTag(ObjectTag::Enemy);

	_owner.lock()->GetComponent<Cp_BoxCollision>().lock()
		->PushTriggerFn([&](ObjectTag _tag) {TriggerFn(_tag); });
}

void Cp_Enemy::PreUpdate()
{
	Math::Vector3 pos = m_owner.lock()->GetspTransform().lock()->GetPosition();
	//pos.y -= 5;
	m_owner.lock()->GetspTransform().lock()->SetPosition(pos);
}

void Cp_Enemy::Update()
{
}

nlohmann::json Cp_Enemy::GetJson()
{
	return nlohmann::json();
}

void Cp_Enemy::TriggerFn(ObjectTag _tag)
{
	if (m_owner.lock()->GetTag() == _tag)return;
	m_owner.lock()->Destroy();
}