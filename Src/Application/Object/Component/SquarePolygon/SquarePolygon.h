#pragma once
#include "../Component.h"

class Cp_SquarePolygon
	:public Component
{
public:
	void Draw3D(UINT type);

	void Start()override;
	void PreUpdateContents()override;
	void PostUpdateContents()override;
	void ImGuiUpdate()override;

	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

	std::weak_ptr<KdSquarePolygon>GetSquarePolygon() { return m_squarePolygon; };
	void SetColor(Math::Color _color) { m_color = _color; }
	Math::Color GetColor() const { return m_color; }

	void SetAnimeName(std::string _name);
private:
	UINT								m_drawType;

	std::string							m_path;
	std::shared_ptr<KdSquarePolygon>	m_squarePolygon;
	Math::Color							m_color = kWhiteColor;

	Math::Vector3						m_offsetPos;
	bool								m_cameraFocus = false;


	std::weak_ptr<class Cp_Transform> m_trans;

	int							m_split[2] = { 1,1 };
	std::shared_ptr<class Animation2D>	m_animation;

	std::shared_ptr<std::function<void(UINT)>> m_draw3D;
};
