#include "GameObjectManager.h"
#include "../../Component/AllComponentIncluder.h"
#include "../GameObject.h"

#include "../../../main.h"

void GameObjectManager::PreUpdate()
{
	for (auto it = m_objectList.begin(); it != m_objectList.end();)
	{
		std::shared_ptr<GameObject>object = *it;
		if (object->GetDestroy())
		{
			it = m_objectList.erase(it);
			continue;
		}

		object->PreUpdate();
		it++;
	}
}
void GameObjectManager::Update() { for (auto& object : m_objectList)object->Update(); }
void GameObjectManager::PostUpdate() { for (auto& object : m_objectList)object->PostUpdate(); }

void GameObjectManager::Load(std::string _path)
{
	Score::Instance().Reset();
	LoadJson(_path);
}
void GameObjectManager::Load(std::list<std::shared_ptr<GameObject>>& _list)
{
	Score::Instance().Reset();
	m_objectList.clear();
	m_objectList = _list;
}
void GameObjectManager::Release(std::string _path, bool _enableSave)
{
	if (_enableSave)
	{
		nlohmann::json json = nlohmann::json::array();
		for (auto& object : m_objectList)
		{
			if (!object->GetAbleSave())continue;
			if (object->GetParent().expired())
			{
				if (object->GetAbleSave())json.push_back(object->OutPutFamilyJson());
			}
		}
		Utility::JsonHelper::OutputJson(json, _path);
	}

	m_objectList.clear();
}

bool GameObjectManager::RayHit(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults)
{
	bool flg = false;

	auto it = m_colliderList.begin();
	while (it != m_colliderList.end())
	{
		if (it->expired())
		{
			it = m_colliderList.erase(it);
			continue;
		}

		flg |= (*it).lock()->Intersects(targetShape, pResults);
		it++;
	}

	return flg;
}
bool GameObjectManager::RayHit(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults)
{
	bool flg = false;

	auto it = m_colliderList.begin();
	while (it != m_colliderList.end())
	{
		if (it->expired())
		{
			it = m_colliderList.erase(it);
			continue;
		}

		flg |= (*it).lock()->Intersects(targetBox, pResults);
		it++;
	}

	return flg;
}
bool GameObjectManager::RayHit(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults)
{
	bool flg = false;

	auto it = m_colliderList.begin();
	while (it != m_colliderList.end())
	{
		if (it->expired())
		{
			it = m_colliderList.erase(it);
			continue;
		}

		flg |= (*it).lock()->Intersects(targetShape, pResults);
		it++;
	}

	return flg;
}

std::shared_ptr<GameObject> GameObjectManager::CreateObject(std::string _tag, std::weak_ptr<GameObject> _parent, bool bPush)
{
	return CreateObject(Utility::JsonHelper::InputJson(_tag), _parent, bPush);
}
std::shared_ptr<GameObject> GameObjectManager::CreateObject(nlohmann::json _json, std::weak_ptr<GameObject> _parent, bool bPush, std::list<std::shared_ptr<GameObject>>* _result)
{
	auto object = std::make_shared<GameObject>();
	if (_parent.lock())
	{
		object->SetParent(_parent);
		_parent.lock()->AddChilds(object);
	}

	object->Init(_json);

	if (_result)_result->push_back(object);
	if (bPush)GameObjectManager::Instance().m_objectList.push_back(object);

	for (auto& child : _json["Childs"])GameObjectManager::Instance().CreateObject(child, object, bPush, _result);

	return object;
}

void GameObjectManager::LoadJson(std::string _path, bool _bOrigin)
{
	nlohmann::json json = Utility::JsonHelper::InputJson(_path);
	auto name = json.begin();
	if (name == json.end())Application::Instance().AddLog("not found json by GameObjectManager");
	while (name != json.end())
	{
		std::shared_ptr<GameObject>object = CreateObject(*name);
		name++;
	}
}

void GameObjectManager::ImGuiGameObject(std::weak_ptr<GameObject> _object)
{
	ImGui::InputText("Name", _object.lock()->WorkName());
	_object.lock()->ImGuiComponents();
	if (std::shared_ptr<Component> compo = RegisterComponent::Instance().ImGuiAddComponent())_object.lock()->AddComponent(compo);
}
