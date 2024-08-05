#include "ScoreCounterComponent.h"
#include "../../../Game/GameObject.h"

void ScoreCounterComponent::Start()
{
	m_drawList = m_owner.lock()->GetComponents<SquarePolygonComponent>();
	assert(m_drawList.size() >= 3 && "drawの足りないよ！！");
}

void ScoreCounterComponent::PreUpdateContents()
{
	std::list<std::weak_ptr<class SquarePolygonComponent>>::iterator it = m_drawList.begin();

	int cnt = Score::Instance().Get();

	(*it).lock()->SetAnimeName(std::to_string(cnt - ((cnt / 10) * 10)));
	it++;
	(*it).lock()->SetAnimeName(std::to_string(cnt / 10));
	it++;
	(*it).lock()->SetAnimeName(std::to_string(cnt / 100));
}

void ScoreCounterComponent::ImGuiUpdate()
{
}
