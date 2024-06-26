#include "TimerDisplay.h"
#include "../../Game/GameObject.h"

#include "../SquarePolygon/SquarePolygon.h"
#include "../../../SceneBase/Manager/SceneManager.h"
#include "../../../Utility/Timer.h"

void Cp_TimerDisplay::Start()
{
	m_drawList = m_owner.lock()->GetComponents<Cp_SquarePolygon>();
	assert(m_drawList.size() >= 3 && "drawの足りないよ！！");
}

void Cp_TimerDisplay::PreUpdateContents()
{
	std::list<std::weak_ptr<class Cp_SquarePolygon>>::iterator it = m_drawList.begin();
	int frame = Timer::Instance().GetCnt(m_sceneTag);

	int s = frame / 60;
	int m = s / 60;
	s = s - (s / 60) * 60;
	m = m - (m / 60) * 60;

	(*it).lock()->SetAnimeName(std::to_string(s - ((s / 10) * 10)));
	it++;
	(*it).lock()->SetAnimeName(std::to_string(s / 10));
	it++;
	(*it).lock()->SetAnimeName(std::to_string(m));
}

void Cp_TimerDisplay::ImGuiUpdate()
{
	ImGui::InputText("SceneTag", &m_sceneTag);
}

void Cp_TimerDisplay::InitJson()
{
	if (m_jsonData["sceneTag"].is_string())m_sceneTag = m_jsonData["sceneTag"];
}

nlohmann::json Cp_TimerDisplay::GetJson()
{
	m_jsonData["sceneTag"] = m_sceneTag;
	return m_jsonData;
}
