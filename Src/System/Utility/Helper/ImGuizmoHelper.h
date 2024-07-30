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

inline bool Update(Math::Matrix _cameraMat, Math::Matrix _projectMat, Math::Matrix _targetMat, Math::Matrix _parentMat, ImGuizmo::OPERATION& _opreration, TransformPack& _resultPack, ImRect _windowRect = { ImGui::GetWindowPos(),ImGui::GetWindowSize() })
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
		ImGuizmo::MODE::LOCAL,      // 変換モード
		TargetMatrixPtr				// オブジェクトの変換行列
	);

	// 操作してる時のみ反映
	if (ImGuizmo::IsUsing())
	{
		_targetMat *= _parentMat.Invert();

		// 結果をMatrixに変換
		float position[3] = { 0.0f,0.0f,0.0f }, rotation[3] = { 0.0f,0.0f,0.0f }, scale[3] = { 0.0f,0.0f,0.0f };
		ImGuizmo::DecomposeMatrixToComponents(TargetMatrixPtr, position, rotation, scale);
		TransformPack result(position, rotation, scale);
		_resultPack = result;
		return true;
	}
	return false;
}

inline void OprerationChanger(ImGuizmo::OPERATION& _opreration)
{
	// ギズモの操作切り替えボタン
	if (ImGui::RadioButton("Transform", _opreration == ImGuizmo::TRANSLATE))
	{
		_opreration = ImGuizmo::TRANSLATE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotation", _opreration == ImGuizmo::ROTATE))
	{
		_opreration = ImGuizmo::ROTATE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", _opreration == ImGuizmo::SCALE))
	{
		_opreration = ImGuizmo::SCALE;
	}
}
#endif