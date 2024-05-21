#include "GameObject.h"
#include "../../Object/Component/Component.h"
#include "Manager/GameObjectManager.h"
#include "../../Object/Component/Transform/Transform.h"
#include "../../Object/Component/Draw/Draw.h"
#include "../../Object/Component/Camera/Camera.h"
	
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

#pragma region void Draw
void GameObject::PreDraw()
{
	if(!m_camera.expired())m_camera.lock()->PreDraw();
	auto draw = m_draws.begin();
	while (draw != m_draws.end())
	{
		if (draw->expired()) 
		{
			draw = m_draws.erase(draw);
			continue;
		}

		draw->lock()->PreDraw();
		draw++;
	}
}
void GameObject::GenerateDepthMapFromLight()
{
	for (auto& draw : m_draws)draw.lock()->GenerateDepthMapFromLight();
}
void GameObject::DrawLit()
{
	for (auto& draw : m_draws)draw.lock()->DrawLit();
}
void GameObject::DrawUnLit()
{
	for (auto& draw : m_draws)draw.lock()->DrawUnLit();
}
void GameObject::DrawBright()
{
	for (auto& draw : m_draws)draw.lock()->DrawBright();
}
void GameObject::DrawSprite()
{
	for (auto& draw : m_draws)draw.lock()->DrawSprite();
}
#pragma endregion

void GameObject::Init(nlohmann::json _json)
{
	m_trans = std::make_shared<Cp_Transform>();
	m_trans->SetOwner(weak_from_this());
	m_trans->SetIDName("Transform");

	m_jsonData = _json;
	if (m_jsonData.is_null())return;

	m_trans->SetJson(m_jsonData["Component"][0]);
	m_trans->InitJson();

	m_tag = m_jsonData["Tag"];
	m_name = _json["Name"];
	AddComponents();
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
	case ComponentID::Draw:
		m_draws.push_back(static_pointer_cast<Cp_Draw>(_addCp));
		break;
	case ComponentID::Camera:
		m_camera = static_pointer_cast<Cp_Camera>(_addCp);
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

nlohmann::json& GameObject::GetJson()
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

void GameObject::ImGuiComponents()
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (!ImGui::TreeNode("Component"))return;

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);

	if (ImGui::TreeNode(m_trans->GetIDName().c_str()))
	{
		m_trans->ImGuiUpdate();
		ImGui::TreePop();
	}

	auto Tree = [&](int _num,std::shared_ptr<Component> _component)
		{
			std::string ImGui = std::to_string(_num) + " : " + _component->GetIDName();
			bool flg = ImGui::TreeNode(ImGui.c_str());
			ImGui::SameLine(); if (ImGui::SmallButton(("Remove##" + ImGui).c_str()))
				_component->Destroy();
			return flg;
		};

	int num = 0;
	for (auto&& it : m_cpList)
	{
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (Tree(num++,it))
		{
			ImGui::SetNextItemOpen(true, ImGuiCond_Once);
			(*it).ImGuiUpdate();
			ImGui::TreePop();
		}
	}

	ImGui::TreePop();
}

void GameObject::Release()
{
	if (!m_bSave)return;
	OutPutJson(GetJson(), GameObjectManager::GetGameObjectPath() + m_name);
}