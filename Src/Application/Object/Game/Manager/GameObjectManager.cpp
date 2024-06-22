#include "GameObjectManager.h"
#include "../../Component/AllComponentIncluder.h"
#include "../../../SceneBase/Manager/SceneManager.h"
#include "../../../SceneBase/SceneBase.h"
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
	//ImGuiCreateObject(true);
	ImGui::SeparatorText(("ObjectList" + std::to_string(m_objectList.size())).c_str());
	ImGui::BeginChild("##ObjectChild", ImVec2(425, 250), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY);
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
	ImGui::EndChild();

	if (EditObject().expired())return;
	ImGui::SeparatorText("EditObject");

	//ImGui::BeginChild("Edit", ImVec2(), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY);
	{
		ImGuiGameObject(EditObject());
	}
	//ImGui::EndChild();
}

void GameObjectManager::Load(std::string _path)
{
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
		OutPutJson(json, _path);
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
	return CreateObject(InPutJson(GameObjectManager::GetGameObjectPath() + _tag), _parent, bPush);
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
	nlohmann::json json = InPutJson(_path);
	auto name = json.begin();
	assert(name != json.end() && "not found json");
	while (name != json.end())
	{
		std::weak_ptr<GameObject>  edit = EditObject().lock();
		std::shared_ptr<GameObject>object = CreateObject(*name);
		if (edit.lock() && !_bOrigin)
		{
			object->SetParent(edit);
			edit.lock()->GetChilds().push_back(object);
		}
		name++;
	}
}
void GameObjectManager::ImGuiCreateObject()
{
	ImGui::SeparatorText("CreateObject");

	if (ImGui::BeginTabBar("CreateObject"))
	{
		static std::string path = "";
		if (ImGui::BeginTabItem("Custom"))
		{
			ImGui::InputText("Name", &path);
			unsigned int state = RegisterComponent::Instance().ImGuiComponentSet();
			if (ImGui::Button("Create"))
			{
				if (EditObject().lock())ImGui::OpenPopup("CCreate");
				else
				{
					std::shared_ptr<GameObject>object = CreateObject(path);
					object->AddComponents(state);
					object->SetName(path);
				}
			}

			if (ImGui::BeginPopup("CCreate"))
			{
				if (ImGui::Selectable("Root"))
				{
					std::shared_ptr<GameObject>object = CreateObject(path);
					object->AddComponents(state);
					object->SetName(path);
				}
				if (ImGui::Selectable("Branch"))
				{
					std::shared_ptr<GameObject>object = CreateObject(path, EditObject());
					object->AddComponents(state);
					object->SetName(path);
				}
				ImGui::EndPopup();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("ImportPreSet"))
		{
			ImGui::InputText("PreSetPath", &path);


			if (ImGui::Button("Create"))
			{
				if (EditObject().lock())ImGui::OpenPopup("Create");
				else LoadJson(GameObjectManager::GetGameObjectSetPath() + path);
			}

			if (ImGui::BeginPopup("Create"))
			{
				if (ImGui::Selectable("Root"))LoadJson(GameObjectManager::GetGameObjectSetPath() + path);
				if (ImGui::Selectable("Branch"))LoadJson(GameObjectManager::GetGameObjectSetPath() + path, false);

				ImGui::EndPopup();
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

void GameObjectManager::ImGuiGameObject(std::weak_ptr<GameObject> _object)
{
	ImGui::InputText("Name", _object.lock()->WorkName());
	_object.lock()->ImGuiComponents();

	if (std::shared_ptr<Component> compo = RegisterComponent::Instance().ImGuiAddComponent())_object.lock()->AddComponent(compo);
}
