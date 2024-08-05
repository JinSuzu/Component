#include "CameraManager.h"

#include "../../../Application/main.h"
#include "../../EditorWindow/DebugLog/DebugLog.h"

void CameraManager::RegisterCamera(std::weak_ptr<CameraPack> _cameraPack)
{
	int size = m_cameraPacks.size();
	m_cameraPacks.emplace(_cameraPack.lock()->priority, _cameraPack);
	if(size == m_cameraPacks.size())Editor::Instance().GetDebugLog().lock()->AddLog("Overlapping CameraPriority!!");
}

void CameraManager::CameraToShader()
{
	std::map<int, std::weak_ptr<CameraPack>>::reverse_iterator it = m_cameraPacks.rbegin();
	if(it != m_cameraPacks.rend() && it->second.lock())
	{
		Math::Matrix& mat = it->second.lock()->matrix;
		m_camera->SetCameraMatrix(mat);
	}
	m_camera->SetToShader();

	m_cameraPacks.clear();
}

void CameraManager::Init()
{
	m_camera = std::make_shared<KdCamera>();
	m_camera->SetCameraMatrix(Math::Matrix::Identity);
}
