#include "../../../Game/GameObject.h"
#include "HitResultBlock.h"

#include "../../Rigidbody/Rigidbody.h"
#include "../../Transform/Transform.h"
#include "HitResultBlock.h"



void Cp_HitResultBlock::UpdateContents()
{
	m_hit = false;
	float range = 0.0f;
	Math::Vector3 move;
	for (auto& result : m_rigid.lock()->GetHitResult())
	{
		if (!(m_colType & result.m_type))continue;
		float nowRange = (m_trans.lock()->GetMatrix().Translation() + m_rigid.lock()->GetOffsetPos() - result.m_hitPos).Length();
		if (range > nowRange || !m_hit)
		{
			result.m_hitDir.y = 0;
			move = result.m_hitDir * result.m_overlapDistance;
			m_hit = true;
		}
	}

	if (m_hit)
	{
		move.y = 0;
		m_rigid.lock()->AddMove(move * 1.1);
	}
}

void Cp_HitResultBlock::PostUpdateContents()
{
	if (m_hit) 
	{
		//m_rigid.lock()->ResetMove();
	}
}
