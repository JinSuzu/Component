#include "CameraManager.h"

#include "../../../Application/main.h"
#include "../../EditorWindow/DebugLog/DebugLog.h"

void CameraManager::RegisterCamera(std::weak_ptr<CameraPack> _cameraPack)
{
	int size = m_cameraPacks.size();
	m_cameraPacks.emplace(_cameraPack.lock()->priority, _cameraPack);
	if(size == m_cameraPacks.size())Editor::Instance().GetDebugLog().lock()->AddLog("Overlapping CameraPriority!!");
}

std::shared_ptr<CameraManager::CameraPack> CameraManager::CreateCameraPack()
{
	std::shared_ptr<CameraPack> temp = std::shared_ptr<CameraPack>(new CameraPack);
	m_debugLists.push_back(temp);
	return temp;
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

void CameraManager::StopUpdate()
{
	std::list<std::weak_ptr<CameraPack>>::iterator it = m_debugLists.begin();
	std::list<std::weak_ptr<CameraPack>>::iterator end = m_debugLists.end();
	while (it != end)
	{ 
		if (it->expired()) 
		{
			it = m_debugLists.erase(it);
			continue;
		}
		RegisterCamera(*it);
		it++;
	}
}

void CameraManager::Init()
{
	m_camera = std::make_shared<KdCamera>();
	m_camera->SetCameraMatrix(Math::Matrix::Identity);
}
