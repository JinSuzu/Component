#pragma once
//ImGuiZmoが存在しているか
#ifdef IMGUIZMO_NAMESPACE
inline bool Update(Math::Matrix _cameraMat, Math::Matrix _projectMat, Math::Matrix _targetMat, ImGuizmo::OPERATION& _opreration, Math::Matrix& _resultMat, ImRect _windowRect = { ImGui::GetWindowPos(),ImGui::GetWindowSize()})
{
	// Manipulate関数に渡す行列データのポインタ
	float* matrixPtr		= reinterpret_cast<float*>(&_cameraMat);
	float* proMatrixPtr		= reinterpret_cast<float*>(&_projectMat);
	float* TargetMatrixPtr	= reinterpret_cast<float*>(&_targetMat);

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(_windowRect.Min.x, _windowRect.Min.y, _windowRect.Max.x, _windowRect.Max.y);
	ImGuizmo::Manipulate(
		matrixPtr,					// カメラのビュー行列
		proMatrixPtr,				// カメ=ラのプロジェクション行列
		_opreration,				// 操作モード
		ImGuizmo::MODE::WORLD,      // 変換モード
		TargetMatrixPtr				// オブジェクトの変換行列
	);

	// 操作してる時のみ反映
	if (ImGuizmo::IsUsing())
	{
		// 結果をMatrixに変換
		Math::Matrix resultWMatrix;
		std::memcpy(&resultWMatrix, TargetMatrixPtr, sizeof(Math::Matrix)); // メモリのコピー
		_resultMat = resultWMatrix;
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