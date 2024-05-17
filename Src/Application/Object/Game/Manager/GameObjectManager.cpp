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


	ImGui::SeparatorText(("ObjectList :" + std::to_string(m_obList.size())).c_str());
	ImGui::BeginChild("##ObjectChild", ImVec2(350, 100), ImGuiChildFlags_None);

	int obNum = 0; static std::weak_ptr<GameObject> pickObject = std::weak_ptr<GameObject>(); static int pickNum = 0;
	if (ImGui::BeginTable("##ObjectTable", 3)) {
		for (auto&& it : m_obList) {
			if (obNum % 3 == 0) ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(obNum % 3);
			ImGui::PushID(obNum);
			if (ImGui::Button((std::to_string(obNum) + " : " + it->GetName()).c_str())) {
				pickNum = obNum;
				pickObject = it;
			}
			ImGui::PopID();
			obNum++;
		}
		ImGui::EndTable();
	}

	ImGui::EndChild();


	ImGui::SeparatorText("EditObject");
	if (pickObject.expired())return;
	auto TreeNode = [&]()
		{
			bool flg = ImGui::TreeNode(std::to_string(pickNum).c_str());
			ImGui::SameLine(); ImGui::Text((" : " + pickObject.lock()->GetName()).c_str());
			return flg;
		};

	ImGui::SetNextItemOpen(true);
	if (TreeNode())
	{
		pickObject.lock()->ImGuiUpdate();
		ImGui::TreePop();
	}
}

void GameObjectManager::ImGuiCreateObject(std::weak_ptr<GameObject> _parent)
{
	if (!ImGui::TreeNode("CreateObject"))return;
	ImGui::SeparatorText("ComponentSet");

	static char path[50] = ""; ImGui::InputText("JsonPath", path, sizeof(path));
	static unsigned int state; int Max = ComponentMap::Instance().GetCompoNum();

	ImGui::BeginChild("##ComponentSet", ImVec2(350, 100),  ImGuiChildFlags_Border);
	for (int i = 0; i < Max; i++)
	{
		bool flg = state & (1 << i);
		if (i % 3)ImGui::SameLine();
		ImGui::Checkbox(ComponentMap::Instance().GetTag(1 << i).c_str(), &flg);

		if (flg)state |= (1 << i);
		if (!flg)state &= ~(1 << i);
	}
	ImGui::EndChild();

	ImGui::SameLine();
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
	if (ImGuiTreeCenterButton("AddComponent"))ImGui::OpenPopup("Components");
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
