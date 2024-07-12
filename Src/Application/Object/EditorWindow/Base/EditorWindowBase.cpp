#include "EditorWindowBase.h"

void EditorWindowBase::Update()
{
	ImGui::Begin(m_name.c_str()); 
	{
		UpdateContents();
	}
	ImGui::End();
	PostUpdate();
}
