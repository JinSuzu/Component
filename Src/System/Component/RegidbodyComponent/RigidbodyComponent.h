﻿#pragma once

//もどき : 移動量管理
//		 : 当たる側の当たり判定設定
class RigidbodyComponent
	:public Component
{
	enum Shape
	{
		Sphere,
		Box,
		Ray,
		Max
	};

	struct ShapeDate
	{
		UINT tag = (UINT)KdCollider::Type::TypeGround;
		Math::Vector3 offsetPos;
		Math::Vector3 radius;

		std::list<KdCollider::CollisionResult> pResults;
	};
public:
	void Start()override;

	void PreUpdateContents()override;
	void UpdateContents()override;
	void PostUpdateContents()override;

	void UpdateRenderContents()override;

	void ImGuiUpdate()override;

	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;

	void AddMove(Math::Vector3 _move) { m_move += _move; }
	Math::Vector3 GetMove()	const { return m_move; }
	void SetMove(Math::Vector3 _move) { m_move = _move; }
	void ResetMove() { m_move = Math::Vector3::Zero; }

	float GetGravity() const { return m_gravityPow; }
	void  SetGravity(float _gravity) { m_gravity = _gravity; }
	bool GetLanding() const { return m_bLanding; }

	float GetRadius() const
	{
		if (m_shape == Shape::Sphere)return m_shapeDate.radius.y;
		return m_shapeDate.radius.Length();
	}

	std::list<KdCollider::CollisionResult>& GetHitResult()
	{
		return m_shapeDate.pResults;
	}
	Math::Vector3 GetOffsetPos() const { return m_shapeDate.offsetPos; }
private:
	float Gravity();
	void MakeResults();

	Math::Vector3 m_move = Math::Vector3::Zero;

	bool  m_bActiveGravity = true;
	float m_gravity = 0.0f;
	float m_gravityPow = 1.0f;
	float m_height = 0.0f;
	float m_offsetHeight = 0.0f;
	bool  m_bLanding = false;


	float m_deceleration = 0.98f;

	bool m_collisionBody = false;
	Shape m_shape = Shape::Sphere;

	ShapeDate m_shapeDate;
};