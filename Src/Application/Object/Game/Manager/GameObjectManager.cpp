#include "GameObjectManager.h"
#include "../../Component/AllComponentIncluder.h"
#include "../GameObject.h"

#define ITERATOR(x)					\
for (auto&& it : family.childs)		\
{									\
	if(it.get() != nullptr)			\
	{								\
		if (it->GetActive())		\
		{							\
			it->x();				\
		}							\
	}								\
}

void GameObjectManager::Draw()
{
	for (auto& family:m_obList) 
	{
		family.Draw();
	}
}

void GameObjectManager::PreUpdate()
{
	auto family = m_obList.begin();
	while (family != m_obList.end())
	{
		if (!family->parent || family->parent->GetDestroy())
		{
			family = m_obList.erase(family);
			break;
		}

		family->PreUpdate();
		family++;
	}
}
void GameObjectManager::Update() {

	for (auto& family : m_obList)
	{
		family.Update();
	}
}
void GameObjectManager::PostUpdate()
{
	for (auto& family : m_obList)
	{
		family.PostUpdate();
	}
}

void GameObjectManager::ImGuiUpdate()
{
	ImGuiCreateObject(true);


	ImGui::SeparatorText(("ObjectList :" + std::to_string(m_obList.size())).c_str());
	ImGui::BeginChild("##ObjectChild", ImVec2(350, 500), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY);

	int obNum = 0;
	for (auto& family : m_obList)
	{
		family.ImGuiUpdate(obNum++);
	}

	ImGui::EndChild();


	ImGui::SeparatorText("EditObject");
	if (!m_editObject)return;

	ImGui::SetNextItemOpen(true);
	if (ImGui::TreeNode(m_editObject->parent->GetName().c_str()))
	{
		m_editObject->parent->ImGuiUpdate();
		ImGuiCreateObject();
		ImGui::TreePop();
	}
}
void GameObjectManager::ImGuiCreateObject(bool _bOrigin)
{
	static std::string path = ""; ImGui::InputText("JsonPath", &path);
	static unsigned int state; int Max = ComponentMap::Instance().GetCompoNum();
	if (ImGui::BeginTabBar("CreateObject"))
	{
		if (ImGui::BeginTabItem("Custom"))
		{
			ImGui::BeginChild("##ComponentSet", ImVec2(350, 100), ImGuiChildFlags_Border);
			ImGui::SeparatorText("ComponentSet");
			for (int i = 0; i < Max; i++)
			{
				bool flg = state & (1 << i);
				if (i % 3)ImGui::SameLine();
				ImGui::Checkbox(ComponentMap::Instance().GetTag(1 << i).c_str(), &flg);

				if (flg)state |= (1 << i);
				if (!flg)state &= ~(1 << i);
			}
			ImGui::EndChild();

			//ImGui::SameLine();
			if (ImGui::Button("Add"))
			{
				CreateObject(path, _bOrigin ? nullptr : m_editObject)
					->AddComponents(state);
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("ImportPreSet"))
		{
			if (ImGui::Button("Add"))
			{
				nlohmann::json json = InPutJson("GameObjectSet/" + path);

				auto name = json.begin();
				while (name != json.end())
				{
					(_bOrigin ? m_obList : m_editObject->childs).push_back(GameObjectFamily(name));
					name++;
				}
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}
void GameObjectManager::ImGuiAddComponent(std::weak_ptr<GameObject> _object)
{
	if (ImGuiTreeCenterButton("AddComponent"))ImGui::OpenPopup("Components");
	if (!ImGui::BeginPopup("Components"))return;
	ImGui::SeparatorText("Component");
	auto it = ComponentMap::Instance().bitBegin();
	while (it != ComponentMap::Instance().bitEnd())
	{
		if (ImGui::MenuItem(it->first.c_str()))
		{
			if (auto obj = _object.lock()) {
				obj->AddComponent(it->second);
			}
			break;
		}
		++it;
	}
	ImGui::EndPopup();

}

void GameObjectManager::Init()
{
	nlohmann::json json = InPutJson("Scene/Test");

	auto name = json.begin();
	while (name != json.end())
	{
		m_obList.push_back(GameObjectFamily(name++));
	}
}

void GameObjectManager::Release()
{
	nlohmann::json json;
	auto it = m_obList.begin();
	while (it != m_obList.end()) 
	{
		json.push_back(it->GetJson());
		it = m_obList.erase(it);
	}

	OutPutJson(json,"Scene/Test");
}

std::shared_ptr<Component> GameObjectManager::ToComponent(unsigned int _id)
{
	return ComponentMap::Instance().createFind(_id);
}
std::string GameObjectManager::ToTag(unsigned int _id)
{
	return ComponentMap::Instance().GetTag(_id);
}
unsigned int GameObjectManager::ToID(std::string _tag)
{
	auto ID = ComponentMap::Instance().bitFind(_tag);
	if (ID == ComponentMap::Instance().bitEnd())assert(false && "コンポIDListにないよ！！");
	return ID->second;
}

//根っこに追加/帰属しないObject生成用
std::shared_ptr<GameObject> GameObjectManager::CreateObject(std::string _tag, bool flg)
{
	auto object = std::make_shared<GameObject>();
	object->Init(_tag);
	if (flg)m_obList.push_back(GameObjectFamily(object));
	return object;
}

//GameObjectFamily用
std::shared_ptr<GameObject> GameObjectManager::CreateObject(std::string _tag, GameObjectFamily* _family,bool bPush)
{
	auto object = std::make_shared<GameObject>();
	object->Init(_tag);
	object->SetParent(_family ? _family->parent : nullptr);

	if (!bPush) return object;

	if(_family)_family->childs.push_back(GameObjectFamily(object));
	else m_obList.push_back(GameObjectFamily(object));

	return object;
}
std::shared_ptr<GameObject> GameObjectManager::CreateObject(nlohmann::json _json, GameObjectFamily* _family, bool bPush)
{
	auto object = std::make_shared<GameObject>();
	object->Init(_json);
	object->SetParent(_family ? _family->parent : nullptr);

	if (!bPush) return object;

	if (_family)_family->childs.push_back(GameObjectFamily(object));
	else m_obList.push_back(GameObjectFamily(object));

	return object;
}

#pragma region GameObjectFamily
void GameObjectManager::GameObjectFamily::Draw() {
	if (parent) parent->Draw();
	for (auto& child : childs) {
		child.Draw();
	}
}
void GameObjectManager::GameObjectFamily::PreUpdate() {
	if (parent) parent->Update();
	auto child = childs.begin();
	while (child != childs.end())
	{
		if (!child->parent || child->parent->GetDestroy())
		{
			child = childs.erase(child);
			break;
		}
		child->PreUpdate();
		child++;
	}
}
void GameObjectManager::GameObjectFamily::Update() {
	if (parent) parent->Update();
	for (auto& child : childs) {
		child.Update();
	}
}
void GameObjectManager::GameObjectFamily::PostUpdate() {
	if (parent) parent->PostUpdate();
	for (auto& child : childs) {
		child.PostUpdate();
	}
}

void GameObjectManager::GameObjectFamily::ImGuiUpdate(int num) {
	if (childs.empty()) 
	{
		ImGui::Text(("   " + std::to_string(num) + " :").c_str()); ImGui::SameLine();
		ImGuiOpenOption(num);
		return;
	}

	auto Tree = [&]()
		{
			bool flg = ImGui::TreeNode((std::to_string(num) + " :").c_str()); ImGui::SameLine();
			ImGuiOpenOption(num);
			return flg;
		};

	if (!Tree())return;

	int i = 0;
	for (auto& child : childs) {
		child.ImGuiUpdate(i++);
	}

	ImGui::TreePop();
}

void GameObjectManager::GameObjectFamily::ImGuiOpenOption(int num)
{
	std::string ImGuiID = parent->GetName() + std::to_string(num);
	if (ImGui::SmallButton((parent->GetName() + "##" + std::to_string(num)).c_str()))ImGui::OpenPopup(("Option##" + ImGuiID).c_str());
	if (!ImGui::BeginPopup(("Option##" + ImGuiID).c_str())) return;

	if (ImGui::MenuItem("Edit"))GameObjectManager::Instance().m_editObject = this;

	static std::string path;
	ImGui::InputText("##Path", &path);ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		nlohmann::json json = nlohmann::json::array();
		json.push_back(GetJson());
		OutPutJson(json,"GameObjectSet/" + path);
		ImGui::CloseCurrentPopup();
	}

	if (ImGui::MenuItem("Remove"))
		this->parent->Destroy();
	ImGui::EndPopup();

}

nlohmann::json GameObjectManager::GameObjectFamily::GetJson()
{
	nlohmann::json json;
	json["parent"] = parent->GetJson();

	json["Childs"] = nlohmann::json::array();
	for (auto& child : childs) {
		json["Childs"].push_back(child.GetJson());
	}
	return json;

	/*
	nlohmann::json json;
	json[parent->GetName()] = nlohmann::json::array();
	if (parent) parent->PostUpdate();
	for (auto& child : childs) {
		json[parent->GetName()].push_back(child.GetJson());
	}
	return json;
	*/
}
#pragma endregion