#include "GameScreen.h"
#include "../../../ImGuiHelper/ImGuiEditor.h"
#include "../../../RenderManger/RenderManger.h"

void GameScreen::Update()
{
	ImVec2 size(1280.0f, 720.0f);
	static std::shared_ptr<KdTexture> tex;
	tex = RenderManager::Instance().CreateBackBuffer();
	ImGui::Image(tex->WorkSRView(), size * (ImGui::GetContentRegionAvail() / size));
}
