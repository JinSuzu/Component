#pragma once
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
inline void ImageWindowCenter(ID3D11ShaderResourceView* _tex, ImVec2 _size, ImVec2* _resultPos = nullptr)
{
	// 画面中央に固定するためのオフセット計算
	ImVec2 imagePos = ((ImGui::GetContentRegionAvail() - _size) * 0.5f);

	// 位置を設定
	ImGui::SetCursorPos(imagePos);

	ImGui::Image(_tex, _size);

	if (_resultPos)*_resultPos = imagePos;
}
/**
 * @fn
 * @brief Itemを親ウィンドウの中心に描画する
 * @param (_size) これから表示するItemのサイズ
 */
inline void SetCenterCursorPos(ImVec2 _size = ImVec2(), ImVec2* _resultPos = nullptr)
{
	// 画面中央に固定するためのオフセット計算
	ImVec2 imagePos = ((ImGui::GetContentRegionAvail() - _size) * 0.5f);

	// 位置を設定
	ImGui::SetCursorPos(imagePos);

	if (_resultPos)*_resultPos = imagePos;
}

inline void SetCenterCursorWidth(float _size = 0.0f)
{
	// 画面中央に固定するためのオフセット計算
	float imageX = ((ImGui::GetContentRegionAvail().x - _size) * 0.5f);
	// 位置を設定
	ImGui::SetCursorPosX(imageX);
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
	for (UINT num = 0; num < magic_enum::enum_count<Enum>(); num++)
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
		ImGui::Text("%s", _tag.c_str());
		ImGui::SameLine();
	}

	bool flg = false;
	char buffer[256];
	std::fill(std::begin(buffer), std::end(buffer), 0); // バッファを初期化

	// _editStrの長さがバッファサイズを超えないようにコピー
	std::copy_n(_editStr.begin(), std::min(_editStr.size(), sizeof(buffer) - 1), buffer);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // ImVec2の初期値を追加

	if (flg = ImGui::InputText(("##" + _tag + _editStr).c_str(), buffer, sizeof(buffer), _flg)) {
		_editStr = std::string(buffer);
	}
	ImGui::PopStyleVar();

	return flg;

}

template<class T>
inline static bool DragDropSource(std::string _tag, T _payload)
{
	bool flg = false;
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload(_tag.c_str(), &_payload, sizeof(T), ImGuiCond_Once);
		ImGui::Text(_tag.c_str());

		flg = true;

		ImGui::EndDragDropSource();
	}
	return flg;
}
inline static bool DragDropSource(std::string _tag, std::string _str)
{
	bool flg = false;
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload(_tag.c_str(), _str.c_str(), _str.size() + 1, ImGuiCond_Once);
		ImGui::Text(_tag.c_str());

		flg = true;

		ImGui::EndDragDropSource();
	}
	return flg;
}

template<class T>
inline bool DragDropTarget(std::string _tag, T& _payload)
{
	bool flg = false;
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_tag.c_str()))
		{
			IM_ASSERT(payload->DataSize == sizeof(T));
			_payload = *(T*)payload->Data;
			flg = true;
		}
		ImGui::EndDragDropTarget();
	}
	return flg;
}
inline bool DragDropTarget(std::string _tag, std::string& _str)
{
	bool flg = false;
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_tag.c_str()))
		{
			const char* payloadData = static_cast<const char*>(payload->Data);
			std::string receivedStr(payloadData);
			_str = receivedStr;
			flg = true;
		}
		ImGui::EndDragDropTarget();
	}
	return flg;
}


template<class T> struct DragDrop
{
	std::list<std::function<bool(T)>>  source;
	std::list<std::function<bool(T&)>> target;
	bool CallSource(T _value)
	{
		bool flg = false;
		for (auto& Fn : source)flg |= Fn(_value);
		return flg;
	}
	bool CallTarget(T& _value)
	{
		bool flg = false;
		for (auto& Fn : target)flg |= Fn(_value);
		return flg;
	}
};
