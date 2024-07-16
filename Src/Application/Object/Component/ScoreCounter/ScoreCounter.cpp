#include "ScoreCounter.h"
#include "../../Game/GameObject.h"

#include "../SquarePolygon/SquarePolygon.h"
#include "../../../../System/SceneMnager/SceneManager.h"


void Cp_ScoreCounter::Start()
{
	m_drawList = m_owner.lock()->GetComponents<Cp_SquarePolygon>();
	assert(m_drawList.size() >= 3 && "drawの足りないよ！！");
}

void Cp_ScoreCounter::PreUpdateContents()
{
	std::list<std::weak_ptr<class Cp_SquarePolygon>>::iterator it = m_drawList.begin();

	int cnt = Score::Instance().Get();

	(*it).lock()->SetAnimeName(std::to_string(cnt - ((cnt / 10) * 10)));
	it++;
	(*it).lock()->SetAnimeName(std::to_string(cnt / 10));
	it++;
	(*it).lock()->SetAnimeName(std::to_string(cnt / 100));
}

void Cp_ScoreCounter::ImGuiUpdate()
{
}
