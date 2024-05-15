#include "GameObject.h"
#include "../../Object/Component/Component.h"
#include "Manager/GameObjectManager.h"
#include "../../Object/Component/Transform/Transform.h"

GameObject::GameObject()
{
	
}

#define ITERATOR(x) for (auto&& it : m_cpList)if(it.get() != nullptr)it->

void GameObject::Draw()
{
	if (!m_bActive)return;
	ITERATOR(m_cpList)Draw();
}

void GameObject::PreUpdate()
{
	if (!m_bActive)return;
	ITERATOR(m_cpList)PreUpdate();
}

void GameObject::Update()
{
	if (!m_bActive)return;
	ITERATOR(m_cpList)Update();
}

void GameObject::PostUpdate()
{
	if (!m_bActive)return;
	ITERATOR(m_cpList)PostUpdate();
}

void GameObject::Init(std::string _name)
{
	m_name = _name;
	m_filePath = "Asset/Data/" + m_name + ".json";

	std::ifstream file(m_filePath);
	if (file.is_open()){
		file >> m_jsonData;
	}

	m_trans = std::make_shared<Cp_Transform>();
	m_trans->SetOwner(weak_from_this());
	m_trans->SetIDName("Transform");
	m_trans->SetJson(m_jsonData["Component"][0]);
	m_trans->InitJson();

	if (file.is_open())
	{
		m_tag = m_jsonData["Tag"];
		AddComponents();
	}
}

void GameObject::ImGuiUpdate()
{
	//AddComponent追加予定
	if (ImGui::TreeNode(m_name.c_str()))
	{
		//ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
		if (ImGui::TreeNode("Transform"))
		{
			m_trans->ImGuiUpdate();
			ImGui::TreePop();
		}

		for (auto&& it : m_cpList)
		{
			//ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
			if (ImGui::TreeNode(it->GetIDName().c_str()))
			{
				(*it).ImGuiUpdate();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

std::weak_ptr<Cp_Transform> GameObject::GetspTransform()
{
	std::shared_ptr<Component> trans = *m_cpList.begin();
	return std::static_pointer_cast<Cp_Transform>(trans);
}

void GameObject::AddComponent(unsigned int _id)
{
	auto addCp = ObjectManager::Instance().ToComponent(_id);
	addCp->SetIDName(ObjectManager::Instance().ToTag(_id));
	addCp->SetID(_id);

	ComponentInit(addCp);
}

void GameObject::AddComponent(Component* _addCp)
{
	std::shared_ptr<Component> addCp(_addCp);
	unsigned int id = ObjectManager::Instance().ToID(typeid(*addCp).name());
	addCp->SetIDName(ObjectManager::Instance().ToTag(id));
	addCp->SetID(id);

	ComponentInit(addCp);
}

void GameObject::AddComponents(unsigned int _id)
{
	int Max = std::log2((double)ComponentID::MaxID);
	//for (unsigned int i = 1; i < ComponentID::MaxID; i = i << 1);
	for (int i = 0; i < Max; i++)
	{
		unsigned int search = 1 << i;
		if (!(_id & search))continue;

		AddComponent(search);
	}
}

void GameObject::AddComponents()
{
	for (auto& json : m_jsonData["Component"])
	for (auto& key : json)
	{
		if (key == "ID") 
		{
			AddComponent(json["ID"]);
			break;
		}
	}
}

bool GameObject::CheckID(unsigned int _id)const
{
	return m_compoID | _id;
}

bool GameObject::CheckIDs(unsigned int _id)const
{
	unsigned int search = m_compoID | _id;
	return m_compoID == search;
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

void GameObject::ComponentInit(std::shared_ptr<Component> _addCp)
{
	m_cpList.push_back(std::shared_ptr<Component>(_addCp));

	_addCp->SetOwner(weak_from_this());

	nlohmann::json json = SearchJson(m_jsonData, "ID", _addCp->GetID());
	if(!json.is_null())_addCp->SetJson(json);
	_addCp->InitJson();
	_addCp->Start();
}

void GameObject::Release()
{
	if (!m_bSave)return;
	nlohmann::json component;


	component.push_back(m_trans->GetJson());

	for (auto&& it : m_cpList) 
	{
		nlohmann::json json = it->GetJson();
		json["ID"] = it->GetID();
		component.push_back(json);
	}
	
	nlohmann::json finalJson;
	finalJson["Component"] = component;
	std::ofstream file(m_filePath);

	finalJson["Tag"] = m_tag;
	if (file.is_open()) {
		file << finalJson.dump(4);  // 読みやすい形式でファイルに書き出す
		file.close();
	}
}

/*
auto Fn = m_delayUpdateList.begin();
	while (Fn != m_delayUpdateList.end())
	{
		(*(Fn->lock()))();
		++Fn;

		continue;
		if (Fn->lock().get() == nullptr)
		{
			Fn = m_delayUpdateList.erase(Fn);
		}
		else
		{
			(*Fn->lock())();
			++Fn;
		}
	};
*/

