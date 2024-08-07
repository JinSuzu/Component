#include "TimerDisplayComponent.h"
#include "../../../Game/GameObject.h"

#include "../../../../Utility/Timer.h"

void TimerDisplayComponent::Start()
{
	m_drawList = m_owner.lock()->GetComponents<SquarePolygonComponent>();
	assert(m_drawList.size() >= 3 && "drawの足りないよ！！");
}

void TimerDisplayComponent::PreUpdateContents()
{
	std::list<std::weak_ptr<SquarePolygonComponent>>::iterator it = m_drawList.begin();
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

void TimerDisplayComponent::ImGuiUpdate()
{
	ImGui::InputText("SceneTag", &m_sceneTag);
}

void TimerDisplayComponent::LoadJson(nlohmann::json _json)
{
	if (_json["sceneTag"].is_string())m_sceneTag = _json["sceneTag"];
}

nlohmann::json TimerDisplayComponent::Serialize()
{
	nlohmann::json json;
	json["sceneTag"] = m_sceneTag;
	return json;
}
