#pragma once

class CounterComponent
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;

private:
	int m_max = 0;

	std::list<std::weak_ptr<class SquarePolygonComponent>>m_drawList;
};
