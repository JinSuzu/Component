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
	if (m_editObject == nullptr)return;

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
	if (!ImGui::TreeNode("CreateObject"))return;

	ImGui::SeparatorText("ComponentSet");

	static char path[50] = ""; ImGui::InputText("JsonPath", path, sizeof(path));
	static unsigned int state; int Max = ComponentMap::Instance().GetCompoNum();

	ImGui::BeginChild("##ComponentSet", ImVec2(350, 100),  ImGuiChildFlags_Border);
	for (int i = 0; i < Max; i++)
	{
		bool flg = state & (1 << i);
		if (i % 3)ImGui::SameLine();
		ImGui::Checkbox(ComponentMap::Instance().GetTag(1 << i).c_str(), &flg);

		if (flg)state |= (1 << i);
		if (!flg)state &= ~(1 << i);
	}
	ImGui::EndChild();

	ImGui::SameLine();
	if (ImGui::Button("Add"))
	{
		auto obj = CreateObject(path
			, _bOrigin ? nullptr : m_editObject);

		obj->AddComponents(state);
	}

	ImGui::TreePop();
}

void GameObjectManager::ImGuiAddComponent(std::weak_ptr<GameObject> _object)
{
	if (ImGuiTreeCenterButton("AddComponent"))ImGui::OpenPopup("Components");
	if (!ImGui::BeginPopup("Components"))return;

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
	std::ifstream file(JsonDataPath("Scene/Test"));
	if (!file.is_open())return;
	nlohmann::json json;
	file >> json;

	auto name = json.begin();
	while (name != json.end())
	{
		m_obList.push_back(GameObjectFamily(name->begin()));
		name++;
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

	std::ofstream file(JsonDataPath("Scene/Test"));

	if (file.is_open()) {
		file << json.dump(4);  // 読みやすい形式でファイルに書き出す
		file.close();
	}
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


std::shared_ptr<GameObject> GameObjectManager::CreateObject(std::string _tag, bool flg)
{
	auto object = std::make_shared<GameObject>();
	object->Init(_tag);
	if (flg)m_obList.push_back(GameObjectFamily(object));
	return object;
}

std::shared_ptr<GameObject> GameObjectManager::CreateObject(std::string _tag, GameObjectFamily* _family)
{
	auto object = std::make_shared<GameObject>();
	object->Init(_tag);
	if (_family) 
	{
		_family->childs.push_back(GameObjectFamily(object));
		object->SetParent(_family->parent);
	}
	
	else m_obList.push_back(GameObjectFamily(object));
	return object;
}

GameObjectManager::GameObjectFamily::GameObjectFamily(nlohmann::json::iterator json, GameObjectFamily* _parent)
	:parent(GameObjectManager::Instance().CreateObject(json.key()))
{
	parent->SetParent(_parent->parent);

	auto name = json->begin();
	while (name != json->end())
	{
		childs.push_back(GameObjectFamily(name->begin(), this));
		name++;
	}
}

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
	auto Tree = [&]()
		{
			bool flg = ImGui::TreeNode((std::to_string(num) + " :").c_str());
			ImGui::SameLine(); if (ImGui::SmallButton(("" + parent->GetName()).c_str()))GameObjectManager::Instance().m_editObject = this;
			return flg;
		};

	if (!Tree())return;

	int i = 0;
	for (auto& child : childs) {
		child.ImGuiUpdate(i++);
	}

	ImGui::TreePop();
}

nlohmann::json GameObjectManager::GameObjectFamily::GetJson()
{
	nlohmann::json json;
	json[parent->GetName()] = nlohmann::json::array();
	if (parent) parent->PostUpdate();
	for (auto& child : childs) {
		json[parent->GetName()].push_back(child.GetJson());
	}
	return json;
}
