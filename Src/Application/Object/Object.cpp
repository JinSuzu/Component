#include "Object.h"
#include "../Component/Component.h"
#include "Manager/ObjectManager.h"
#include "../Component/Transform/Transform.h"

Object::Object()
{
	
}

#define ITERATOR(x) for (auto&& it : m_cpList)if(it.get() != nullptr)it->

void Object::Draw()
{
	if (m_bActive)ITERATOR(m_cpList)Draw();
}

void Object::PreUpdate()
{
	if (m_bActive)ITERATOR(m_cpList)PreUpdate();
}

void Object::Update()
{
	if (m_bActive)ITERATOR(m_cpList)Update();
}

void Object::PostUpdate()
{
	if (m_bActive)ITERATOR(m_cpList)PostUpdate();
}

void Object::Init(std::string _name)
{
	m_name = _name;
	m_filePath = "Asset/Data/" + m_name + ".json";

	std::ifstream file(m_filePath);
	if (file.is_open()){
		file >> m_jsonData;
	}

	m_trans = std::make_shared<Cp_Transform>();
	m_trans->SetOwner(weak_from_this());
	m_trans->InitJson();

	if (file.is_open())
	{
		m_tag = m_jsonData["Tag"];
		AddComponents(m_jsonData["ID"]);
	}
}

void Object::ImGuiUpdate()
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
			if (ImGui::TreeNode(it->GetTag().c_str()))
			{
				(*it).ImGuiUpdate();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

std::weak_ptr<Cp_Transform> Object::GetspTransform()
{
	std::shared_ptr<Component> trans = *m_cpList.begin();
	return std::static_pointer_cast<Cp_Transform>(trans);
}

void Object::AddComponent(unsigned int _id)
{
	auto addCp = ObjectManager::Instance().ToComponent(_id);

	addCp->SetTag(ObjectManager::Instance().ToTag(_id));
	addCp->SetOwner(weak_from_this());

	m_cpList.push_back(std::shared_ptr<Component>(addCp));

	addCp->SetOwner(weak_from_this());
	addCp->InitJson();
	addCp->Start();
}

void Object::AddComponent(Component* _addCp)
{
	unsigned int id = ObjectManager::Instance().ToID(typeid(*_addCp).name());

	m_cpList.push_back(std::shared_ptr<Component>(_addCp));
	_addCp->SetOwner(weak_from_this());
	_addCp->InitJson();
	_addCp->SetTag(ObjectManager::Instance().ToTag(id));
	_addCp->Start();
}

void Object::AddComponents(unsigned int _id)
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

bool Object::CheckID(unsigned int _id)const
{
	return m_compoID | _id;
}

bool Object::CheckIDs(unsigned int _id)const
{
	unsigned int search = m_compoID | _id;
	return m_compoID == search;
}

std::shared_ptr<Component> Object::SearchTag(std::string _tag)
{
	auto&& it = m_cpList.begin();
	while (it != m_cpList.end() && !(*it)->CheckTag(PickName(_tag,'_'))) { it++; }

	return  *it;
}

std::list<std::shared_ptr<Component>> Object::SearchTags(std::string _tag)
{
	auto&& it = m_cpList.begin();
	std::list<std::shared_ptr<Component>> list;
	while (it != m_cpList.end())
	{
		if((*it)->CheckTag(PickName(_tag, '_')))list.push_back(*it);
		it++;
	}

	return list;
}

void Object::Release()
{
	if (!m_bSave)return;
	nlohmann::json component;
	for (auto&& it : m_cpList)component[it->GetTag()] = it->GetJson();
	nlohmann::json finalJson;
	finalJson["Component"] = component;
	std::ofstream file(m_filePath);

	finalJson["ID"] = m_compoID;
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

