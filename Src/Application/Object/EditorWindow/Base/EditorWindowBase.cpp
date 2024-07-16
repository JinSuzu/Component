#include "EditorWindowBase.h"

void EditorWindowBase::Update()
{
	ImGui::Begin(m_name.c_str());
	{
		ImGui::BeginChild(std::to_string((int)this).c_str());
		{
			UpdateContents();
		}
		ImGui::EndChild();
		PostUpdate();
	}
	ImGui::End();
}

void EditorWindowBase::ConfigLoad()
{
	ConfigManger config;
	nlohmann::json data;
	if (config.Load(m_name,data))
	{
		ConfigLoadContents(data);
	}
}

void EditorWindowBase::ConfigSave()
{
	nlohmann::json data;
	ConfigLoadContents(data);

	ConfigManger config;
	config.Save(m_name, data);
}
