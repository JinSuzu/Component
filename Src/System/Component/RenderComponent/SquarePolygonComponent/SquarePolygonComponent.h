﻿#pragma once

class SquarePolygonComponent
	:public Component
{
public:
	void Awake()override;
	void Start()override;
	void PreUpdateContents()override;
	void PostUpdateContents()override;
	void ImGuiUpdate()override;

	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;

	std::weak_ptr<KdSquarePolygon>GetSquarePolygon() { return m_squarePolygonPack->squarePolygon; };
	void SetColor(Math::Color _color) { m_squarePolygonPack->color = _color; }
	Math::Color GetColor() const { return m_squarePolygonPack->color; }

	void SetAnimeName(std::string _name);
private:
	std::string							m_path;

	Math::Vector3						m_offsetPos;
	bool								m_cameraFocus = false;

	int							m_split[2] = { 1,1 };
	std::shared_ptr<class Animation2D>	m_animation;

	std::shared_ptr<RenderManager::SquarePolygonComponent>			m_squarePolygonPack;
};
