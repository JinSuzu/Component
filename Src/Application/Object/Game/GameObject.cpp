﻿#include "GameObject.h"
#include "Manager/GameObjectManager.h"
#include "../Component/Transform/Transform.h"
#include "../Component/AllComponentIncluder.h"
#include "../../SceneBase/Manager/SceneManager.h"
#include "../../SceneBase/SceneBase.h"
#include "../../Object/Component/Component.h"
#include "../../ImGuiHelper/ImGuiEditor.h"
#include "../../main.h"

#define ITERATOR(x) for (auto&& it : m_cpList)if(it.get() != nullptr)it->


void GameObject::PreUpdate()
{
	if (!GetActive())return;

	auto cmp = m_cpList.begin();
	while (cmp != m_cpList.end())
	{
		if ((*cmp)->GetDestroy())
		{
			cmp = m_cpList.erase(cmp);
			continue;
		}
		(*cmp)->PreUpdate();
		cmp++;
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
	if (m_parent.lock())m_trans->SetParent(m_parent.lock()->GetTransform());
	m_trans->Start();

	if (_json.is_null())return;
	m_jsonData = _json["Parent"];

	m_trans->SetJson(m_jsonData["Component"][0]);
	m_trans->InitJson();

	m_tag = m_jsonData["tag"];
	m_name = m_jsonData["name"];
	AddComponents();

	for (auto& child : _json["Childs"])
	{
		SceneManager::Instance().m_objectMgr->CreateObject(child, WeakThisPtr(this));
	}
}

//Inspector用
void GameObject::ImGuiUpdate(int num)
{
	int treeFlg = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
	if (m_childs.empty())treeFlg = ImGuiTreeNodeFlags_Leaf;

	bool flg = ImGui::TreeNodeEx((std::to_string(num) + " :" + m_name + "##" + std::to_string(GetInstanceID())).c_str(), treeFlg);
	if ((ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)))Application::Instance().GetEditor().lock()->SetEditObject(WeakThisPtr(this));
	if (Editor::SourceGameObject(WeakThisPtr(this)));
	else  Editor::TargetGameObject(WeakThisPtr(this));

	if (flg)
	{
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

#pragma endregion
void GameObject::SetUpParent(std::weak_ptr<GameObject> _parent, bool _push)
{
	std::weak_ptr<GameObject> me = WeakThisPtr(this);
	if (m_parent.lock())
	{
		std::list<std::weak_ptr<GameObject>>::iterator it = m_parent.lock()->GetChilds().begin();
		while (it != m_parent.lock()->GetChilds().end())
		{
			if ((*it).lock() == me.lock())
			{
				m_parent.lock()->GetChilds().erase(it);
				break;
			}
			it++;
		}

	}

	if (_parent.lock())_parent.lock()->AddChilds(me);

	m_parent = _parent;
	m_trans->SetParent(m_parent.lock() ? m_parent.lock()->GetTransform() : std::weak_ptr<Cp_Transform>());
}

void GameObject::AddChilds(std::weak_ptr<GameObject> _child)
{
	m_addFamily = true;
	_child.lock()->SetParent(WeakThisPtr(this));
	m_childs.push_back(_child);
}

void GameObject::Destroy()
{
	if (m_bDestroy)return;
	Object::Destroy();

	for (auto& cmp : m_cpList)
	{
		if (cmp)cmp->Destroy();
	}
	for (auto& child : m_childs)
	{
		if (child.lock())child.lock()->Destroy();
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
	nlohmann::json json;
	json["Parent"] = m_jsonData;
	json["Childs"] = nlohmann::json::array();
	return json;
}

nlohmann::json GameObject::OutPutFamilyJson()
{
	nlohmann::json json = GetJson();
	for (auto& child : m_childs)
	{
		if (child.expired())continue;
		if (child.lock()->GetAbleSave())json["Childs"].push_back(child.lock()->OutPutFamilyJson());
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
	if (ImGui::TreeNode(m_trans->GetIDName().c_str()))
	{
		m_trans->ImGuiUpdate();
		ImGui::TreePop();
	}

	int num = 0; bool deleteFlg = false;
	for (auto&& it : m_cpList)
	{
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		std::string ImGui = std::to_string(num++) + " : " + it->GetIDName();
		bool flg = ImGui::TreeNode(ImGui.c_str());
		if (ImGui::SameLine(); ImGui::SmallButton(("Remove##" + ImGui).c_str()))
		{
			deleteFlg = true;
			it->Destroy();
		}

		if (flg)
		{
			ImGui::SetNextItemOpen(true, ImGuiCond_Once);
			(*it).ImGuiUpdate();
			ImGui::TreePop();
		}
	}

	if (deleteFlg == false)return;
	auto it = m_cpList.begin();
	while (it != m_cpList.end())
	{
		if ((*it)->GetDestroy())
		{
			it = m_cpList.erase(it);
			continue;
		}
		it++;
	}
}

void GameObject::SetActive(bool _flg)
{
	Object::SetActive(_flg);
	for (auto& it : m_cpList)
	{
		it->SetActive(_flg);
	}

	for (auto& child : m_childs)
	{
		if (child.expired())continue;
		child.lock()->SetActive(_flg);
	}
}

bool GameObject::GetActive()
{
	if (m_parent.lock())
	{
		m_bActive = m_bActive && m_parent.lock()->GetActive();
	}
	return m_bActive;
}
