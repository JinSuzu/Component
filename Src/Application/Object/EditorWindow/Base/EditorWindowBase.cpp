#include "EditorWindowBase.h"

void EditorWindowBase::Draw()
{
	ImGui::Begin(m_name.c_str()); 
	{
		Update();
	}
	ImGui::End();
}
