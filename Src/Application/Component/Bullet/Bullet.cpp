#include "Bullet.h"
#include "../../Object/Object.h"
#include "../Transform/Transform.h"
#include "../../Object/Manager/ObjectManager.h"
#include "../BoxCollision/BoxCollision.h"

void Cp_Bullet::Start(std::weak_ptr<Object> _owner)
{
	Component::Start(_owner);
	m_owner = _owner;
	_owner.lock()->DotSave();
	m_swTrans = _owner.lock()->GetspTransform();

	auto Fn = [&](ObjectTag _it) {TriggerFn(_it); };
	_owner.lock()->GetComponent<Cp_BoxCollision>().lock()
		//->PushTriggerFn(std::bind(&Cp_Bullet::TriggerFn, this, std::placeholders::_1));
		->PushTriggerFn(Fn);
}

void Cp_Bullet::PostUpdate()
{
	Math::Vector3 pos = m_swTrans.lock()->GetPosition();
	bool flg = abs(pos.x) - 330 > 350;
	flg |= abs(pos.y) > 400;

	if (flg)m_owner.lock()->Destroy();
}

void Cp_Bullet::Update()
{
	Math::Vector3 pos = m_swTrans.lock()->GetPosition();
	m_swTrans.lock()->SetPosition(pos + move);
}

void Cp_Bullet::Emit(std::weak_ptr<Object> _obj,float _angle)
{
	m_owner.lock()->SetTag(_obj.lock()->GetTag());
	m_swTrans.lock()->SetPosition(_obj.lock()->GetspTransform().lock()->GetPosition());
	move = TrigToVec2D(_angle) * speedPow;
}

nlohmann::json Cp_Bullet::GetJson()
{
	return nlohmann::json();
}

void Cp_Bullet::TriggerFn(ObjectTag _tag)
{
	if (m_owner.lock()->GetTag() == _tag) return;
	m_owner.lock()->Destroy();
}