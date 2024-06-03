#pragma once
#include "../Component.h"

class Cp_SquarePolygon
	:public Component
{
public:
	void Draw3D(UINT type);

	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;
private:
	UINT								m_drawType;

	std::string							m_path;
	std::shared_ptr<KdSquarePolygon>	m_squarePolygon;

	std::weak_ptr<class Cp_Transform> m_trans;

	int							m_split[2] = {1,1};
	std::shared_ptr<class Animation2D>	m_animation;

	std::shared_ptr<std::function<void(UINT)>> m_draw3D;
};
