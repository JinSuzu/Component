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
		if (family->parent->GetDestroy())
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
	ImGui::BeginChild("##ObjectChild", ImVec2(350, 500), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY);

	int obNum = 0;
	for (auto& family : m_obList)
	{
		family.ImGuiUpdate(obNum++);
	}

	ImGui::EndChild();


	ImGui::SeparatorText("EditObject");
	if (!EditObject())return;

	ImGui::SetNextItemOpen(true);
	if (ImGui::TreeNode(EditObject()->parent->GetName().c_str()))
	{
		EditObject()->parent->ImGuiUpdate();
		ImGuiCreateObject();
		ImGui::TreePop();
	}
}

void GameObjectManager::Init()
{
	LoadJson("Scene/Test");
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

//GameObjectFamily用
std::shared_ptr<GameObject> GameObjectManager::CreateObject(std::string _tag,GameObjectFamily* _family,bool bPush)
{
	auto object = std::make_shared<GameObject>();
	object->Init(InPutJson(_tag));
	object->SetParent(_family ? _family->parent : nullptr);

	if (!bPush) return object;

	if(_family)_family->childs.push_back(GameObjectFamily(object));
	else m_obList.push_back(GameObjectFamily(object));

	return object;
}
std::shared_ptr<GameObject> GameObjectManager::CreateObject(nlohmann::json _json,GameObjectFamily* _family)
{
	auto object = std::make_shared<GameObject>();
	object->Init(_json);
	object->SetParent(_family ? _family->parent : nullptr);

	if (_family)_family->childs.push_back(GameObjectFamily(object));

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
	if (parent) parent->PreUpdate();
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

	if (ImGui::MenuItem("Edit")) 
	{
		GameObjectManager::EditObject(this);
		
	}

	static std::string path;
	ImGui::InputText("##Path", &path);ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		nlohmann::json json = nlohmann::json::array();
		json.push_back(GetJson());
		OutPutJson(json,"GameObject/Set/" + path);
		ImGui::CloseCurrentPopup();
	}

	if (ImGui::MenuItem("Remove"))
		this->parent->Destroy();
	ImGui::EndPopup();

}

nlohmann::json GameObjectManager::GameObjectFamily::GetJson()
{
	nlohmann::json json;
	json["Parent"] = parent->GetJson();

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
void GameObjectManager::ImGuiCreateObject(bool _bOrigin)
{
	ImGui::SeparatorText("CreateObject");

	if (ImGui::BeginTabBar("CreateObject"))
	{
		static std::string path = "";
		if (ImGui::BeginTabItem("Custom"))
		{
			ImGui::InputText("Name", &path);

			unsigned int state = ComponentMap::Instance().ImGuiComponentSet();
			if (ImGui::Button("Create"))CreateObject(GameObjectManager::GetGameObjectPath() + path, _bOrigin ? nullptr : EditObject())
				->AddComponents(state);
			
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("ImportPreSet"))
		{
			ImGui::InputText("PreSetPath", &path);

			if (ImGui::Button("Create"))LoadJson(GameObjectManager::GetGameObjectSetPath() + path,_bOrigin);
			
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

}
void GameObjectManager::LoadJson(std::string _path, bool _bOrigin)
{
	nlohmann::json json = InPutJson(_path);
	auto name = json.begin();
	while (name != json.end())
	{
		(_bOrigin ? m_obList : EditObject()->childs).push_back(GameObjectFamily(name, EditObject()));
		name++;
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