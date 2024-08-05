#include "BuildCameraComponent.h"
#include "../../../Game/GameObject.h"
#include "../../../../main.h"

void BuildCameraComponent::Start()
{
	m_trans = m_owner.lock()->GetTransform();
}


void BuildCameraComponent::ImGuiUpdate()
{
}

void BuildCameraComponent::HoverMove()
{
	Math::Vector3 vec;
	int wheelValue = Application::Instance().GetMouseWheelValue();
	if (wheelValue < 0)vec = m_trans.lock()->GetRMat().Forward();
	else if (wheelValue > 0)vec = m_trans.lock()->GetRMat().Backward();
	
	m_trans.lock()->SetPosition(m_trans.lock()->GetPosition() + (vec * 0.5));
}

void BuildCameraComponent::AngleShift()
{
	Math::Vector2 delta = Application::Instance().GetMouseDelta();
	Math::Vector3 angle = m_trans.lock()->GetRotation();
	angle.y += delta.x;
	angle.x -= delta.y;
	m_trans.lock()->SetRotation(angle);
}
