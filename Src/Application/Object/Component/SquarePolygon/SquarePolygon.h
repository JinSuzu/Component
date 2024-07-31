﻿#pragma once
#include "../Component.h"

class Cp_SquarePolygon
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void PostUpdateContents()override;
	void ImGuiUpdate()override;

	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

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

	struct SquarePolygon
		:RenderManager::Asset3DPackBase
	{
		void Draw(UINT _type)override
		{
			if (drawType & _type)
			{
				KdShaderManager::Instance().m_StandardShader.DrawPolygon(*squarePolygon, mat,color);
			}
		}

		Math::Matrix						mat = Math::Matrix::Identity;
		Math::Color							color = kWhiteColor;
		std::shared_ptr<KdSquarePolygon>	squarePolygon;
	};

	std::shared_ptr<SquarePolygon>			m_squarePolygonPack;
};
