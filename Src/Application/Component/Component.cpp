#include "Component.h"
#include "../Object/Object.h"

void Component::InitJson()
{
	nlohmann::json json = m_owner.lock()->GetJson();
	if (json.is_null())return;
	m_jsonData = m_owner.lock()->GetJson()["Component"][m_tag];
}
