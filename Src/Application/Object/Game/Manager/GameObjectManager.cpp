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
	ImGuiCreateObject(true);

	ImGui::SeparatorText(("ObjectList" + std::to_string(m_objectList.size())).c_str());
	ImGui::BeginChild("##ObjectChild", ImVec2(350, 250), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY);
	{
		int obNum = 0;
		for (auto& object : m_objectList)
		{
			if (object->GetParent().expired())object->ImGuiUpdate(obNum++);
		}
	}
	ImGui::EndChild();

	if (EditObject().expired())return;
	ImGui::SeparatorText("EditObject");

	ImGui::BeginChild("Edit", ImVec2(), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY);
	{
		auto obj = EditObject().lock();
		ImGui::InputText("Name", obj->WorkName());
		obj->ImGuiComponents();

		if (std::shared_ptr<Component> compo = RegisterComponent::Instance().ImGuiAddComponent())obj->AddComponent(compo);
	}
	ImGui::EndChild();

	ImGui::BeginChild("CreateObject_branch", ImVec2(), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY);
	{
		ImGuiCreateObject(false);
	}
	ImGui::EndChild();
	//ImGui::Text("##");
}

void GameObjectManager::Load(std::string _path)
{
	LoadJson(_path);
}
void GameObjectManager::Release(std::string _path)
{
	nlohmann::json json = nlohmann::json::array();
	for (auto& object : m_objectList)
	{
		if (object->GetParent().expired())json.push_back(object->OutPutFamilyJson());
	}
	m_objectList.clear();
	OutPutJson(json, _path);
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

std::shared_ptr<GameObject> GameObjectManager::CreateObject(std::string _tag, bool bPush)
{
	return CreateObject(InPutJson(GameObjectManager::GetGameObjectPath() + _tag), bPush);
}
std::shared_ptr<GameObject> GameObjectManager::CreateObject(nlohmann::json _json, bool bPush)
{
	auto object = std::make_shared<GameObject>();
	object->Init(_json);
	if (bPush)SceneManager::Instance().GetNowScene().lock()->GetGameObject().m_objectList.push_back(object);
	else assert(false&&"生成しませんでした");
	return object;
}

void GameObjectManager::LoadJson(std::string _path, bool _bOrigin)
{
	nlohmann::json json = InPutJson(_path);
	auto name = json.begin();
	assert(name != json.end() && "not found json");
	while (name != json.end())
	{
		std::shared_ptr<GameObject>object = CreateObject(*name);
		if (auto edit = EditObject().lock(); edit && !_bOrigin)
		{
			object->SetParent(edit);
			edit->GetChilds().push_back(object);
		}
		name++;
	}
}
void GameObjectManager::ImGuiCreateObject(bool _bOrigin)
{
	ImGui::SeparatorText(("CreateObject On" + std::string(_bOrigin ? "Root" : "Branch")).c_str());
	std::shared_ptr<GameObject> object;

	if(ImGui::BeginTabBar(("CreateObject##" + std::string(_bOrigin ? "Root" : "Branch")).c_str()))
	{
		static std::string path = "";
		if (ImGui::BeginTabItem("Custom"))
		{
			ImGui::InputText("Name", &path);
			if (unsigned int state = RegisterComponent::Instance().ImGuiComponentSet(); ImGui::Button("Create"))
			{
				object = CreateObject(path);
				object->AddComponents(state);
				object->SetName(path);
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("ImportPreSet"))
		{
			ImGui::InputText("PreSetPath", &path);

			if (ImGui::Button("Create"))
			{
				LoadJson(GameObjectManager::GetGameObjectSetPath() + path, _bOrigin);
			}

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	if (object == nullptr)return;

	if (EditObject().lock() && !_bOrigin)
	{
		EditObject().lock()->AddChilds(object);
		object->SetParent(EditObject());
	}

}