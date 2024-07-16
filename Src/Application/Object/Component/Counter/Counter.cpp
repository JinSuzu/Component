#include "Counter.h"
#include "../../Game/GameObject.h"

#include "../SquarePolygon/SquarePolygon.h"


void Cp_Counter::Start()
{
	m_drawList = m_owner.lock()->GetComponents<Cp_SquarePolygon>();
	assert(m_drawList.size() >= 2 && "drawの足りないよ！！");
}

void Cp_Counter::PreUpdateContents()
{
	if (m_max == 0) 
	{
		m_max = m_owner.lock()->GetChilds().size();
	}


	std::list<std::weak_ptr<class Cp_SquarePolygon>>::iterator it = m_drawList.begin();

	int cnt = m_max - m_owner.lock()->GetChilds().size();

	(*it).lock()->SetAnimeName(std::to_string(cnt - ((cnt / 10) * 10)));
	it++;
	(*it).lock()->SetAnimeName(std::to_string(cnt / 10));
}

void Cp_Counter::ImGuiUpdate()
{
}
