#include "DrawTex.h"
#include "../Transform/Transform.h"

#include "../../Object/GameObject.h"
#include "../../AssetManager/AssetManager.h"

#include "../../Utility/Timer.h"


void Cp_DrawTex::Start()
{
	m_wpTrans = m_owner.lock()->GetspTransform();
}

void Cp_DrawTex::InitJson()
{
	Component::InitJson();
	if (m_jsonData.is_object())
	{
		m_path = m_jsonData["path"];
		m_tex = AssetManager::Instance().GetKdTexture(m_path);
		m_rect = JsonToRect(m_jsonData["rect"]);
		if (m_jsonData["Animation"].is_array())
		{
			for (auto&& it : m_jsonData["Animation"])
			{
				m_animeMap[it["tag"]] = AnimeSet();
				m_animeMap[it["tag"]].Init(it);
				m_bAnime = true;
				m_nowAnime = it["tag"];
			}
		}
		m_matTag = m_jsonData["matTag"];
	}
}

void Cp_DrawTex::Draw()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_wpTrans.lock()->GetMatrix(m_matTag));

	KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex.get(), 0, 0, m_rect.width, m_rect.height, &m_rect);
	
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void Cp_DrawTex::Update()
{
	if (m_bAnime && Timer::Instance().GetAlarm(m_animeMap[m_nowAnime].interval)) {
		if (m_animeMap.find(m_nowAnime) == m_animeMap.end())return;
		m_animeMap[m_nowAnime].Update(m_rect);
	}

}

void Cp_DrawTex::ImGuiUpdate()
{
	static char path[50] = "";
	ImGui::InputText("Path", path, sizeof(path));
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		m_path = path;
		m_tex = AssetManager::Instance().GetKdTexture(std::string(path));
	}

	float value[4] = { (float)m_rect.x,(float)m_rect.y ,(float)m_rect.width ,(float)m_rect.height };
	ImGui::DragFloat4("Rect", value);
	m_rect = Math::Rectangle((long)value[0], (long)value[1], (long)value[2], (long)value[3]);


	if (ImGui::TreeNode("Animation"))
	{
		ImGui::Checkbox("Active", &m_bAnime);
		static char nowTag[50] = "";
		ImGui::InputText("nowAnimaion", nowTag, sizeof(path));
		ImGui::SameLine(); if (ImGui::Button("Set"))m_nowAnime = nowTag;

		static char animeTag[50] = "";
		ImGui::InputText("AddAnime", animeTag, sizeof(path));
		ImGui::SameLine(); if (ImGui::Button("Add"))m_animeMap[animeTag] = AnimeSet();


		for (auto&& it : m_animeMap) {
			it.second.ImGuiUpdate(it.first);
		}
		ImGui::TreePop();
	}
	static char matTag[50] = "";
	ImGui::InputText("matTag", matTag, sizeof(path));
	ImGui::SameLine(); if (ImGui::Button("Set"))m_matTag = matTag;
}

nlohmann::json Cp_DrawTex::GetJson()
{
	m_jsonData["path"] = m_path;
	m_jsonData["rect"] = RectToJson(m_rect);
	m_jsonData["Animation"] = nlohmann::json::array();
	for (auto&& it : m_animeMap)m_jsonData["Animation"].push_back(GetAnimeJson(it.first, it.second));
	m_jsonData["matTag"] = m_matTag;
	return m_jsonData;
}

nlohmann::json Cp_DrawTex::GetAnimeJson(std::string _tag,AnimeSet _set)
{
	nlohmann::json json;
	json["tag"] = _tag;
	json["h"] = _set.h;
	json["w"] = _set.w;
	json["interval"] = _set.interval;
	return json;
}

void AnimeSet::Init(nlohmann::json _json)
{
	h = _json["h"];
	w = _json["w"];
	interval = _json["interval"];
}

nlohmann::json AnimeSet::GetJson()
{
	nlohmann::json json;
	json["h"] = h;
	json["w"] = w;
	json["interval"] = interval;
	return json;
}

void AnimeSet::Update(Math::Rectangle& _rect)
{
	_rect.x += _rect.width;
	_rect.x *= _rect.x < (_rect.width * w);
	_rect.y = h * _rect.height;
}

void AnimeSet::ImGuiUpdate(std::string _tag)
{
	if (ImGui::TreeNode(_tag.c_str()))
	{
		int value[2] = { h,w };
		ImGui::DragInt2("h/w", value);
		h = value[0]; w = value[1];

		ImGui::DragInt("Interval", &interval);

		ImGui::TreePop();
	}
}
