#include "DebugLog.h"
#include "../../../ImGuiHelper/ImGuiEditor.h"
#include "../../../main.h"

void DebugLog::Update()
{
	Application::Instance().m_log.Draw();
}
