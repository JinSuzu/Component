#include "GameObject.h"
#include "../../SceneBase/Manager/SceneManager.h"
#include "../../SceneBase/SceneBase.h"
#include "../../Object/Component/Component.h"
#include "Manager/GameObjectManager.h"
#include "../../Object/Component/Transform/Transform.h"
#include "../../Object/Component/Camera/Camera.h"
#include "../../Object/Component/Collider/Collider.h"
	
#define ITERATOR(x) for (auto&& it : m_cpList)if(it.get() != nullptr)it->


void GameObject::PreUpdate()
{
	if (!m_bActive)return;
	if (m_bDestroy)return;

	auto it = m_cpList.begin();
	while (it != m_cpList.end())
	{
		if ((*it)->GetDestroy())
		{
			it = m_cpList.erase(it);
			continue;
		}
		(*it)->PreUpdate();
		it++;
	}
}
void GameObject::Update()
{
	if (!m_bActive)return;
	if (m_bDestroy)return;
	ITERATOR(m_cpList)Update();
}
void GameObject::PostUpdate()
{
	if (!m_bActive)return;
	if (m_bDestroy)return;
	ITERATOR(m_cpList)PostUpdate();
}

void GameObject::Init(nlohmann::json _json)
{
	m_trans = std::make_shared<Cp_Transform>();
	m_trans->SetOwner(weak_from_this());
	m_trans->SetIDName("Transform");

	if (_json.is_null())return;
	m_jsonData = _json["Parent"];

	m_trans->SetJson(m_jsonData["Component"][0]);
	m_trans->InitJson();

	m_tag = m_jsonData["Tag"];
	m_name = m_jsonData["Name"];
	AddComponents();

	for (auto& child : _json["Childs"]) 
	{
		std::shared_ptr<GameObject> object = 
			SceneManager::Instance().GetNowScene().lock()
				->GetGameObject().CreateObject(child);

		object->SetParent(weak_from_this());
		m_childs.push_back(object);
	}
}

void GameObject::ImGuiUpdate(int num)
{
	if (m_childs.empty())
	{
		ImGui::Text(("   " + std::to_string(num) + " :").c_str()); ImGui::SameLine();
		ImGuiOpenOption();
		return;
	}
	else
	{
		bool flg = ImGui::TreeNode((std::to_string(num) + " :").c_str());
		ImGui::SameLine(); ImGuiOpenOption();
		if (!flg)return;

		int i = 0;
		std::list<std::weak_ptr<GameObject>>::iterator child = m_childs.begin();
		while (child != m_childs.end())
		{
			if (child->expired()) 
			{
				child = m_childs.erase(child);
				continue;
			}

			(*child).lock()->ImGuiUpdate(i++);
			child++;
		}
		ImGui::TreePop();
	}
}
void GameObject::ImGuiOpenOption()
{
	std::string ImGuiID = "##" + std::to_string(m_instanceID);
	if (ImGui::SmallButton((m_name + ImGuiID).c_str()))ImGui::OpenPopup(("Option" + ImGuiID).c_str());
	if (!ImGui::BeginPopup(("Option" + ImGuiID).c_str())) return;
	{
		if (ImGui::MenuItem("Edit"))GameObjectManager::EditObject() = weak_from_this();

		static std::string path;
		ImGui::InputText("##Path", &path);
		if (ImGui::SameLine(); ImGui::Button("Save"))
		{
			nlohmann::json json = nlohmann::json::array();
			json.push_back(OutPutFamilyJson());
			OutPutJson(json, GameObjectManager::GetGameObjectSetPath() + path);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::MenuItem("Remove"))Destroy();
	}
	ImGui::EndPopup();
}

#pragma region ComponentFns
std::shared_ptr<Component> GameObject::AddComponent(unsigned int _id, nlohmann::json _json)
{
	auto addCp = GameObjectManager::ToComponent(_id);
	addCp->SetIDName(GameObjectManager::ToTag(_id));
	addCp->SetID(_id);
	_json.is_null() ? ComponentInit(addCp) : ComponentInit(addCp, _json);
	return addCp;
}
std::shared_ptr<Component> GameObject::AddComponent(Component* _addCp)
{
	std::shared_ptr<Component> addCp(_addCp);
	unsigned int id = GameObjectManager::ToID(typeid(*addCp).name());
	addCp->SetIDName(GameObjectManager::ToTag(id));
	addCp->SetID(id);
	ComponentInit(addCp);
	return addCp;
}

std::list<std::shared_ptr<Component>> GameObject::AddComponents(unsigned int _id)
{
	std::list<std::shared_ptr<Component>>list;

	int Max = std::log2((double)ComponentID::MaxID);
	for (int i = 0; i < Max; i++)
	{
		unsigned int search = 1 << i;
		if (!(_id & search))continue;

		auto addCp = AddComponent(search);
		list.push_back(addCp);
	}

	return list;
}
std::list<std::shared_ptr<Component>> GameObject::AddComponents()
{
	std::list<std::shared_ptr<Component>>list;

	if (m_jsonData.is_null())return list;
	for (auto& json : m_jsonData["Component"])
	{
		auto Key = json.begin();
		while (Key != json.end())
		{
			if (Key.key() == "ID")
			{
				auto addCp = AddComponent(json["ID"], json);
				list.push_back(addCp);
				break;
			}
			Key++;
		}
	}

	return list;
}

void GameObject::ComponentInit(std::shared_ptr<Component>& _addCp)
{
	switch (_addCp->GetID())
	{
	case ComponentID::Camera:
		m_camera = static_pointer_cast<Cp_Camera>(_addCp);
		break;
	case ComponentID::Collider:
		if (SearchID(ComponentID::Collider)) _addCp->Destroy();
		else 
		{
			SceneManager::Instance().GetNowScene().lock()
				->GetGameObject().AddColliderList(static_pointer_cast<Cp_Collider>(_addCp));
		}
		break;
	}

	m_cpList.push_back(_addCp);
	_addCp->SetOwner(weak_from_this());
	_addCp->Start();
}
void GameObject::ComponentInit(std::shared_ptr<Component>& _addCp, nlohmann::json& _json)
{
	ComponentInit(_addCp);
	_addCp->SetJson(_json);
	_addCp->InitJson();
}
#pragma endregion

void GameObject::SetParent(std::weak_ptr<GameObject> _parent)
{
	if (_parent.lock())m_trans->SetParent(_parent.lock()->GetTransform());
	m_parent = _parent;
}

void GameObject::Destroy()
{
	Object::Destroy();
	for (auto& child : m_childs) 
	{
		child.lock()->Destroy();
	}
}

nlohmann::json GameObject::GetJson()
{
	if (!m_bSave)return m_jsonData;
	nlohmann::json component;
	component.push_back(m_trans->GetJson());

	for (auto&& it : m_cpList)
	{
		nlohmann::json json = it->GetJson();
		json["ID"] = it->GetID();
		component.push_back(json);
	}

	m_jsonData["Name"] = m_name;
	m_jsonData["Tag"] = m_tag;
	m_jsonData["Component"] = component;

	return m_jsonData;
}

nlohmann::json GameObject::OutPutFamilyJson()
{
	nlohmann::json json;
	json["Parent"] = GetJson();
	json["Childs"] = nlohmann::json::array();
	for (auto& child : m_childs) 
	{
		json["Childs"].push_back(child.lock()->OutPutFamilyJson());
	}

	return json;
}

std::shared_ptr<Component> GameObject::SearchTag(std::string _tag)
{
	auto&& it = m_cpList.begin();
	while (it != m_cpList.end() && !(*it)->CheckIDName(PickName(_tag,'_'))) { it++; }

	return  *it;
}
std::list<std::shared_ptr<Component>> GameObject::SearchTags(std::string _tag)
{
	auto&& it = m_cpList.begin();
	std::list<std::shared_ptr<Component>> list;
	while (it != m_cpList.end())
	{
		if((*it)->CheckIDName(PickName(_tag, '_')))list.push_back(*it);
		it++;
	}

	return list;
}
std::shared_ptr<Component> GameObject::SearchID(UINT _id)
{
	auto&& it = m_cpList.begin();
	while (it != m_cpList.end())
	{
		if ((*it)->GetID() == _id)return  *it;
		it++;
	}

	return std::shared_ptr<Component>();
}
std::list<std::shared_ptr<Component>> GameObject::SearchIDs(UINT _id)
{
	auto&& it = m_cpList.begin();
	std::list<std::shared_ptr<Component>> list;
	while (it != m_cpList.end())
	{
		if ((*it)->GetID() != _id)list.push_back(*it);
		it++;
	}

	return list;
}

void GameObject::ImGuiComponents()
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (!ImGui::TreeNode("Component"))return;
	{
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode(m_trans->GetIDName().c_str()))
		{
			m_trans->ImGuiUpdate();
			ImGui::TreePop();
		}


		int num = 0;
		for (auto&& it : m_cpList)
		{
			ImGui::SetNextItemOpen(true, ImGuiCond_Once);
			std::string ImGui = std::to_string(num++) + " : " + it->GetIDName();
			bool flg = ImGui::TreeNode(ImGui.c_str());
			if (ImGui::SameLine(); ImGui::SmallButton(("Remove##" + ImGui).c_str()))
			{
				it->Destroy();
			}


			if (flg)
			{
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				(*it).ImGuiUpdate();
				ImGui::TreePop();
			}
		}
	}

	ImGui::TreePop();
}

void GameObject::Release()
{
	if (!m_bSave)return;

	nlohmann::json json;
	json["Parent"] = GetJson();
	json["Childs"] = nlohmann::json::array();

	OutPutJson(json, GameObjectManager::GetGameObjectPath() + m_name);
}