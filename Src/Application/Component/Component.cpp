#include "Component.h"
#include "../Object/Object.h"

void Component::Start(std::weak_ptr<Object> _owner)
{
	m_owner = _owner;

	nlohmann::json json = _owner.lock()->GetJson();
	if (json.is_null())return;
	Json = _owner.lock()->GetJson()["Component"][m_tag];
}