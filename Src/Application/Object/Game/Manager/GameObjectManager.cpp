#include "GameObjectManager.h"
#include "../../Component/AllComponentIncluder.h"
#include "../GameObject.h"

#define ITERATOR(x)				\
for (auto&& it : m_obList)		\
{								\
	if(it.get() != nullptr)		\
	{							\
		if (it->GetActive())	\
		{						\
			it->x();			\
		}						\
	}							\
}

void GameObjectManager::Draw()
{
	ITERATOR(Draw)
}

void GameObjectManager::PreUpdate()
{
	auto it = m_obList.begin();
	for (; it != m_obList.end();)
	{
		if (it->get() == nullptr || (*it)->GetDestroy())
		{
			it = m_obList.erase(it);
			continue;
		}
		if ((*it)->GetActive())(*it)->PreUpdate();
		it++;
	}
}
void GameObjectManager::Update() {

	ITERATOR(Update);

	auto it = COMPONENTLISTINSTANCE(BoxCollision).Get().begin();
	while (it != COMPONENTLISTINSTANCE(BoxCollision).Get().end())
	{
		if (!it->use_count())
		{
			it = COMPONENTLISTINSTANCE(BoxCollision).Get().erase(it);
			continue;
		}

		auto next = std::next(it, 1);
		while (next != COMPONENTLISTINSTANCE(BoxCollision).Get().end())
		{
			if (!next->use_count() || it == next)
			{
				next++;
				continue;
			}

			if (it->lock()->WithPoint(*next))
			{
				it->lock()->PushColliList(next->lock()->GetOwner().lock()->GetTag());
				(*next).lock()->OnHit();
				(*next).lock()->PushColliList(it->lock()->GetOwner().lock()->GetTag());
			}
			it->lock()->TurnPostPos(*next);
			next++;
		}

		it++;
	}
}
void GameObjectManager::PostUpdate()
{
	ITERATOR(PostUpdate)
}

void GameObjectManager::ImGuiUpdate()
{
	ImGuiCreateObject();

	ImGui::Text("ObjNum : %d", m_obList.size());
	ImGui::BeginChild(ImGuiCond_Always, ImVec2(500, 300), ImGuiWindowFlags_NoTitleBar);
	int obNum = 0;
	for (auto&& it : m_obList)
	{
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);

		auto TreeNode = [&]()
			{
				bool flg = ImGui::TreeNode(std::to_string(obNum++).c_str());
				ImGui::SameLine(); ImGui::Text((" : " + it->GetName()).c_str());
				return flg;
			};

		if (TreeNode())
		{
			it->ImGuiUpdate();
			ImGui::TreePop();
		}

	}
	
	ImGui::EndChild();
}

void GameObjectManager::ImGuiCreateObject(std::weak_ptr<GameObject> _parent)
{
	if (!ImGui::TreeNode("CreateObject"))return;

	static char path[50] = "";
	ImGui::InputText("JsonPath", path, sizeof(path));


	ImGui::BeginChild("ComponentSet", ImVec2(350, 100), ImGuiWindowFlags_NoTitleBar);
	static unsigned int state;
	int Max = ComponentMap::Instance().GetCompoNum();
	for (int i = 0; i < Max; i++)
	{
		bool flg = state & (1 << i);
		if (i % 3)ImGui::SameLine();
		ImGui::Checkbox(ComponentMap::Instance().GetTag(1 << i).c_str(), &flg);

		if (flg)state |= (1 << i);
		if (!flg)state &= ~(1 << i);
	}
	ImGui::EndChild();

	if (ImGui::Button("Add"))
	{
		auto obj = CreateObject(path);
		if(!_parent.expired())obj->SetParent(_parent);
		obj->AddComponents(state);

	}

	ImGui::TreePop();
}

void GameObjectManager::ImGuiAddComponent(std::weak_ptr<GameObject> _object)
{
	if (ImGui::Button("AddComponent"))ImGui::OpenPopup("Components");
	if (!ImGui::BeginPopup("Components"))return;

	auto it = ComponentMap::Instance().bitBegin();
	while (it != ComponentMap::Instance().bitEnd())
	{
		if (ImGui::MenuItem(it->first.c_str()))
		{
			if (auto obj = _object.lock()) {
				obj->AddComponent(it->second);
			}
			break;
		}
		++it;
	}
	ImGui::EndPopup();

}

void GameObjectManager::Init()
{
}
void GameObjectManager::Release()
{
	auto it = m_obList.begin();
	while (it != m_obList.end()) it = m_obList.erase(it);
}

std::shared_ptr<Component> GameObjectManager::ToComponent(unsigned int _id)
{
	return ComponentMap::Instance().createFind(_id);
}
std::string GameObjectManager::ToTag(unsigned int _id)
{
	return ComponentMap::Instance().GetTag(_id);
}
unsigned int GameObjectManager::ToID(std::string _tag)
{
	auto ID = ComponentMap::Instance().bitFind(_tag);
	if (ID == ComponentMap::Instance().bitEnd())assert(false && "コンポIDListにないよ！！");
	return ID->second;
}


std::shared_ptr<GameObject> GameObjectManager::CreateObject(std::string _tag,bool flg)
{
	auto object = std::make_shared<GameObject>();
	object->Init(_tag);
	if(flg)m_obList.push_back(object);
	return object;
}
