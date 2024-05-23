#include "Draw.h"
#include "../Transform/Transform.h"

#include "../../Game/GameObject.h"
#include "../../../AssetManager/AssetManager.h"

#include "../../../Utility/Timer.h"

void Cp_Draw::Start()
{
	m_wpTrans = m_owner.lock()->GetTransform();
	m_tex = AssetManager::Instance().GetKdTexture(m_texPath);
	m_rect = { 0,0,(LONG)m_tex->GetInfo().Width,(LONG)m_tex->GetInfo().Height };
}


void Cp_Draw::InitJson()
{
	Component::InitJson();
	if (!m_jsonData.is_null()){
		if (!std::string(m_jsonData["texPath"]).empty())			m_tex = AssetManager::Instance().GetKdTexture(m_texPath = m_jsonData["texPath"]);
																	else m_tex = nullptr;
		if (!std::string(m_jsonData["modelDataPath"]).empty())		m_modelData = AssetManager::Instance().GetModelData(m_modelDataPath = m_jsonData["modelDataPath"]);
																	else m_modelData = nullptr;
		if(!std::string(m_jsonData["squarePolygonPath"]).empty())	m_squarePolygon = AssetManager::Instance().GetSquarePolygon(m_squarePolygonPath = m_jsonData["squarePolygonPath"]);
																	else m_squarePolygon = nullptr;

		m_drawType = m_jsonData["drawType"];

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
	}
}

void Cp_Draw::PreDraw()
{
	
}
void Cp_Draw::GenerateDepthMapFromLight()
{
	if (!(m_drawType & DrawType::DepthOfShadow))return;
	if(m_modelData)    KdShaderManager::Instance().m_StandardShader.DrawModel(*m_modelData, m_wpTrans.lock()->GetMatrix());
	if(m_squarePolygon)KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_squarePolygon, m_wpTrans.lock()->GetMatrix());
}
void Cp_Draw::DrawLit()
{
	if (!(m_drawType & DrawType::Lit))return;
	if (m_modelData)    KdShaderManager::Instance().m_StandardShader.DrawModel(*m_modelData, m_wpTrans.lock()->GetMatrix());
	if (m_squarePolygon)KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_squarePolygon, m_wpTrans.lock()->GetMatrix());

}
void Cp_Draw::DrawUnLit()
{
	if (!(m_drawType & DrawType::UnLit))return;
	if (m_modelData)    KdShaderManager::Instance().m_StandardShader.DrawModel(*m_modelData, m_wpTrans.lock()->GetMatrix());
	if (m_squarePolygon)KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_squarePolygon, m_wpTrans.lock()->GetMatrix());

}
void Cp_Draw::DrawBright()
{
	if (!(m_drawType & DrawType::Bright))return;
	if (m_modelData)    KdShaderManager::Instance().m_StandardShader.DrawModel(*m_modelData, m_wpTrans.lock()->GetMatrix());
	if (m_squarePolygon)KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_squarePolygon, m_wpTrans.lock()->GetMatrix());

}
void Cp_Draw::DrawSprite()
{
	if (!(m_drawType & DrawType::UI))return;

	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_wpTrans.lock()->GetMatrix());

	KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex, 0, 0, m_rect.width, m_rect.height, &m_rect);
	
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void Cp_Draw::UpdateContents()
{
	if (m_bAnime && Timer::Instance().GetAlarm(m_animeMap[m_nowAnime].interval)) {
		if (m_animeMap.find(m_nowAnime) == m_animeMap.end())return;
		m_animeMap[m_nowAnime].Update(m_rect);
	}
}

void Cp_Draw::ImGuiUpdate()
{
	static std::string path = "";
	ImGui::InputText("Path", &path);
	ImGui::SameLine();
	if (ImGui::Button("Load"))ImGui::OpenPopup("Loads");
	if (ImGui::BeginPopup("Loads")) 
	{
		if (ImGui::MenuItem("Texture"))
		{
			m_tex = AssetManager::Instance().GetKdTexture(m_texPath = path);
			m_rect = { 0,0,(LONG)m_tex->GetInfo().Width,(LONG)m_tex->GetInfo().Height };
			if (m_tex) 
			{
				m_jsonData["texPath"] = path;
				m_drawType = DrawType::UI;

				m_squarePolygonPath = "";
				m_modelDataPath = "";
			}

		}

		if (ImGui::MenuItem("ModelData"))
		{
			m_modelData = AssetManager::Instance().GetModelData(m_modelDataPath = path);
			if (m_modelData)
			{
				m_drawType = DrawType::Lit;
				m_texPath = "";
				m_squarePolygonPath = "";
			}
		}
		
		if (ImGui::MenuItem("SquarePolygon"))
		{
			m_squarePolygon = AssetManager::Instance().GetSquarePolygon(m_squarePolygonPath = path);
			if (m_modelData)
			{
				m_drawType = DrawType::Lit;
				m_texPath = "";
				m_modelDataPath = "";
			}
		}
		ImGui::EndPopup();
	}

	if (ImGui::Button("DrawType"))ImGui::OpenPopup("Types");
	if (ImGui::BeginPopup("Types"))
	{
		ImGuiCheckBoxBit("Lit"	 , m_drawType, DrawType::Lit);
		ImGuiCheckBoxBit("UnLit" , m_drawType, DrawType::UnLit);
		ImGuiCheckBoxBit("Bright", m_drawType, DrawType::Bright);
		ImGuiCheckBoxBit("UI"	 , m_drawType, DrawType::UI);
		ImGuiCheckBoxBit("DepthOfShadow", m_drawType, DrawType::DepthOfShadow);

		ImGui::EndPopup();
	}

	float value[4] = { (float)m_rect.x,(float)m_rect.y ,(float)m_rect.width ,(float)m_rect.height };
	ImGui::DragFloat4("Rect", value);
	m_rect = Math::Rectangle((long)value[0], (long)value[1], (long)value[2], (long)value[3]);

	if (ImGui::TreeNode("Animation"))
	{
		ImGui::Checkbox("Active", &m_bAnime);
		static char nowTag[50] = "";
		ImGui::InputText("nowAnimation", nowTag, sizeof(path));
		ImGui::SameLine(); if (ImGui::Button("Set"))m_nowAnime = nowTag;

		static char animeTag[50] = "";
		ImGui::InputText("AddAnime", animeTag, sizeof(path));
		ImGui::SameLine(); if (ImGui::Button("Add"))m_animeMap[animeTag] = AnimeSet();

		for (auto&& it : m_animeMap) {
			it.second.ImGuiUpdate(it.first);
		}

		ImGui::TreePop();
	}
}

nlohmann::json Cp_Draw::GetJson()
{
	m_jsonData["texPath"]			= m_texPath;
	m_jsonData["modelDataPath"]		= m_modelDataPath;
	m_jsonData["squarePolygonPath"] = m_squarePolygonPath;
	
	m_jsonData["drawType"] = m_drawType;

	m_jsonData["rect"] = RectToJson(m_rect);
	m_jsonData["Animation"] = nlohmann::json::array();
	for (auto&& it : m_animeMap)m_jsonData["Animation"].push_back(GetAnimeJson(it.first, it.second));
	return m_jsonData;
}

nlohmann::json Cp_Draw::GetAnimeJson(std::string _tag,AnimeSet _set)
{
	m_jsonData["tag"] = _tag;
	m_jsonData["h"] = _set.h;
	m_jsonData["w"] = _set.w;
	m_jsonData["interval"] = _set.interval;
	return m_jsonData;
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
	json["h"]			= h;
	json["w"]			= w;
	json["interval"]	= interval;
	return json;
}

void AnimeSet::Update(Math::Rectangle& _rect)
{
	_rect.x += _rect.width;
	_rect.x *= _rect.x < (_rect.width * w);
	_rect.y	 = h * _rect.height;
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
