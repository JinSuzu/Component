#include "PlayerCircle.h"
#include "../../Object/Object.h"

#include "../Bullet/Bullet.h"
#include "../../Object/Manager/ObjectManager.h"
#include "../Transform/Transform.h"

void Cp_PlayerCircle::Start(std::weak_ptr<Object> _owner)
{
	Component::Start(_owner);
	m_wpTrans = _owner.lock()->GetspTransform();
	m_wpTrans.lock()->SetPosition({ -330,-280,0 });
}

void Cp_PlayerCircle::Update()
{
	Math::Vector3 pos = m_wpTrans.lock()->GetPosition();
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) pos.x	+= 5;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)  pos.x	-= 5;

	if (pos.x < -500)pos.x = -500;
	if (pos.x > -160)pos.x = -160;

	m_wpTrans.lock()->SetPosition(pos);

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) 
	{
		auto temp = ObjectManager::Instance().CreateObject("Game/BulletBase");
		auto bullet = new Cp_Bullet;
		temp->AddComponent(bullet);
		bullet->Emit(m_owner,90);
	}
}

nlohmann::json Cp_PlayerCircle::GetJson()
{
	return nlohmann::json();
}
