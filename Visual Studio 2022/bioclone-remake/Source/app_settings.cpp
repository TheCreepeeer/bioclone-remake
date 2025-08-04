/*
*
*	Megan Grass
*	March 2, 2025
* 
*/

#include "app.h"

void Global_Application::Tooltip(String Tip)
{
	ImGui::SameLine(); ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(Tip.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void Global_Application::TooltipOnHover(String Tip)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip))
	{
		ImGui::BeginTooltip();
		ImGui::Text(Tip.c_str());
		ImGui::EndTooltip();
	}
}

bool Global_Application::ScrollOnHover(void* Input, ImGuiDataType DataType, std::uintmax_t Step, std::uintmax_t Min, std::uintmax_t Max, std::function<void(void)> OnComplete) const
{
	if (ImGui::GetIO().MouseWheel == 0.0f) { return false; }

	if (ImGui::IsItemHovered())
	{
		ImGui::SetItemKeyOwner(ImGuiKey_MouseWheelX);
		ImGui::SetItemKeyOwner(ImGuiKey_MouseWheelY);

		bool b_Add = ImGui::GetIO().MouseWheel > 0.0f;

		if (DataType == ImGuiDataType_S8)
		{
			b_Add ? *(int8_t*)Input += static_cast<std::int8_t>(Step) : *(int8_t*)Input -= static_cast<std::int8_t>(Step);
			*(int8_t*)Input = std::clamp(*(int8_t*)Input, (int8_t)Min, (int8_t)Max);
		}
		else if (DataType == ImGuiDataType_U8)
		{
			b_Add ? *(uint8_t*)Input += static_cast<std::uint8_t>(Step) : *(uint8_t*)Input -= static_cast<std::uint8_t>(Step);
			*(uint8_t*)Input = std::clamp(*(uint8_t*)Input, (uint8_t)Min, (uint8_t)Max);
		}
		else if (DataType == ImGuiDataType_S16)
		{
			b_Add ? *(int16_t*)Input += static_cast<std::int16_t>(Step) : *(int16_t*)Input -= static_cast<std::int16_t>(Step);
			*(int16_t*)Input = std::clamp(*(int16_t*)Input, (int16_t)Min, (int16_t)Max);
		}
		else if (DataType == ImGuiDataType_U16)
		{
			b_Add ? *(uint16_t*)Input += static_cast<std::uint16_t>(Step) : *(uint16_t*)Input -= static_cast<std::uint16_t>(Step);
			*(uint16_t*)Input = std::clamp(*(uint16_t*)Input, (uint16_t)Min, (uint16_t)Max);
		}
		else if (DataType == ImGuiDataType_S32)
		{
			b_Add ? *(int32_t*)Input += static_cast<std::int32_t>(Step) : *(int32_t*)Input -= static_cast<std::int32_t>(Step);
			*(int32_t*)Input = std::clamp(*(int32_t*)Input, (int32_t)Min, (int32_t)Max);
		}
		else if (DataType == ImGuiDataType_U32)
		{
			b_Add ? *(uint32_t*)Input += static_cast<std::uint32_t>(Step) : *(uint32_t*)Input -= static_cast<std::uint32_t>(Step);
			*(uint32_t*)Input = std::clamp(*(uint32_t*)Input, (uint32_t)Min, (uint32_t)Max);
		}
		else if (DataType == ImGuiDataType_S64)
		{
			b_Add ? *(int64_t*)Input += static_cast<std::int64_t>(Step) : *(int64_t*)Input -= static_cast<std::int64_t>(Step);
			*(int64_t*)Input = std::clamp(*(int64_t*)Input, (int64_t)Min, (int64_t)Max);
		}
		else if (DataType == ImGuiDataType_U64)
		{
			b_Add ? *(uint64_t*)Input += static_cast<std::uint64_t>(Step) : *(uint64_t*)Input -= static_cast<std::uint64_t>(Step);
			*(uint64_t*)Input = std::clamp(*(uint64_t*)Input, Min, Max);
		}
		else if (DataType == ImGuiDataType_Bool)
		{
			if (!b_Add && *(bool*)Input == false) { return false; }
			b_Add ? *(bool*)Input = true : *(bool*)Input = false;
		}

		OnComplete();

		return true;
	}

	return false;
}

bool Global_Application::ScrollFloatOnHover(void* Input, ImGuiDataType DataType, double Step, double Min, double Max, std::function<void(void)> OnComplete) const
{
	if (ImGui::GetIO().MouseWheel == 0.0f) { return false; }

	if (ImGui::IsItemHovered())
	{
		ImGui::SetItemKeyOwner(ImGuiKey_MouseWheelX);
		ImGui::SetItemKeyOwner(ImGuiKey_MouseWheelY);

		bool b_Add = ImGui::GetIO().MouseWheel > 0.0f;

		if (DataType == ImGuiDataType_Float)
		{
			b_Add ? *(float*)Input += static_cast<float>(Step) : *(float*)Input -= static_cast<float>(Step);
			*(float*)Input = std::clamp(*(float*)Input, (float)Min, (float)Max);
		}
		else if (DataType == ImGuiDataType_Double)
		{
			b_Add ? *(double*)Input += Step : *(double*)Input -= Step;
			*(double*)Input = std::clamp(*(double*)Input, (double)Min, (double)Max);
		}

		OnComplete();

		return true;
	}

	return false;
}

bool Global_Application::ScrollComboOnHover(String ID, void* Input, ImGuiDataType DataType, std::uintmax_t Step, std::uintmax_t Min, std::uintmax_t Max, std::function<void(void)> OnComplete) const
{
	if (ImGui::GetIO().MouseWheel == 0.0f) { return false; }

	if (ImGui::IsItemHovered() && !ImGui::IsPopupOpen(ID.c_str()))
	{
		ImGui::SetItemKeyOwner(ImGuiKey_MouseWheelX);
		ImGui::SetItemKeyOwner(ImGuiKey_MouseWheelY);

		if (DataType == ImGuiDataType_S8)
		{
			*(int8_t*)Input -= static_cast<std::int8_t>(ImGui::GetIO().MouseWheel);
			*(int8_t*)Input = std::clamp(*(int8_t*)Input, (int8_t)Min, (int8_t)Max);
		}
		else if (DataType == ImGuiDataType_U8)
		{
			*(uint8_t*)Input -= static_cast<std::uint8_t>(ImGui::GetIO().MouseWheel);
			*(uint8_t*)Input = std::clamp(*(uint8_t*)Input, (uint8_t)Min, (uint8_t)Max);
		}
		else if (DataType == ImGuiDataType_S16)
		{
			*(int16_t*)Input -= static_cast<std::int16_t>(ImGui::GetIO().MouseWheel);
			*(int16_t*)Input = std::clamp(*(int16_t*)Input, (int16_t)Min, (int16_t)Max);
		}
		else if (DataType == ImGuiDataType_U16)
		{
			*(uint16_t*)Input -= static_cast<std::uint16_t>(ImGui::GetIO().MouseWheel);
			*(uint16_t*)Input = std::clamp(*(uint16_t*)Input, (uint16_t)Min, (uint16_t)Max);
		}
		else if (DataType == ImGuiDataType_S32)
		{
			*(int32_t*)Input -= static_cast<std::int32_t>(ImGui::GetIO().MouseWheel);
			*(int32_t*)Input = std::clamp(*(int32_t*)Input, (int32_t)Min, (int32_t)Max);
		}
		else if (DataType == ImGuiDataType_U32)
		{
			*(uint32_t*)Input -= static_cast<std::uint32_t>(ImGui::GetIO().MouseWheel);
			*(uint32_t*)Input = std::clamp(*(uint32_t*)Input, (uint32_t)Min, (uint32_t)Max);
		}
		else if (DataType == ImGuiDataType_S64)
		{
			*(int64_t*)Input -= static_cast<std::int64_t>(ImGui::GetIO().MouseWheel);
			*(int64_t*)Input = std::clamp(*(int64_t*)Input, (int64_t)Min, (int64_t)Max);
		}
		else if (DataType == ImGuiDataType_U64)
		{
			*(uint64_t*)Input -= static_cast<std::uint64_t>(ImGui::GetIO().MouseWheel);
			*(uint64_t*)Input = std::clamp(*(uint64_t*)Input, (uint64_t)Min, (uint64_t)Max);
		}
		else if (DataType == ImGuiDataType_Bool)
		{
			bool b_Add = ImGui::GetIO().MouseWheel > 0.0f;
			if (!b_Add && *(bool*)Input == false) { return false; }
			b_Add ? *(bool*)Input = true : *(bool*)Input = false;
		}

		OnComplete();

		return true;
	}

	return false;
}

void Global_Application::DrawVerticalLine(float HorizontalIndent, float VerticalIndent, float Thickness, float Red, float Green, float Blue, float Alpha)
{
	ImVec2 p1 = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x + HorizontalIndent, ImGui::GetItemRectMax().y + HorizontalIndent);
	ImVec2 p2 = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x - HorizontalIndent, ImGui::GetItemRectMax().y + HorizontalIndent);
	ImGui::GetWindowDrawList()->AddLine(p1, p2, IM_COL32(Red * 255, Green * 255, Blue * 255, Alpha * 255), Thickness);
	ImGui::Dummy(ImVec2(0, VerticalIndent));
}