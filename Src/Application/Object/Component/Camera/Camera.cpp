#include "Camera.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"
#include "../../../RenderManger/RenderManger.h"

void Cp_Camera::Start()
{
	m_camera = std::make_unique<KdCamera>();
	m_preDraw = std::make_shared<std::function<void()>>([&]() {PreDraw(); });
	RenderManager::Instance().AddPreDraw(m_preDraw);
}

void Cp_Camera::PreDraw()
{
	m_camera->SetCameraMatrix(m_owner.lock()->GetTransform().lock()->GetMatrix());
	m_camera->SetToShader();
}
