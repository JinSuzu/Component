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

	int									interval;
};

class Cp_DrawTex
	:public Component
{
public:
	Cp_DrawTex(std::shared_ptr<KdTexture> _tex = nullptr)
		:m_tex(_tex)
	{}
	~Cp_DrawTex() {}

	void Start()override;
	void InitJson()override;

	void Draw()override;
	void UpdateContents()override;
	void ImGuiUpdate()override;
	nlohmann::json GetJson()override;
	nlohmann::json GetAnimeJson(std::string _tag,AnimeSet _set);

	void SetNowAnime(std::string _str) { m_nowAnime = _str; }
	void SetTex(std::shared_ptr<KdTexture> _tex) {m_tex = _tex;}

private:
	std::shared_ptr<KdTexture>		m_tex		= nullptr;
	Math::Rectangle					m_rect		= { 0,0,64,64 };
	std::string						m_path		= "chara/block.png";
	
	bool							m_bAnime	= false;
	std::string						m_nowAnime	= "default";
	std::map<std::string, AnimeSet>	m_animeMap;

	std::weak_ptr<Cp_Transform>		m_wpTrans;
};
