#include "GameObject.h"
#include "../../Object/Component/Component.h"
#include "Manager/GameObjectManager.h"
#include "../../Object/Component/Transform/Transform.h"

#define ITERATOR(x) for (auto&& it : m_cpList)if(it.get() != nullptr)it->

void GameObject::Draw()
{
	if (!m_bActive)return;
	if (m_bDestroy)return;
	ITERATOR(m_cpList)Draw();
}
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
	ITERATOR(m_cpList)PostUpdate();
}

void GameObject::Init(std::string _name)
{
	m_name = _name;

	m_trans = std::make_shared<Cp_Transform>();
	m_trans->SetOwner(weak_from_this());
	m_trans->SetIDName("Transform");

	std::ifstream file(JsonDataPath(m_name));
	if (file.is_open()) {
		file >> m_jsonData;
		LoadJson(m_jsonData);
	}
}

void GameObject::Init(nlohmann::json _json)
{
	m_trans = std::make_shared<Cp_Transform>();
	m_trans->SetOwner(weak_from_this());
	m_trans->SetIDName("Transform");

	m_jsonData = _json;
	LoadJson(_json);
}

void GameObject::LoadJson(nlohmann::json _json)
{
	m_jsonData = _json;

	m_trans->SetJson(m_jsonData["Component"][0]);
	m_trans->InitJson();

	m_tag = m_jsonData["Tag"];
	m_name = _json["Name"];
	AddComponents();
}

void GameObject::ImGuiUpdate()
{
	ImGui::InputText("Name", &m_name);

	ImGuiComponents();

	GameObjectManager::Instance().ImGuiAddComponent(weak_from_this());
}

#pragma region ComponentFns
std::shared_ptr<Component> GameObject::AddComponent(unsigned int _id, nlohmann::json _json)
{
	auto addCp = GameObjectManager::Instance().ToComponent(_id);
	addCp->SetIDName(GameObjectManager::Instance().ToTag(_id));
	addCp->SetID(_id);
	_json.is_null() ? ComponentInit(addCp) : ComponentInit(addCp, _json);
	return addCp;
}
std::shared_ptr<Component> GameObject::AddComponent(Component* _addCp)
{
	std::shared_ptr<Component> addCp(_addCp);
	unsigned int id = GameObjectManager::Instance().ToID(typeid(*addCp).name());
	addCp->SetIDName(GameObjectManager::Instance().ToTag(id));
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
	m_cpList.push_back(std::shared_ptr<Component>(_addCp));
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
	if(_parent.lock())m_trans->SetParent(_parent.lock()->GetTransform());
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

void GameObject::Release()
{
	if (!m_bSave)return;
	GetJson();
	std::ofstream file(JsonDataPath(m_name));

	if (file.is_open()) {
		file << m_jsonData.dump(4);  // 読みやすい形式でファイルに書き出す
		file.close();
	}
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

	int num = 0;
	for (auto&& it : m_cpList)
	{
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode((std::to_string(num++) + " : " + (*it).GetIDName()).c_str()))
		{
			ImGui::SetNextItemOpen(true, ImGuiCond_Once);
			(*it).ImGuiUpdate();
			ImGui::TreePop();
		}
	}

	ImGui::TreePop();
}