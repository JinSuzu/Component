#include "EditorWindowBase.h"

void EditorWindowBase::Update()
{
	ImGui::Begin(m_name.c_str());
	{
		BeginChildOption();
		{
			UpdateContents();
		}
		EndChildOption();
	}
	ImGui::End();
}

void EditorWindowBase::ConfigLoad()
{
	ConfigManger config;
	if (config.Load(m_name))
	{
		ConfigLoadContents(config);
	}
}

void EditorWindowBase::ConfigSave()
{
	ConfigManger config;
	ConfigSaveContents(config);

	config.Save(m_name);
}

void EditorWindowBase::Init()
{

}
void EditorWindowBase::Release()
{

}

void EditorWindowBase::BeginChildOption()
{
	ImGui::BeginChild(std::to_string((int)this).c_str(),ImVec2());
}

void EditorWindowBase::EndChildOption()
{
	ImGui::EndChild();
}
