#include "GameObject.h"
#include "../../SceneBase/Manager/SceneManager.h"
#include "../../SceneBase/SceneBase.h"
#include "../../Object/Component/Component.h"
#include "Manager/GameObjectManager.h"
#include "../../Object/Component/Transform/Transform.h"
#include "../../Object/Component/AllComponentIncluder.h"

#define ITERATOR(x) for (auto&& it : m_cpList)if(it.get() != nullptr)it->


void GameObject::PreUpdate()
{
	if (!m_bActive)return;

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
	m_trans->SetOwner(WeakThisPtr(this));
	m_trans->SetIDName("Transform");

	if (_json.is_null())return;
	m_jsonData = _json["Parent"];

	m_trans->SetJson(m_jsonData["Component"][0]);
	m_trans->InitJson();

	m_tag = m_jsonData["tag"];
	m_name = m_jsonData["name"];
	AddComponents();

	for (auto& child : _json["Childs"])
	{
		std::shared_ptr<GameObject> object =
			SceneManager::Instance().GetNowScene().lock()
			->GetGameObject().CreateObject(child);

		object->SetParent(WeakThisPtr(this));
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
		if (ImGui::MenuItem("Edit"))GameObjectManager::EditObject() = WeakThisPtr(this);

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
std::shared_ptr<Component> GameObject::AddComponent(std::string _name, nlohmann::json _json)
{
	auto addCp = RegisterComponent::Instance().CreateComponent(_name);
	ComponentInit(addCp, _json);
	return addCp;
}
std::shared_ptr<Component> GameObject::AddComponent(UINT _id, nlohmann::json _json)
{
	auto addCp = RegisterComponent::Instance().CreateComponent(_id);
	ComponentInit(addCp, _json);
	return addCp;
}
std::shared_ptr<Component> GameObject::AddComponent(std::shared_ptr<Component> _add)
{
	_add->CheckIDName(PickName(typeid(*_add.get()).name(), '_'));
	ComponentInit(_add, nlohmann::json());
	return _add;
}

std::list<std::shared_ptr<Component>> GameObject::AddComponents(unsigned int _id)
{
	std::list<std::shared_ptr<Component>>list;

	int Max = RegisterComponent::Instance().GetCompoNum();
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
			if (Key.key() == "IDName")
			{
				std::shared_ptr<Component> addCp = AddComponent(json["IDName"].get<std::string>(), json);
				list.push_back(addCp);
				break;
			}
			Key++;
		}
	}

	return list;
}

void GameObject::ComponentInit(std::shared_ptr<Component>& _addCp, nlohmann::json _json)
{
	m_cpList.push_back(_addCp);
	_addCp->SetOwner(WeakThisPtr(this));
	_addCp->Start();
	if (_json.is_null())return;
	_addCp->SetJson(_json);
	_addCp->InitJson();
}
std::shared_ptr<GameObject> GameObject::Initialize(std::weak_ptr<GameObject> _parent)
{
	std::shared_ptr<GameObject>	clone = std::make_shared<GameObject>();
	nlohmann::json json;

	SceneManager::Instance().GetNowScene().lock()->GetGameObject().AddObject(clone);
	if (_parent.lock())
	{
		clone->SetParent(_parent);
		_parent.lock()->AddChilds(clone);
	}

	return clone;
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
		json["IDName"] = it->GetIDName();
		component.push_back(json);
	}
	m_jsonData["tag"] = m_tag;
	m_jsonData["Component"] = component;
	m_jsonData["name"] = m_name;
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
	while (it != m_cpList.end() && !(*it)->CheckIDName(PickName(_tag, '_'))) { it++; }

	if (it == m_cpList.end())
	{
		assert(false && "GetComponent失敗！");
		return std::shared_ptr<Component>();
	}
	return  *it;
}
std::list<std::shared_ptr<Component>> GameObject::SearchTags(std::string _tag)
{
	auto&& it = m_cpList.begin();
	std::list<std::shared_ptr<Component>> list;
	while (it != m_cpList.end())
	{
		if ((*it)->CheckIDName(PickName(_tag, '_')))list.push_back(*it);
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