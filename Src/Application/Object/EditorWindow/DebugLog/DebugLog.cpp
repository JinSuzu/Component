#include "DebugLog.h"
#include "../../../ImGuiHelper/ImGuiEditor.h"
#include "../../../main.h"

void DebugLog::UpdateContents()
{
	Application::Instance().m_log.Draw();
}
