#pragma once
#include "../Component.h"

enum Shaft
{
	X = 1 << 0,
	Y = 1 << 1,
	Z = 1 << 2
};

class Cp_Transform
	:public Component
{
public:
	Cp_Transform() {}
	~Cp_Transform()override {}

	void Start()override;

	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson();

	Math::Vector3 GetPosition(bool _PushFollow = false) const;
	Math::Vector3& RefPosition() { return m_position; }
	Math::Vector3 GetRotation(bool _PushFollow = false) const;
	Math::Vector3 GetScale(bool _PushFollow = false)	const;

	void SetPosition(Math::Vector3 _pos) { m_position = _pos; }
	void SetRotation(Math::Vector3 _rota) { m_rotation = _rota; }
	void SetScale(Math::Vector3 _scal) { m_scale = _scal; }

	//_matTag : "S""T""R"で行列計算を入れ替えられる
	//_PushFollow : followフラグを無視する
	Math::Matrix GetMatrix(std::string _matTag = std::string(),bool _PushFollow = false);
	Math::Matrix GetMatrix(bool _PushFollow);

	Math::Matrix GetTMat() const;
	Math::Matrix GetSMat() const;
	Math::Matrix GetRMat(UINT _shafts = 0);

	std::weak_ptr<Cp_Transform> GetParent() { return m_parent; }
	void SetParent(std::weak_ptr<Cp_Transform> _parent) { m_parent = _parent; }
	void UnFollow() { m_bFollow = false; };
private:
	Math::Matrix	m_mWorld = Math::Matrix::Identity;

	Math::Vector3	m_position = { 0,0,0 };
	Math::Vector3	m_rotation = { 0,0,0 };
	Math::Vector3	m_scale = { 1,1,1 };

	std::string		m_myMatTag = "SRT";
	std::string		m_parentMatTag = "SRT";
	std::weak_ptr<Cp_Transform>	m_parent;

	bool m_bFollow = true;

};
