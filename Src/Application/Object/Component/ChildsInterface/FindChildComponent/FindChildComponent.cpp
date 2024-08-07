#include "FindChildComponent.h"
#include "../../../Game/GameObject.h"

void FindChildComponent::PreUpdateContents()
{
	if (m_holdChild.expired())return;
	std::list<std::weak_ptr<GameObject>>::iterator it = m_owner.lock()->WorkChilds().begin();
	while (it != m_owner.lock()->WorkChilds().end())
	{
		if (it->expired())
		{
			it = m_owner.lock()->WorkChilds().erase(it);
			continue;
		}

		if (it->lock()->GetName() == m_searchName) 
		{
			m_holdChild = *it;
		}
		it++;
	}
}

void FindChildComponent::ImGuiUpdate()
{

	if (ImGui::BeginCombo("ChildNames",m_searchName.c_str()))
	{
		std::list<std::weak_ptr<GameObject>>::iterator it = m_owner.lock()->WorkChilds().begin();
		while (it != m_owner.lock()->WorkChilds().end())
		{
			if (it->expired()) 
			{
				it = m_owner.lock()->WorkChilds().erase(it);
				continue;
			}

			std::string_view name = it->lock()->GetName();
			if (ImGui::Selectable(name.data())) 
			{
				m_searchName = name.data();
				break;
			}
			it++;
		}
		ImGui::EndCombo();
	}
}

nlohmann::json FindChildComponent::Serialize()
{
	return nlohmann::json();
}

void FindChildComponent::LoadJson(nlohmann::json _json)
{
}
