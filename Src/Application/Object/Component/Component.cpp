#include "Component.h"
#include "../Game/GameObject.h"

nlohmann::json Component::GetJson()
{
	m_jsonData["ID"] = m_id;
	m_jsonData["Name"] = m_name;
	return m_jsonData;
}