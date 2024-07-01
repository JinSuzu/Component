#pragma once
namespace MyImGui
{
	inline bool ButtonWindowCenter(std::string label)
	{
		// ボタンの幅と高さを取得
		ImVec2 button_size = ImGui::CalcTextSize(label.c_str());
		button_size.x += ImGui::GetStyle().FramePadding.x * 2.0f;
		button_size.y += ImGui::GetStyle().FramePadding.y * 2.0f;

		// ウィンドウの幅を取得
		float window_width = ImGui::GetWindowSize().x;

		// ボタンを中央揃えにするためのx位置を計算
		float x_pos = (window_width - button_size.x) / 2.0f;

		// 位置を設定してボタンを描画
		ImGui::SetCursorPosX(x_pos);
		return ImGui::Button(label.c_str());
	}
	inline bool SmallButtonWindowCenter(std::string label)
	{
		// ボタンの幅と高さを取得
		ImVec2 button_size = ImGui::CalcTextSize(label.c_str());
		button_size.x += ImGui::GetStyle().FramePadding.x * 2.0f;
		button_size.y += ImGui::GetStyle().FramePadding.y * 2.0f;

		// ウィンドウの幅を取得
		float window_width = ImGui::GetWindowSize().x;

		// ボタンを中央揃えにするためのx位置を計算
		float x_pos = (window_width - button_size.x) / 2.0f;

		// 位置を設定してボタンを描画
		ImGui::SetCursorPosX(x_pos);
		return ImGui::SmallButton(label.c_str());
	}
	inline bool CenterButton(std::string label, ImVec2 size = {})
	{
		//ボタンのサイズを取得
		ImVec2 button_size = { size.x,size.y };
		if (size == ImVec2())
		{
			button_size = ImGui::CalcTextSize(label.c_str());
			button_size.x += ImGui::GetStyle().FramePadding.x * 2.0f;
			button_size.y += ImGui::GetStyle().FramePadding.y * 2.0f;
		}

		// ツリーノードの幅を取得
		float tree_node_width = ImGui::GetContentRegionAvail().x;

		// ボタンを中央揃えにするためのx位置を計算
		float x_pos = (tree_node_width - button_size.x) / 2.0f;

		// 位置を設定してボタンを描画
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_pos);
		return ImGui::Button(label.c_str(), size);
	}

	inline bool CheckBoxBit(std::string _name, UINT& _ID, UINT _checkID)
	{
		bool flg = _ID & _checkID;
		bool change = ImGui::Checkbox(_name.c_str(), &flg);
		if (flg) _ID |= _checkID;
		else _ID &= (~_checkID);
		return change;
	}
	template<class Enum> inline bool CheckBoxAllBit(UINT& _ID)
	{
		bool change = false;
		for (int num = 0; num < magic_enum::enum_count<Enum>(); num++)
		{
			UINT checkID = 1 << num;
			bool flg = _ID & checkID;
			Enum type = static_cast<Enum>(checkID);
			bool edit = ImGui::Checkbox(magic_enum::enum_name(type).data(), &flg);
			change = change || edit;
			if (edit)
			{
				if (flg) _ID |= checkID;
				else _ID &= (~checkID);
			}
		}
		return change;
	}


	inline bool InputText(std::string _tag, std::string& _editStr, ImGuiInputTextFlags _flg = ImGuiInputTextFlags_EnterReturnsTrue)
	{
		if (!_tag.empty())
		{
			ImGui::Text(_tag.c_str());
			ImGui::SameLine();
		}

		bool flg = false;
		char buffer[256];
		std::strncpy(buffer, _editStr.c_str(), sizeof(buffer));
		if (flg = ImGui::InputText(("##" + _tag).c_str(), buffer, sizeof(buffer), _flg)) {
			_editStr = std::string(buffer);
		}

		return flg;
	}
};