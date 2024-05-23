#include "GameObjectManager.h"
#include "../../Component/AllComponentIncluder.h"
#include "../GameObject.h"

#define ITERATOR(x)					\
for (auto&& it : family.childs)		\
{									\
	if(it)							\
	{								\
		if (it->GetActive())		\
		{							\
			it->x();				\
		}							\
	}								\
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
	ImGui::BeginChild("##ObjectChild", ImVec2(350, 250), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY);
	{
		int obNum = 0;
		for (auto& family : m_obList)
		{
			family.ImGuiUpdate(obNum++);
		}
	}
	ImGui::EndChild();

	if (!EditObject())return;
	ImGui::SeparatorText("EditObject");

	auto& obj = EditObject()->parent;
	ImGui::InputText("Name", obj->WorkName());
	obj->ImGuiComponents();
	
	ImGui::BeginChild("Edit");
	{
		ImGuiAddComponent(obj);
		ImGuiCreateObject();
	}
	ImGui::EndChild();

}

void GameObjectManager::Load(std::string _path)
{
	LoadJson(_path);
}

void GameObjectManager::Release(std::string _path)
{
	nlohmann::json json;
	auto it = m_obList.begin();
	while (it != m_obList.end()) 
	{
		json.push_back(it->GetJson());
		it = m_obList.erase(it);
	}

	OutPutJson(json, _path);
}

std::shared_ptr<GameObject> GameObjectManager::CreateObject(std::string _tag,GameObjectFamily* _family,bool bPush)
{
	auto object = std::make_shared<GameObject>();
	object->Init(InPutJson(GameObjectManager::GetGameObjectPath() + _tag));
	object->SetParent(_family ? _family->parent : nullptr);

	if (!bPush) return object;

	if(_family)_family->childs.push_back(GameObjectFamily(object));
	else m_obList.push_back(GameObjectFamily(object));

	return object;
}

//GameObjectFamily用
std::shared_ptr<GameObject> GameObjectManager::CreateObject(nlohmann::json _json,GameObjectFamily* _family)
{
	auto object = std::make_shared<GameObject>();
	object->Init(_json);
	object->SetParent(_family ? _family->parent : nullptr);

	return object;
}

#pragma region GameObjectFamily

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

#pragma region	void Draw
void GameObjectManager::GameObjectFamily::PreDraw()
{
	if (parent)parent->PreDraw();
	for (auto& child : childs) child.PreDraw();
}
void GameObjectManager::GameObjectFamily::GenerateDepthMapFromLight()
{
	if (parent) parent->GenerateDepthMapFromLight();
	for (auto& child : childs) child.GenerateDepthMapFromLight();
}
void GameObjectManager::GameObjectFamily::DrawLit()
{
	if (parent) parent->DrawLit();
	for (auto& child : childs) child.DrawLit();
}
void GameObjectManager::GameObjectFamily::DrawUnLit()
{
	if (parent) parent->DrawUnLit();
	for (auto& child : childs) child.DrawUnLit();
}
void GameObjectManager::GameObjectFamily::DrawBright()
{
	if (parent) parent->DrawBright();
	for (auto& child : childs) child.DrawBright();
}
void GameObjectManager::GameObjectFamily::DrawSprite()
{
	if (parent) parent->DrawSprite();
	for (auto& child : childs) child.DrawSprite();
}
#pragma endregion

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
		OutPutJson(json,GetGameObjectSetPath() + path);
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

void GameObjectManager::LoadJson(std::string _path, bool _bOrigin)
{
	nlohmann::json json = InPutJson(_path);

	auto name = json.begin();
	while (name != json.end())
	{
		if (_bOrigin)m_obList.push_back(GameObjectFamily(name));
		else if(auto obj = EditObject())obj->childs.push_back(GameObjectFamily(name, obj));
		name++;
	}
}
void GameObjectManager::ImGuiCreateObject(bool _bOrigin)
{
	ImGui::SeparatorText(("CreateObject On" + std::string(_bOrigin ? "Root" : "Branch")).c_str());

	if (ImGui::BeginTabBar("CreateObject##"))
	{
		static std::string path = "";
		if (ImGui::BeginTabItem("Custom"))
		{
			ImGui::InputText("Name", &path);

			unsigned int state = ComponentMap::Instance().ImGuiComponentSet();
			if (ImGui::Button("Create"))
			{
				auto object = CreateObject(path, _bOrigin ? nullptr : EditObject());
				object->AddComponents(state);
				object->SetName(path);
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("ImportPreSet"))
		{
			ImGui::InputText("PreSetPath", &path);

			if (ImGui::Button("Create"))LoadJson(GameObjectManager::GetGameObjectSetPath() + path, _bOrigin);

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
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
