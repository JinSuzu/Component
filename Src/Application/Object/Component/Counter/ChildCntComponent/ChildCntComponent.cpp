#include "ChildCntComponent.h"
#include "../../../Game/GameObject.h"

void ChildCntComponent::PreUpdateContents()
{
	if (m_postNum == 0) 
	{
		m_postNum = m_owner.lock()->GetChilds().size();
	}

	std::list<std::weak_ptr<GameObject>>* list = m_owner.lock()->WorkChilds();
	std::list<std::weak_ptr<GameObject>>::iterator it = m_owner.lock()->GetChilds().begin();
	while (it != m_owner.lock()->GetChilds().end())
	{
		if (it->expired())
		{
			it = m_owner.lock()->WorkChilds()->erase(it);
			continue;
		}
		it++;
	}

	if (m_postNum != m_owner.lock()->GetChilds().size())
	{
		m_postNum = m_owner.lock()->GetChilds().size();
		Score::Instance().Add();
	}
}
