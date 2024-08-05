#pragma once

class TransformLimitComponent
	:public Component 
{
public:
	void PostUpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;
private:
	enum Shaft
	{
		Null = 0,
		X = 1 << 0,
		Y = 1 << 1,
		Z = 1 << 2,
		Max = 1 << 3
	};

	struct LimitSet
	{
		bool			bActive = false;

		unsigned int	shaft	= Shaft::Null;
		bool			bMax	= false;
		Math::Vector3	Max		= {100,100,100};
		bool			bMin	= false;
		Math::Vector3	Min		= { -100,-100,-100 };
		bool			bOffset = false;
		Math::Vector3	Offset	= Math::Vector3::Zero;

		void ImGuiUpdate(std::string _name);
		void InitJson(nlohmann::json _json);
		nlohmann::json GetJson();
	};

	LimitSet m_posLimit;
	LimitSet m_rotaLimit;
	LimitSet m_scaleLimit;

	void Clamp(Math::Vector3& _vec3, LimitSet _limitSet);
};