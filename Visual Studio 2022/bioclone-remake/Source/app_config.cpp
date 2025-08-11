/*
*
*	Megan Grass
*	March 2, 2025
*
*/

#include "app.h"

#include <std_text.h>

void Global_Application::OpenConfig(void)
{
	if (b_Shutdown) { return; }

	std::unique_ptr<StdText> Text = std::make_unique<StdText>(GetConfigFilename(), FileAccessMode::Read_Ex, TextFileBOM::UTF16_LE);
	if (!Text->IsOpen())
	{
		Str.Message("Error, could not open \"%ws\" app config", GetConfigFilename().filename().wstring().c_str());
		return;
	}

	for (std::size_t i = 0; i < Text->GetLineCount(); i++)
	{
		StrVecW Args = Text->GetArgsW(i);

		if (!Args.empty() && (Args.size() > 1))
		{
			if (Str.ToUpper(Args[0]) == L"M_FONT")
			{
				Window->SetFont(Args[1], Window->FontSize());
			}

			if (Str.ToUpper(Args[0]) == L"M_FONTSIZE")
			{
				Window->FontSize() = std::stof(Args[1]);
				Window->FontSize() = std::clamp(Window->FontSize(), Window->FontSizeMin(), Window->FontSizeMax());
			}

			if (Str.ToUpper(Args[0]) == L"M_WINDOWCOLOR")
			{
				DWORD Color = std::stoul(Args[1], nullptr, 16);
				Window->SetColor(GetRValue(Color), GetGValue(Color), GetBValue(Color));
			}

			if (Str.ToUpper(Args[0]) == L"M_WIDTH")
			{
				m_BootWidth = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"M_HEIGHT")
			{
				m_BootHeight = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_MAXIMIZED")
			{
				b_BootMaximized = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_FULLSCREEN")
			{
				b_BootFullscreen = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_VIEWWINDOWOPTIONS")
			{
				b_ViewWindowOptions = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_VIEWGRID")
			{
				Render->b_ViewGrid = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_VIEWAXIS")
			{
				Render->b_ViewAxis = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_VIEWBACKGROUND")
			{
				Camera->b_ViewBackground = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_CAMERALINE")
			{
				Camera->b_DrawLine = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_VIEWSPRITE")
			{
				Camera->b_ViewSprite = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_VIEWSWITCH")
			{
				Camera->b_DrawSwitch = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_VIEWCOLLISION")
			{
				Geometry->b_DrawCollision = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_VIEWBLOCK")
			{
				Geometry->b_DrawBlock = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_VIEWFLOOR")
			{
				Geometry->b_DrawFloor = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_SOLIDCOLLISION")
			{
				Geometry->b_SolidCollision = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_SOLIDCOLLISIONALL")
			{
				Geometry->b_SolidCollisionAll = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_SHAPECOLLISION")
			{
				Geometry->b_ShapeCollision = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_SHAPECOLLISIONALL")
			{
				Geometry->b_ShapeCollisionAll = std::stoi(Args[1]);
			}

			if (Str.ToUpper(Args[0]) == L"B_HIGHLIGHTCOLLISION")
			{
				Geometry->b_HighlightCollision = std::stoi(Args[1]);
			}
		}
	}
}

void Global_Application::SaveConfig(void)
{
	if (b_Shutdown) { return; }

	std::unique_ptr<StdText> Text = std::make_unique<StdText>(GetConfigFilename(), FileAccessMode::Write_Ex, TextFileBOM::UTF16_LE);
	if (!Text->IsOpen())
	{
		Str.Message("Error, could not create \"%ws\" app config", GetConfigFilename().filename().wstring().c_str());
		return;
	}

	Text->AddLine(L"app\t\"%ws\"\r", Window->GetModuleStr().c_str());

	Text->AddLine(L"m_Font\t%ws\r", Window->GetFont().stem().wstring().c_str());
	Text->AddLine(L"m_FontSize\t%.0f\r", Window->FontSize());
	Text->AddLine(L"m_WindowColor\t0x%x\r", Window->GetColor());
	Text->AddLine(L"m_Width\t%d\r", (Window->GetRect().right - Window->GetRect().left));
	Text->AddLine(L"m_Height\t%d\r", (Window->GetRect().bottom - Window->GetRect().top));
	Text->AddLine(L"b_Maximized\t%d\r", Window->IsMaximized());
	Text->AddLine(L"b_Fullscreen\t%d\r", Window->IsFullscreen());

	Text->AddLine(L"b_ViewWindowOptions\t%d\r", b_ViewWindowOptions);

	Text->AddLine(L"b_ViewGrid\t%d\r", Render->b_ViewGrid);
	Text->AddLine(L"b_ViewAxis\t%d\r", Render->b_ViewAxis);

	Text->AddLine(L"b_ViewBackground\t%d\r", Camera->b_ViewBackground);
	Text->AddLine(L"b_CameraLine\t%d\r", Camera->b_DrawLine);
	Text->AddLine(L"b_ViewSprite\t%d\r", Camera->b_ViewSprite);
	Text->AddLine(L"b_ViewSwitch\t%d\r", Camera->b_DrawSwitch);
	Text->AddLine(L"b_ViewCollision\t%d\r", Geometry->b_DrawCollision);
	Text->AddLine(L"b_ViewBlock\t%d\r", Geometry->b_DrawBlock);
	Text->AddLine(L"b_ViewFloor\t%d\r", Geometry->b_DrawFloor);

	Text->AddLine(L"b_SolidCollision\t%d\r", Geometry->b_SolidCollision);
	Text->AddLine(L"b_SolidCollisionAll\t%d\r", Geometry->b_SolidCollisionAll);
	Text->AddLine(L"b_ShapeCollision\t%d\r", Geometry->b_ShapeCollision);
	Text->AddLine(L"b_ShapeCollisionAll\t%d\r", Geometry->b_ShapeCollisionAll);
	Text->AddLine(L"b_HighlightCollision\t%d\r", Geometry->b_HighlightCollision);

	Text->FlushUTF16();
}