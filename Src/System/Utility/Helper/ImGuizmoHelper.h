#pragma once
//ImGuiZmoが存在しているか
#ifdef IMGUIZMO_NAMESPACE

struct TransformPack
{
	TransformPack() {}
	TransformPack(float* _position, float* _rotation, float* _scale)
		:position(_position)
		, rotation(_rotation)
		, scale(_scale)
	{}
	Math::Vector3 position;
	Math::Vector3 rotation;
	Math::Vector3 scale;
};

inline bool Update(Math::Matrix _cameraMat, Math::Matrix _projectMat, Math::Matrix _targetMat, Math::Matrix _parentMat, ImGuizmo::OPERATION& _opreration, ImGuizmo::MODE& _mode, TransformPack& _resultPack, ImRect _windowRect = { ImGui::GetWindowPos(),ImGui::GetWindowSize() })
{
	// Manipulate関数に渡す行列データのポインタ
	//float* matrixPtr = reinterpret_cast<float*>(&_cameraMat);
	//float* proMatrixPtr = reinterpret_cast<float*>(&_projectMat);
	//float* TargetMatrixPtr = reinterpret_cast<float*>(&_targetMat);
	//
	float* matrixPtr		=  _cameraMat.m[0];
	float* proMatrixPtr		=  _projectMat.m[0];
	float* TargetMatrixPtr  =  _targetMat.m[0];

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(_windowRect.Min.x, _windowRect.Min.y, _windowRect.Max.x, _windowRect.Max.y);
	ImGuizmo::Manipulate(
		matrixPtr,					// カメラのビュー行列
		proMatrixPtr,				// カメ=ラのプロジェクション行列
		_opreration,				// 操作モード
		_mode,						// 変換モード
		TargetMatrixPtr				// オブジェクトの変換行列
	);

	// 操作してる時のみ反映
	if (ImGuizmo::IsUsing())
	{
		_targetMat *= _parentMat.Invert();

		// 結果をMatrixに変換
		ImGuizmo::DecomposeMatrixToComponents(TargetMatrixPtr, &_resultPack.position.x, &_resultPack.rotation.x, &_resultPack.scale.x);
		return true;
	}
	return false;
}

inline void OprerationChanger(ImGuizmo::OPERATION& _opreration)
{
	// ギズモの操作切り替えボタン
	std::string str = magic_enum::enum_name(_opreration).data();
	if (ImGui::BeginCombo("OPERATION",str.c_str()))
	{
		auto list = magic_enum::enum_names<ImGuizmo::OPERATION>();
		for (auto& it : list) 
		{
			if (ImGui::Selectable(it.data())) 
			{
				_opreration = magic_enum::enum_cast<ImGuizmo::OPERATION>(it).value();
				break;
			}
		}
		ImGui::EndCombo();
	}
}

inline void ModeChanger(ImGuizmo::MODE& _mode)
{
	// ギズモの操作切り替えボタン
	std::string str = magic_enum::enum_name(_mode).data();
	if (ImGui::BeginCombo("MODE",str.c_str()))
	{
		auto list = magic_enum::enum_names<ImGuizmo::MODE>();
		for (auto& it : list) 
		{
			if (ImGui::Selectable(it.data())) 
			{
				_mode = magic_enum::enum_cast<ImGuizmo::MODE>(it).value();
				break;
			}
		}
		ImGui::EndCombo();
	}
}
#endif