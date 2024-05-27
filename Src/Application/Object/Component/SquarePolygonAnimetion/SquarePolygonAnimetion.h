#pragma once
#include "../Component.h"

class Cp_SquarePolygonAnimetion
	:public Component
{
public:
	void Start()override;
	void PostUpdateContents()override;
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;
private:
	struct IntSet
	{
		int w = 0;
		int h = 0;

		void InitJson(nlohmann::json _json) 
		{
			w =_json["w"];
			h = _json["h"];
		}
		
		nlohmann::json GetJson() 
		{
			nlohmann::json json;
			json["w"] = w;
			json["h"] = h;
			return json;
		}
	};
	struct AnimeSet 
	{
		std::string name;
		int	interval;
		std::vector<IntSet> frames;
		int	nowKoma = 0;

		void Animetion(std::weak_ptr<KdSquarePolygon> _asset);
		void ImGuiUpdate();

		void InitJson(nlohmann::json _json) 
		{
			name = _json["name"];
			interval = _json["interval"];
			
			for(auto & frame:_json["frames"])
			{
				IntSet temp;
				temp.InitJson(frame);
				frames.push_back(temp);
			}
		}
		nlohmann::json GetJson()
		{
			nlohmann::json json;
			json["name"] = name;
			json["interval"] = interval;
			for (auto& frame : frames)
			{
				json["frames"].push_back(frame.GetJson());
			}
			return json;
		}
	};

	std::string						m_nowAnime;
	IntSet							m_split = {1,1};
	std::list<AnimeSet>				m_animes;

	std::weak_ptr<class Cp_Draw>	m_wpDraw;
};