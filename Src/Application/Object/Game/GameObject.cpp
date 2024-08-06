#include "GameObject.h"
#include "Manager/GameObjectManager.h"

#include "../../main.h"

#include "../../../System/EditorWindow/Prefab/Prefab.h"

#define ITERATOR(x) for (auto&& it : m_cpList)it->


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
	m_trans->MatrixUpdata();
	ITERATOR(m_cpList)PostUpdate();
}

void GameObject::UpdateRender()
{
	ITERATOR(m_cpList)UpdateRender();
}

void GameObject::Init(nlohmann::json _json)
{
	m_trans = std::make_shared<TransformComponent>();
	((std::shared_ptr<Component>)m_trans)->m_owner = WeakThisPtr(this);
	if (m_parent.lock())m_trans->SetParent(m_parent.lock()->GetTransform());
	m_trans->Start();

	if (_json.is_null())return;
	nlohmann::json myData = _json["_Data"];

	m_trans->LoadJson(myData["Component"][0]);

	m_tag = myData["tag"];
	m_name = myData["name"];

	for (auto& json : myData["Component"])
	{
		auto Key = json.begin();
		while (Key != json.end())
		{
			if (Key.key() == "ID")
			{
				std::shared_ptr<Component> addCp = AddComponent(json["ID"].get<int>(), json);
				break;
			}
			Key++;
		}
	}
}

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
	m_trans->SetParent(m_parent.lock() ? m_parent.lock()->GetTransform() : std::weak_ptr<TransformComponent>());
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
	if (!m_bSave)return nlohmann::json();
	nlohmann::json component;
	component.push_back(m_trans->GetJson());

	for (auto&& it : m_cpList)
	{
		nlohmann::json json = it->GetJson();
		json["ID"] = it->GetID();
		component.push_back(json);
	}
	nlohmann::json object;
	object["tag"] = m_tag;
	object["Component"] = component;
	object["name"] = m_name;
	nlohmann::json json;
	json["_Data"] = object;
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

std::shared_ptr<Component> GameObject::AddComponent(size_t _id, nlohmann::json _json)
{
	std::shared_ptr<Component> addCp = ComponentFactory::Instance().CreateComponent(_id);
	m_cpList.push_back(addCp);
	addCp->m_owner = WeakThisPtr(this);
	addCp->m_trans = m_trans;
	addCp->Start();
	if (!_json.is_null())
	{
		addCp->LoadJson(_json);
	}
	return addCp;
}

void GameObject::ImGuiComponents()
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode("Transform"))
	{
		m_trans->ImGuiUpdate();
		ImGui::TreePop();
	}

	int num = 0; bool deleteFlg = false;
	for (auto& it : m_cpList)
	{
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		std::string ImGui = std::to_string(num++) + " : " + ComponentFactory::Instance().GetRegistry().at(it->GetID()).name;
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
