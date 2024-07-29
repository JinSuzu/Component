#include "SetUpScene.h"

void SetUpScene::UpdateContents()
{
	//ImGui::PushItemWidth(ImGui::CalcTextSize(_editStr.c_str()).x + ImGui::GetStyle().FramePadding.x * 2.0f);
	std::string nowScene = SceneManager::Instance().GetNowSceneName();
	if (ImGui::BeginCombo("NowScene", nowScene.c_str()))
	{
		ShowCombContents();
		ImGui::EndCombo();
	}


	if (ImGui::BeginTable("SceneList", m_tableContents.size(), ImGuiTableFlags_BordersH))
	{
		ShowTableContents();
		ImGui::EndTable();
	}
}

void SetUpScene::BeginChildOption()
{
}

void SetUpScene::EndChildOption()
{
	MyDragDrop::TargetScene();
}

void SetUpScene::ShowCombContents()
{
	std::map <std::string, std::string>& list = SceneManager::Instance().WorkSceneList();
	for (auto& map : list)
	{
		if (ImGui::Selectable(map.first.c_str()))
		{
			SceneManager::Instance().LoadScene(map.first);
		}
	}
}

void SetUpScene::ShowTableContents()
{
	std::map <std::string, std::string>& list = SceneManager::Instance().WorkSceneList();
	for (std::pair<const std::string, std::string>& map : list)
	{
		for (auto& content : m_tableContents)
		{
			ImGui::TableNextColumn();
			if (content(map))return;
		}
	}
}

bool SetUpScene::SceneTag(std::pair<const std::string, std::string>& _pair)
{
	std::string first = _pair.first;
	if (Utility::ImGuiHelper::InputText("", first))
	{
		std::string path = _pair.second;
		SceneManager::Instance().WorkSceneList().erase(_pair.first);
		SceneManager::Instance().WorkSceneList().emplace(first, path);
		return true;
	}
	return false;
}
bool SetUpScene::ScenePath(std::pair<const std::string, std::string>& _pair)
{
	ImGui::Text(_pair.second.c_str());
	return false;
}
bool SetUpScene::Remove(std::pair<const std::string, std::string>& _pair)
{
	if (ImGui::SmallButton(("Remove##" + _pair.first).c_str()))
	{
		SceneManager::Instance().WorkSceneList().erase(_pair.first);
		return true;
	}
	return false;
}

SetUpScene::SetUpScene()
{
	m_tableContents.push_back([&](std::pair<const std::string, std::string>& _pair) {return SceneTag(_pair); });
	m_tableContents.push_back([&](std::pair<const std::string, std::string>& _pair) {return ScenePath(_pair); });
	m_tableContents.push_back([&](std::pair<const std::string, std::string>& _pair) {return Remove(_pair); });
}

namespace MyDragDrop
{
	bool SourceScene(std::string _path)
	{
		return Utility::ImGuiHelper::DragDropSource("SceneDragDrop", _path);
	}
	void TargetScene()
	{
		if (std::string _path; Utility::ImGuiHelper::DragDropTarget("SceneDragDrop", _path))
		{
			std::filesystem::path path(_path);
			std::map <std::string, std::string>& list = SceneManager::Instance().WorkSceneList();
			list[path.filename().string()] = path.string();
		}
	}
}