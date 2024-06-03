﻿#pragma once

class Animation2D
{
public:
	void PreUpdate(Math::Rectangle& _rect);
	void PreUpdate(KdSquarePolygon* _asset);

	void ImGuiUpdate();

	void ChangeAnime(std::string _nowAnime);

	nlohmann::json GetJson();
	void SetJson(nlohmann::json _json);
private:
	struct AnimeFrame
	{
		int w = 0;
		int h = 0;

		void InitJson(nlohmann::json _json)
		{
			w = _json["w"];
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
		std::vector<AnimeFrame> frames;
		int	nowFrame = 0;

		void Animation(Math::Rectangle& _rect);
		void Animation(KdSquarePolygon* _rect);
		void ImGuiUpdate();

		void InitJson(nlohmann::json _json)
		{
			name = _json["name"];
			interval = _json["interval"];

			for (auto& frame : _json["frames"])
			{
				AnimeFrame temp;
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

	AnimeFrame			m_split;
	std::list<AnimeSet> m_animeList;

	std::string			m_nowAnime;
};
