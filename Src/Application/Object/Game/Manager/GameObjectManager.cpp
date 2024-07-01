#include "GameObjectManager.h"
#include "../../Component/AllComponentIncluder.h"
#include "../../../SceneBase/Manager/SceneManager.h"
#include "../../../SceneBase/SceneBase.h"
#include "../../../ImGuiHelper/ImGuiEditor.h"
#include "../GameObject.h"

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

void GameObjectManager::ImGuiUpdate()
{
	int obNum = 0; std::list<std::shared_ptr<GameObject>>::iterator it = m_objectList.begin();
	while (it != m_objectList.end())
	{
		if ((*it)->GetDestroy())
		{
			it = m_objectList.erase(it);
			continue;
		}
		if ((*it)->GetParent().expired())(*it)->ImGuiUpdate(obNum++);
		it++;
	}
}

void GameObjectManager::Load(std::string _path)
{
	Score::Instance().Reset();
	LoadJson(_path);
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
		MyJson::OutPutJson(json, _path);
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
	return CreateObject(MyJson::InPutJson(_tag), _parent, bPush);
}
std::shared_ptr<GameObject> GameObjectManager::CreateObject(nlohmann::json _json, std::weak_ptr<GameObject> _parent, bool bPush)
{
	auto object = std::make_shared<GameObject>();
	if (_parent.lock())
	{
		object->SetParent(_parent);
		if (bPush)_parent.lock()->AddChilds(object);
	}

	object->Init(_json);
	if (bPush)SceneManager::Instance().m_objectMgr->m_objectList.push_back(object);

	return object;
}

void GameObjectManager::LoadJson(std::string _path, bool _bOrigin)
{
	nlohmann::json json = MyJson::InPutJson(_path);
	auto name = json.begin();
	assert(name != json.end() && "not found json");
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
