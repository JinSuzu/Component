#pragma once
#include "../Component.h"

class Cp_Transform
	:public Component
{
public:
	Cp_Transform() {}
	~Cp_Transform() {}

	void InitJson()override;

	void ImGuiUpdate()override;
	nlohmann::json GetJson();

	Math::Vector3 GetPosition() const	{ return m_position; }
	Math::Vector3 GetRotation() const	{ return m_rotation; }
	Math::Vector3 GetScale()	const	{ return m_scale; }

	Math::Matrix GetMatrix();
	Math::Matrix GetTMat();
	Math::Matrix GetSMat();
	Math::Matrix GetRMat();

	void SetPosition(Math::Vector3 _pos)	{ m_position = _pos; }
	void SetRotation(Math::Vector3 _rota)	{ m_rotation = _rota; }
	void SetScale(Math::Vector3 _scal)		{ m_scale = _scal; }

	void SetParent(std::weak_ptr<Cp_Transform> _parent) { m_parent = _parent; }
private:
	Math::Matrix	m_mWorld		= Math::Matrix::Identity;

	Math::Vector3	m_position		= { 0,0,0 };
	Math::Vector3	m_rotation		= { 0,0,0 };
	Math::Vector3	m_scale			= { 1,1,1 };


	std::string		m_matTag		= "SRT";
	std::weak_ptr<Cp_Transform>	m_parent;
};
