#include "Bullet.h"
#include "../../Game/GameObject.h"
#include "../Rigidbody/Rigidbody.h"
#include "../Transform/Transform.h"
#include "../../../Utility/Timer.h"
#include "Bullet.h"

void Cp_Bullet::Start()
{
	m_trans = m_owner.lock()->GetTransform();

	m_rigid = m_owner.lock()->GetComponent<Cp_Rigidbody>();
	assert(m_rigid.lock() && "リジットボディが無いよ by Bullet");
	m_rigid.lock()->ResetMove();
}

void Cp_Bullet::PreUpdateContents()
{
	Math::Vector3 move = m_trans.lock()->GetRMat().Backward();
	move.Normalize();

	if (m_final != m_phase)m_phase++;
	m_rigid.lock()->SetMove((move * (m_speedPow * m_phase / (float)m_final)));
}

void Cp_Bullet::ImGuiUpdate()
{
}

void Cp_Bullet::InitJson()
{
}

nlohmann::json Cp_Bullet::GetJson()
{
    return nlohmann::json();
}
