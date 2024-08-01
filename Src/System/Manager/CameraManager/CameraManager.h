#pragma once

class CameraManager
{
public:
	class CameraPack
	{
	public:

		int priority = 0;
		Math::Matrix matrix;

	private:
		friend CameraManager;
		CameraPack() {}
	};

	//カメラをへ入れる情報を優先度を付けて登録する
	void RegisterCamera(std::weak_ptr<CameraPack> _cameraPack);
	//カメラ情報の生成
	std::shared_ptr<CameraPack> CreateCameraPack();

	//優先度の最も高い情報をカメラに送り、登録情報をリセットする
	void CameraToShader();

	const std::weak_ptr<KdCamera> GetCamera() { return m_camera; }

	void StopUpdate();

	void Init();
private:
	std::map<int, std::weak_ptr<CameraPack>>m_cameraPacks;
	std::shared_ptr<KdCamera> m_camera;

	std::list<std::weak_ptr<CameraPack>> m_debugLists;

	CameraManager() {}
	~CameraManager() {}
public:
	static CameraManager& Instance()
	{
		static CameraManager inst;
		return inst;
	}
};