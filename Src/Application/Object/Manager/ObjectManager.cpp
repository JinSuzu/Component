#include "ObjectManager.h"
#include "../../Component/AllComponentIncluder.h"
#include "../Object.h"

void ObjectManager::Draw()
{
	for (auto&& it : m_obList) { it->Draw(); }
}

void ObjectManager::PreUpdate()
{
	auto it = m_obList.begin();
	for (; it != m_obList.end();)
	{
		if (it->get() == nullptr || !(*it)->GetActive())
		{
			it = m_obList.erase(it);
			continue;
		}
		(*it)->PreUpdate();
		it++;
	}
}

void ObjectManager::Update() {

	for (auto& obj : m_obList) {
		obj->Update();
	}

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


void ObjectManager::PostUpdate()
{
	auto it = m_obList.begin();
	for (; it != m_obList.end();)
	{
		if (it->get() == nullptr || !(*it)->GetActive())
		{
			it = m_obList.erase(it);
			continue;
		}
		(*it)->PostUpdate();
		it++;
	}
}

void ObjectManager::ImGuiUpdate()
{
	static int num = 0;
	ImGui::DragInt("Shift", &num, 1, 0, 1);
	if (num == 0)
	{
		static char path[50] = "";
		ImGui::InputText("CreateObject", path, sizeof(path));

		static unsigned int state;
		int Max = ComponentMap::Instance().GetCompoNum();
		for (int i = 0; i < Max; i++)
		{
			bool flg = state & (1 << i);
			if (i % 2)ImGui::SameLine();
			ImGui::Checkbox(ComponentMap::Instance().GetTag(1 << i).c_str(), &flg);

			if (flg)state |= (1 << i);
			if (!flg)state &= ~(1 << i);
		}

		if (ImGui::Button("Add"))CreateObject(path)->AddComponents(state);
	}
	if (num == 1) 
	{
		ImGui::Text("ObjNum : %d", m_obList.size());
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(500, 300), ImGuiWindowFlags_NoTitleBar);
		for (auto&& it : m_obList)
		{
			it->ImGuiUpdate();
		}
		ImGui::EndChild();
	}

	
}

void ObjectManager::Init()
{
}

void ObjectManager::Relese()
{
	auto it = m_obList.begin();
	while (it != m_obList.end()) it = m_obList.erase(it);
}

std::shared_ptr<Component> ObjectManager::ToComponent(unsigned int _id)
{
	return ComponentMap::Instance().createFind(_id);
}

std::string ObjectManager::ToTag(unsigned int _id)
{
	return ComponentMap::Instance().GetTag(_id);
}

unsigned int ObjectManager::ToID(std::string _tag)
{
	auto ID = ComponentMap::Instance().bitFind(_tag);
	if (ID == ComponentMap::Instance().bitEnd())assert(false && "コンポIDlistにないよ！！");
	return ID->second;
}


std::shared_ptr<Object> ObjectManager::CreateObject(std::string _tag,bool flg)
{
	auto object = std::make_shared<Object>();
	object->Init(_tag);
	if(flg)m_obList.push_back(object);
	return object;
}

void ObjectManager::UpdateRange(std::list<std::shared_ptr<Object>>::iterator start, std::list<std::shared_ptr<Object>>::iterator end)
{
	for (auto&& it = start; it != end; ++it) {
		if ((*it) && (*it)->GetActive()) {
			(*it)->Update();
		}
	}

}

