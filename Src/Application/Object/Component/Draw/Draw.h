#pragma once
#include "../Component.h"

#include "../Transform/Transform.h"

class Cp_Transform;

struct AnimeSet
{
	void Init(nlohmann::json _json);
	nlohmann::json GetJson();

	void Update(Math::Rectangle& _rect);
	void ImGuiUpdate(std::string _tag);

	int h = 0;
	int w = 0;

	int	interval;
};


class Cp_Draw
	:public Component
{
private:
	enum DrawType
	{
		Lit = 1 << 0,
		UnLit = 1 << 1,
		Bright = 1 << 2,
		UI = 1 << 3,
		DepthOfShadow = 1 << 4,
	};

public:
	Cp_Draw(){}
	~Cp_Draw() {}

	void Start()override;
	void InitJson()override;

	void PreDraw();
	void GenerateDepthMapFromLight();
	void DrawLit();
	void DrawUnLit();
	void DrawBright(); 
	void DrawSprite(); 

	void UpdateContents()override;
	void ImGuiUpdate()override;

	nlohmann::json GetJson()override;
	nlohmann::json GetAnimeJson(std::string _tag,AnimeSet _set);

	void SetNowAnime(std::string _str) { m_nowAnime = _str; }
	void SetTex(std::shared_ptr<KdTexture> _tex) {m_tex = _tex;}

	std::weak_ptr<KdSquarePolygon> GetwpSuqrePolygon() {return m_squarePolygon; };
private:
	unsigned int							m_drawType		= DrawType::UI;
	std::shared_ptr<KdTexture>				m_tex			= nullptr;
	Math::Rectangle					m_rect		= { 0,0,64,64 }; 

	std::shared_ptr<KdModelData>			m_modelData		= nullptr;
	std::shared_ptr<KdSquarePolygon>		m_squarePolygon = nullptr;
	
	std::string m_texPath			= "chara/block";
	std::string m_modelDataPath		= "";
	std::string m_squarePolygonPath	= "";
	
	bool							m_bAnime	= false;
	std::string						m_nowAnime	= "default";
	std::map<std::string, AnimeSet>	m_animeMap;

	std::weak_ptr<Cp_Transform>		m_wpTrans;
};
