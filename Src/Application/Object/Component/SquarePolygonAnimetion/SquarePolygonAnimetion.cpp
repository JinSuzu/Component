#include "SquarePolygonAnimetion.h"
#include "../Draw/Draw.h"
#include "../../Game/GameObject.h"
#include "../../../Utility/Timer.h"

void Cp_SquarePolygonAnimetion::Start()
{
	m_wpDraw = m_owner.lock()->GetComponent<Cp_Draw>();
	assert(m_wpDraw.lock() && "Drawが見つかりません！！！");
}

void Cp_SquarePolygonAnimetion::PostUpdateContents()
{
	if (m_wpDraw.expired())return;
	if (auto asset = m_wpDraw.lock()->GetwpSuqrePolygon(); asset.lock())
	{
		asset.lock()->SetSplit(m_split.w, m_split.h);
		for (auto& anime : m_animes) 
		{
			if (anime.name == m_nowAnime) 
			{
				anime.Animetion(asset);
				return;
			}
		}
	}
}

void Cp_SquarePolygonAnimetion::ImGuiUpdate()
{
	ImGui::InputInt2("Split", &m_split.w);
	ImGui::InputText("nowAnime",&m_nowAnime);

	static std::string name;
	ImGui::InputText("Name",&name);
	if (ImGui::SameLine(); ImGui::Button("add")) 
	{
		AnimeSet temp;
		temp.name = name;
		m_animes.push_back(temp);
	}
	auto anime = m_animes.begin();
	while (anime != m_animes.end())
	{
		if (ImGui::TreeNode(anime->name.c_str()))
		{
			anime->ImGuiUpdate();

			ImGui::TreePop();
		}
		else if (ImGui::SameLine(); ImGui::SmallButton("Remove")) 
		{
			anime = m_animes.erase(anime);
			continue;
		}
		anime++;
	}
}

void Cp_SquarePolygonAnimetion::InitJson()
{
	m_split.InitJson(m_jsonData["split"]);
	for (auto& animeDate : m_jsonData["animes"]) 
	{
		AnimeSet anime;
		anime.InitJson(animeDate);
		m_animes.push_back(anime);
	}
	m_nowAnime = m_jsonData["nowAnime"];
}
nlohmann::json Cp_SquarePolygonAnimetion::GetJson()
{
	nlohmann::json json;
	for (auto& anime : m_animes) json["animes"].push_back(anime.GetJson());
	json["split"] = m_split.GetJson();
	json["nowAnime"] = m_nowAnime;

	return json;
}

void Cp_SquarePolygonAnimetion::AnimeSet::Animetion(std::weak_ptr<KdSquarePolygon> _asset)
{
	if (frames.empty())return;
	if (Timer::Instance().GetAlarm(interval))
	{
		nowKoma *= ++nowKoma < frames.size();
	}
	_asset.lock()->SetUVRect(frames[nowKoma].w, frames[nowKoma].h);
}
void Cp_SquarePolygonAnimetion::AnimeSet::ImGuiUpdate()
{
	static IntSet addKoma;
	ImGui::InputInt("interval",&interval);
	ImGui::InputInt2("Koma", &addKoma.w); ImGui::SameLine();
	if (ImGui::Button("Add"))frames.push_back(addKoma);

	ImGui::BeginChild("frame", {0,0}, ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY);
	for (int i = 0; i < frames.size(); i++) 
	{
		ImGui::DragInt2(std::to_string(i).c_str(),&frames[i].w);
		if (ImGui::SameLine();ImGui::Button("Remove")) frames.erase(std::next(frames.begin(), i--));
	}
	ImGui::EndChild();
}
