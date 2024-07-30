#include "EditorWindowBase.h"

void EditorWindowBase::Update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
	ImGui::Begin(m_name.c_str());
	ImGui::PopStyleVar(1);
	{
		if(m_beginChildOption.before)m_beginChildOption.before();	//子ウィンドウに対しての生成前処理
		ImGui::BeginChild
		(
			std::to_string((int)this).c_str(),
			ImGui::GetContentRegionAvail(),						//表示上限サイズで子ウィンドウを生成
			ImGuiChildFlags_Border
		);	
		if(m_beginChildOption.after)m_beginChildOption.after();	//子ウィンドウに対しての生成後処理
		{
			UpdateContents();
		}
		if(m_endChildOption.before)m_endChildOption.before();	//子ウィンドウに対しての終了前処理
		ImGui::EndChild();
		if(m_endChildOption.after)m_endChildOption.after();	//子ウィンドウに対しての終了後処理
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