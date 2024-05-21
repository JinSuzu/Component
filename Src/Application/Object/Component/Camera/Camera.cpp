#include "Camera.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"

void Cp_Camera::Start()
{
	m_camera = std::make_unique<KdCamera>();
}

void Cp_Camera::PreDraw()
{
	m_camera->SetCameraMatrix(m_owner.lock()->GetTransform().lock()->GetMatrix());
	m_camera->SetToShader();
}
