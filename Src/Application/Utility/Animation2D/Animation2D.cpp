#include "Animation2D.h"
#include "../Timer.h"

void Animation2D::PreUpdate(Math::Rectangle& _rect)
{
	for (auto& anime : m_animeList)
	{
		if (anime.name == m_nowAnime)
		{
			anime.Animation(_rect);
			break;
		}
	}
}
void Animation2D::PreUpdate(KdSquarePolygon* _asset)
{
	for (auto& anime : m_animeList)
	{
		if (anime.name == m_nowAnime)
		{
			m_nowFrame = anime.Animation(_asset);
			break;
		}
	}
}

void Animation2D::ImGuiUpdate()
{
	if (ImGui::TreeNode("Amination"))
	{
		ImGui::InputText("nowAnime", &m_nowAnime);

		static std::string name;
		ImGui::InputText("Name", &name);
		if (ImGui::Button("add"))
		{
			AnimeSet temp;
			temp.name = name;
			m_animeList.push_back(temp);
		}
		auto anime = m_animeList.begin();
		while (anime != m_animeList.end())
		{
			if (ImGui::TreeNode(anime->name.c_str()))
			{
				anime->ImGuiUpdate();

				ImGui::TreePop();
			}

			if (ImGui::SameLine(); ImGui::SmallButton("Remove"))
			{
				anime = m_animeList.erase(anime);
				continue;
			}
			anime++;
		}

		ImGui::TreePop();
	}
}

void Animation2D::ChangeAnime(std::string _nowAnime)
{
	m_nowAnime = _nowAnime;
	for (auto& anime : m_animeList)
	{
		if (anime.name == m_nowAnime)
		{
			anime.nowFrame = 0;
			break;
		}
	}
}

nlohmann::json Animation2D::GetJson()
{
	nlohmann::json json;

	json["nowAnime"] = m_nowAnime;
	json["animeList"] = nlohmann::json::array();
	for (auto& anime : m_animeList) json["animeList"].push_back(anime.GetJson());
	return json;
}

void Animation2D::SetJson(nlohmann::json _json)
{
	m_nowAnime = _json["nowAnime"];
	for (auto& animeDate : _json["animeList"])
	{
		AnimeSet anime;
		anime.InitJson(animeDate);
		m_animeList.push_back(anime);
	}
}

void Animation2D::AnimeSet::Animation(Math::Rectangle& _rect)
{
	if (frames.empty())return;
	if (Timer::Instance().GetAlarm(interval))
	{
		nowFrame *= ++nowFrame < frames.size();
	}

	_rect.x = frames[nowFrame].w * _rect.width;
	_rect.y = frames[nowFrame].h * _rect.height;
}
Animation2D::AnimeFrame Animation2D::AnimeSet::Animation(KdSquarePolygon* _asset)
{
	if (frames.empty())return Animation2D::AnimeFrame();
	if (Timer::Instance().GetAlarm(interval))
	{
		nowFrame *= ++nowFrame < frames.size();
	}
	_asset->SetUVRect(frames[nowFrame].w, frames[nowFrame].h);
	return frames[nowFrame];
}
void Animation2D::AnimeSet::ImGuiUpdate()
{
	static AnimeFrame addKoma;
	ImGui::InputInt("interval", &interval);
	ImGui::InputInt2("AnimeFrame w/h", &addKoma.w);
	if (ImGui::Button("Add"))frames.push_back(addKoma);

	ImGui::BeginChild("frame", { 0,0 }, ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY);
	std::vector<AnimeFrame>::iterator frame = frames.begin();
	int i = 0;
	while (frame != frames.end())
	{
		ImGui::DragInt2(std::to_string(i++).c_str(), &frame->w);
		if (ImGui::SameLine(); ImGui::Button("Remove"))
		{
			frames.erase(frame);
			continue;
		}
		frame++;
	}
	ImGui::EndChild();
}