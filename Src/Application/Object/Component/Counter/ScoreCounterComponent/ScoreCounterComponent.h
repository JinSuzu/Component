#pragma once


class ScoreCounterComponent
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;

private:

	std::list<std::weak_ptr<class SquarePolygonComponent>>m_drawList;
};
