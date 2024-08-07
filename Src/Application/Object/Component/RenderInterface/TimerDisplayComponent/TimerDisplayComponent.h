#pragma once

class TimerDisplayComponent
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;
private:
	std::list<std::weak_ptr<SquarePolygonComponent>>m_drawList;
	std::string m_sceneTag = "Game";
};
