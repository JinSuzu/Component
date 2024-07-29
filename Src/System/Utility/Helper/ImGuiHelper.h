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

inline bool BeginDragDropSource(ImGuiDragDropFlags flags, ImGuiMouseButton _mouse_button)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	// FIXME-DRAGDROP: While in the common-most "drag from non-zero active id" case we can tell the mouse button,
	// in both SourceExtern and id==0 cases we may requires something else (explicit flags or some heuristic).

	bool source_drag_active = false;
	ImGuiID source_id = 0;
	ImGuiID source_parent_id = 0;
	if ((flags & ImGuiDragDropFlags_SourceExtern) == 0)
	{
		source_id = g.LastItemData.ID;
		if (source_id != 0)
		{
			// Common path: items with ID
			if (g.ActiveId != source_id)
				return false;
			if (g.ActiveIdMouseButton != -1)
				_mouse_button = g.ActiveIdMouseButton;
			if (g.IO.MouseDown[_mouse_button] == false || window->SkipItems)
				return false;
			g.ActiveIdAllowOverlap = false;
		}
		else
		{
			// Uncommon path: items without ID
			if (g.IO.MouseDown[_mouse_button] == false || window->SkipItems)
				return false;
			if ((g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect) == 0 && (g.ActiveId == 0 || g.ActiveIdWindow != window))
				return false;

			// If you want to use BeginDragDropSource() on an item with no unique identifier for interaction, such as Text() or Image(), you need to:
			// A) Read the explanation below, B) Use the ImGuiDragDropFlags_SourceAllowNullID flag.
			if (!(flags & ImGuiDragDropFlags_SourceAllowNullID))
			{
				IM_ASSERT(0);
				return false;
			}

			// Magic fallback to handle items with no assigned ID, e.g. Text(), Image()
			// We build a throwaway ID based on current ID stack + relative AABB of items in window.
			// THE IDENTIFIER WON'T SURVIVE ANY REPOSITIONING/RESIZINGG OF THE WIDGET, so if your widget moves your dragging operation will be canceled.
			// We don't need to maintain/call ClearActiveID() as releasing the button will early out this function and trigger !ActiveIdIsAlive.
			// Rely on keeping other window->LastItemXXX fields intact.
			source_id = g.LastItemData.ID = window->GetIDFromRectangle(g.LastItemData.Rect);
			ImGui::KeepAliveID(source_id);
			bool is_hovered = ImGui::ItemHoverable(g.LastItemData.Rect, source_id, g.LastItemData.InFlags);
			if (is_hovered && g.IO.MouseClicked[_mouse_button])
			{
				ImGui::SetActiveID(source_id, window);
				ImGui::FocusWindow(window);
			}
			if (g.ActiveId == source_id) // Allow the underlying widget to display/return hovered during the mouse release frame, else we would get a flicker.
				g.ActiveIdAllowOverlap = is_hovered;
		}
		if (g.ActiveId != source_id)
			return false;
		source_parent_id = window->IDStack.back();
		source_drag_active = ImGui::IsMouseDragging(_mouse_button);

		// Disable navigation and key inputs while dragging + cancel existing request if any
		ImGui::SetActiveIdUsingAllKeyboardKeys();
	}
	else
	{
		// When ImGuiDragDropFlags_SourceExtern is set:
		window = NULL;
		source_id = ImHashStr("#SourceExtern");
		source_drag_active = true;
		_mouse_button = g.IO.MouseDown[0] ? 0 : -1;
		ImGui::KeepAliveID(source_id);
		ImGui::SetActiveID(source_id, NULL);
	}

	IM_ASSERT(g.DragDropWithinTarget == false); // Can't nest BeginDragDropSource() and BeginDragDropTarget()
	if (!source_drag_active)
		return false;

	// Activate drag and drop
	if (!g.DragDropActive)
	{
		IM_ASSERT(source_id != 0);
		ImGui::ClearDragDrop();
		IMGUI_DEBUG_LOG_ACTIVEID("[dragdrop] BeginDragDropSource() DragDropActive = true, source_id = 0x%08X%s\n",
			source_id, (flags & ImGuiDragDropFlags_SourceExtern) ? " (EXTERN)" : "");
		ImGuiPayload& payload = g.DragDropPayload;
		payload.SourceId = source_id;
		payload.SourceParentId = source_parent_id;
		g.DragDropActive = true;
		g.DragDropSourceFlags = flags;
		g.DragDropMouseButton = _mouse_button;
		if (payload.SourceId == g.ActiveId)
			g.ActiveIdNoClearOnFocusLoss = true;
	}
	g.DragDropSourceFrameCount = g.FrameCount;
	g.DragDropWithinSource = true;

	if (!(flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
	{
		// Target can request the Source to not display its tooltip (we use a dedicated flag to make this request explicit)
		// We unfortunately can't just modify the source flags and skip the call to BeginTooltip, as caller may be emitting contents.
		bool ret;
		if (g.DragDropAcceptIdPrev && (g.DragDropAcceptFlags & ImGuiDragDropFlags_AcceptNoPreviewTooltip))
			ret = ImGui::BeginTooltipHidden();
		else
			ret = ImGui::BeginTooltip();
		IM_ASSERT(ret); // FIXME-NEWBEGIN: If this ever becomes false, we need to Begin("##Hidden", NULL, ImGuiWindowFlags_NoSavedSettings) + SetWindowHiddendAndSkipItemsForCurrentFrame().
		IM_UNUSED(ret);
	}

	if (!(flags & ImGuiDragDropFlags_SourceNoDisableHover) && !(flags & ImGuiDragDropFlags_SourceExtern))
		g.LastItemData.StatusFlags &= ~ImGuiItemStatusFlags_HoveredRect;

	return true;
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
