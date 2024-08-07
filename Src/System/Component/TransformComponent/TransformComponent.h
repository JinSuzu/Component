#pragma once

enum class Shaft
{
	X = 1 << 0,
	Y = 1 << 1,
	Z = 1 << 2
};

class TransformComponent
	:public Component
{
public:
	TransformComponent() {}
	~TransformComponent()override {}

	void Start()override;
	void MatrixUpdata();

	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize();

	//座標
	const Math::Vector3 GetWorldPosition()	const	{ return m_mWorld.Translation(); };
	const Math::Vector3& GetLocalPosition()	const	{ return m_position; };
	Math::Vector3& WorkLocalPosition()				{ return m_position; }
	void SetLoaclPosition(Math::Vector3 _pos)		{ m_position = _pos; }

	//回転
	const Math::Vector3 GetWorldRotation()	const	{ return m_rotation + (m_bFollow ? m_parent.lock()->GetWorldPosition() : Math::Vector3::Zero); }
	const Math::Vector3 GetLocalRotation()	const	{ return m_rotation; };
	Math::Vector3& WorkLocalRotation()				{ return m_rotation; };
	void SetLocalRotation(Math::Vector3 _rota)
	{
		if (abs(_rota.x) > 360)_rota.x = _rota.x + (_rota.x < 0 ? 360 : -360);
		if (abs(_rota.y) > 360)_rota.y = _rota.y + (_rota.y < 0 ? 360 : -360);
		if (abs(_rota.z) > 360)_rota.z = _rota.z + (_rota.z < 0 ? 360 : -360);
		m_rotation = _rota;
	}

	//拡縮
	const Math::Vector3 GetWorldScale()		const	{ return m_scale * (m_bFollow ? m_parent.lock()->GetWorldScale() : Math::Vector3::One); };
	const Math::Vector3& GetLocalScale()	const	{ return m_scale; }
	Math::Vector3& WorkLocalScale()					{ return m_scale; }
	void SetLocalScale(Math::Vector3 _scale) { m_scale = _scale; }

	//座標行列
	const Math::Matrix& GetMatrix() { return m_mWorld; };
	Math::Matrix& WorkMatrix()		{ return m_mWorld; };


	const Math::Matrix GetTMat() const { return Math::Matrix::CreateTranslation(m_position); };
	const Math::Matrix GetSMat() const { return Math::Matrix::CreateScale(m_scale); }
	const Math::Matrix GetRMat() const
	{
		Math::Matrix rMat;
		rMat *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_rotation.x));
		rMat *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotation.y));
		rMat *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_rotation.z));
		return rMat;
	}
	Math::Matrix GetRMat(UINT _shafts)
	{
		Math::Matrix rMat;
		if (_shafts & (UINT)Shaft::X)rMat *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_rotation.x));
		if (_shafts & (UINT)Shaft::Y)rMat *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotation.y));
		if (_shafts & (UINT)Shaft::Z)rMat *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_rotation.z));
		return rMat;
	}

	//_matTag : "S""T""R"で行列計算を入れ替えられる

	std::weak_ptr<TransformComponent> GetParent()				{ return m_parent; }
	void SetParent(std::weak_ptr<TransformComponent> _parent)	{ m_parent = _parent; }

	const std::string& GetMatTag() { return m_myMatTag; }

	void UnFollow() { m_bFollow = false; };
private:
	Math::Matrix	m_mWorld = Math::Matrix::Identity;

	Math::Vector3	m_position = Math::Vector3::Zero;
	Math::Vector3	m_rotation = Math::Vector3::Zero;
	Math::Vector3	m_scale    = Math::Vector3::One;

	std::string		m_myMatTag = "SRT";
	std::weak_ptr<TransformComponent>	m_parent;

	std::unordered_map<char, std::function<Math::Matrix()>> m_getMatrix;

	bool m_bFollow = true;

};
