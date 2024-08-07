﻿#pragma once
//ImGuiZmoが存在しているか
#ifdef IMGUIZMO_NAMESPACE

inline bool Update(Math::Matrix _cameraMat, Math::Matrix _projectMat, Math::Matrix& _targetMat, ImGuizmo::OPERATION& _opreration, ImGuizmo::MODE& _mode, ImRect _windowRect = { ImGui::GetWindowPos(),ImGui::GetWindowSize() })
{
	// Manipulate関数に渡す行列データのポインタ
	float* matrixPtr = _cameraMat.m[0];
	float* proMatrixPtr = _projectMat.m[0];
	float* TargetMatrixPtr = _targetMat.m[0];

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(_windowRect.Min.x, _windowRect.Min.y, _windowRect.Max.x, _windowRect.Max.y);
	ImGuizmo::Manipulate(
		matrixPtr,					// カメラのビュー行列
		proMatrixPtr,				// カメ=ラのプロジェクション行列
		_opreration,				// 操作モード
		_mode,						// 変換モード
		TargetMatrixPtr				// オブジェクトの変換行列
	);

	// 操作してる時を返す
	return ImGuizmo::IsUsing();
}

inline void OprerationChanger(ImGuizmo::OPERATION& _opreration)
{
	// ギズモの操作切り替えボタン
	std::string str = magic_enum::enum_name(_opreration).data();
	if (ImGui::BeginCombo("OPERATION", str.c_str()))
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
	if (ImGui::BeginCombo("MODE", str.c_str()))
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