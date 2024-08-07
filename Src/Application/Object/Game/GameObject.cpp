#include "GameObject.h"
#include "Manager/GameObjectManager.h"

#include "../../main.h"

#include "../../../System/EditorWindow/Prefab/Prefab.h"

#define ITERATOR(x) for (auto&& it : m_cpList)it->


void GameObject::PreUpdate()
{
	if (!m_bActive)return;
	if (m_bDestroy)return;
	ITERATOR(m_cpList)PreUpdate();
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
	auto cmp = m_cpList.begin();
	while (cmp != m_cpList.end())
	{
		if ((*cmp)->GetDestroy())
		{
			cmp = m_cpList.erase(cmp);
			continue;
		}
		(*cmp)->UpdateRender();
		cmp++;
	}
}

void GameObject::Init()
{
	m_trans = std::make_shared<TransformComponent>();
	((std::shared_ptr<Component>)m_trans)->m_owner = WeakThisPtr(this);
	if (m_parent.lock())m_trans->SetParent(m_parent.lock()->GetTransform());
	m_trans->Start();
}

void GameObject::ChangeParent(std::weak_ptr<GameObject> _parent, bool _push)
{
	std::weak_ptr<GameObject> me = WeakThisPtr(this);
	if (m_parent.lock())
	{
		std::list<std::weak_ptr<GameObject>>::iterator it = m_parent.lock()->WorkChilds().begin();
		while (it != m_parent.lock()->GetChilds().end())
		{
			if ((*it).lock() == me.lock())
			{
				m_parent.lock()->WorkChilds().erase(it);
				break;
			}
			it++;
		}

	}

	if (_parent.lock())_parent.lock()->AddChilds(me);

	m_parent = _parent;
	m_trans->SetParent(m_parent.lock() ? m_parent.lock()->GetTransform() : std::weak_ptr<TransformComponent>());
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

nlohmann::json GameObject::Serialize()
{
	if (!m_bSave)return nlohmann::json();
	nlohmann::json component;
	component.push_back(m_trans->Serialize());

	for (auto&& it : m_cpList)
	{
		nlohmann::json json = it->Serialize();
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
nlohmann::json GameObject::SerializeFamily()
{
	nlohmann::json json = Serialize();
	for (auto& child : m_childs)
	{
		if (child.expired())continue;
		if (child.lock()->GetAbleSave())json["Childs"].push_back(child.lock()->SerializeFamily());
	}

	return json;
}
void GameObject::Deserialize(nlohmann::json _json)
{
	if (_json.is_null())return;
	nlohmann::json myData = _json["_Data"];

	m_trans->LoadJson(myData["Component"][0]);

	m_tag = myData["tag"];
	m_name = myData["name"];

	m_cpList.clear();
	for (auto& json : myData["Component"])
	{
		auto Key = json.begin();
		while (Key != json.end())
		{
			if (Key.key() == "ID")
			{
				AddComponent(json["ID"].get<int>(), json);
				break;
			}
			Key++;
		}
	}

	for (auto& it : m_cpList)
	{
		it->Start();
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
