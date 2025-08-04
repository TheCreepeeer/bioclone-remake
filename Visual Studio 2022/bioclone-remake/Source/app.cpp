/*
*
*	Megan Grass
*	March 2, 2025
*
*/

#include "app.h"

std::unique_ptr<Global_Application> G = std::make_unique<Global_Application>();

void Global_Application::About(void) const
{
	Standard_String Str;

	String AboutStr = Str.FormatCStyle("%ws - %ws", VER_INTERNAL_NAME_STR, VER_FILE_DESCRIPTION_STR);
	AboutStr += Str.FormatCStyle("\r\nv%ws %s %s Megan Grass", VER_PRODUCT_VERSION_STR, __DATE__, __TIME__);
	AboutStr += Str.FormatCStyle("\r\n\r\n<a href=\"https://github.com/MeganGrass/%ws\">https://github.com/megangrass/%ws</a>", VER_INTERNAL_NAME_STR, VER_INTERNAL_NAME_STR);

	AboutStr += Str.FormatCStyle("\r\n\r\nConfiguration:\r\n<a href=\"%ws\">%ws</a>", GetConfigFilename().wstring().c_str(), GetConfigFilename().wstring().c_str());
	AboutStr += Str.FormatCStyle("\r\n<a href=\"%ws\">%ws</a>", GetImGuiConfigFilename().wstring().c_str(), GetImGuiConfigFilename().wstring().c_str());
	AboutStr += Str.FormatCStyle("\r\n<a href=\"%ws\">%ws</a>", GetToolbarIconFilename().wstring().c_str(), GetToolbarIconFilename().wstring().c_str());

	AboutStr += Str.FormatCStyle("\r\n\r\nimgui: <a href=\"https://github.com/ocornut/imgui\">https://github.com/ocornut/imgui</a>");
	AboutStr += Str.FormatCStyle("\r\n\r\nlibjpeg-turbo: <a href=\"https://github.com/libjpeg-turbo/libjpeg-turbo\">https://github.com/libjpeg-turbo/libjpeg-turbo</a>");
	AboutStr += Str.FormatCStyle("\r\n\r\nlibpng: <a href=\"https://github.com/pnggroup/libpng\">https://github.com/pnggroup/libpng</a>");
	AboutStr += Str.FormatCStyle("\r\n\r\nzlib: <a href=\"https://github.com/madler/zlib\">https://github.com/madler/zlib</a>");

	G->Window->MessageModal(L"About", L"", Str.GetWide(AboutStr));
}

void Global_Application::Controls(void) const
{
	Standard_String Str;

	String ControlsStr = Str.FormatCStyle("CTRL+O -- Open (any file type)");
	ControlsStr += Str.FormatCStyle("\r\nCTRL+N -- New Sony PlayStation Texture Image (*.TIM)");
	ControlsStr += Str.FormatCStyle("\r\nCTRL+S -- Save Sony PlayStation Texture Image (*.TIM)");
	ControlsStr += Str.FormatCStyle("\r\nUP/DOWN -- View previous/next texture (when file count > 1)");
	ControlsStr += Str.FormatCStyle("\r\nLEFT/RIGHT -- View previous/next color lookup table (palette)");
	ControlsStr += Str.FormatCStyle("\r\nCTRL+MOUSEWHEEL -- Adjust image zoom");
	ControlsStr += Str.FormatCStyle("\r\nF11 -- Enter/Exit fullscreen mode");
	ControlsStr += Str.FormatCStyle("\r\nCTRL+? -- About this application");
	ControlsStr += Str.FormatCStyle("\r\nESC -- Exit Application");

	G->Window->MessageModal(L"Controls", L"", Str.GetWide(ControlsStr));
}

void Global_Application::RenderScene(void)
{
	if (!m_RenderTexture || !m_RenderSurface) { return; }

	IDirect3DSurface9* OriginalSurface = nullptr;
	IDirect3DSurface9* TextureSurface = nullptr;

	{
		Render->Device()->GetRenderTarget(0, &OriginalSurface);
		Render->Device()->SetRenderTarget(0, m_RenderSurface.get());

		Render->Device()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(GetRValue(Window->GetColor()), GetGValue(Window->GetColor()), GetBValue(Window->GetColor())), 1.0f, 0);
		Render->Device()->BeginScene();
	}

	{
		if (Render->b_ViewGrid) { Render->DrawGrid(); }

		if (Render->b_ViewAxis) { Render->DrawAxis(); }

		DrawCamera();

		DrawSwitch();

		DrawCollision();

		DrawBlock();

		DrawFloor();

		Render->Device()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
		Render->Device()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
		Render->Device()->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_BORDER);
		Render->Device()->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0x00000000);

		Player->Draw();

		Render->Device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		Render->Device()->SetRenderState(D3DRS_CLIPPING, FALSE);

		DrawBackground();

		DrawSprite();
	}

	{
		Render->Device()->EndScene();
		Render->Device()->PresentEx(NULL, NULL, NULL, NULL, NULL);

		m_RenderTexture->GetSurfaceLevel(0, &TextureSurface);
		Render->Device()->StretchRect(m_RenderSurface.get(), nullptr, TextureSurface, nullptr, D3DTEXF_NONE);

		Render->Device()->SetRenderTarget(0, OriginalSurface);
	}
}

void Global_Application::Draw(void)
{
	if (Render->b_DeviceWasReset)
	{
		Render->b_DeviceWasReset = false;

		ImGui_ImplDX9_InvalidateDeviceObjects();
		ImGui_ImplDX9_CreateDeviceObjects();
	}

	if (b_RequestFontChange)
	{
		b_RequestFontChange = false;

		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->Clear();
		io.Fonts->AddFontFromFileTTF(Window->FontList()[Window->FontIndex()].string().c_str(), Window->FontSize());
		io.FontDefault = io.Fonts->Fonts.back();
		io.Fonts->Build();

		ImGui_ImplDX9_InvalidateDeviceObjects();
		ImGui_ImplDX9_CreateDeviceObjects();
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	{
		//bool show_demo_window = true;
		//ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 7.4f);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 7.4f);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 7.4f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.4f);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 7.4f);
		ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 7.4f);
		ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 7.4f);

		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));

		ImGui::GetStyle().WindowBorderSize = 0.0f;
		ImGui::GetStyle().Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);

		MainMenu();

		float totalWidth = ImGui::GetIO().DisplaySize.x - 4.0f;
		float leftPanelWidth = 200.0f;
		float rightPanelWidth = 420.0f;
		float centerPanelWidth = max(static_cast<float>(m_RenderWidth), totalWidth - leftPanelWidth - rightPanelWidth);

		float panelHeight = ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeightWithSpacing() - 2.0f;

		LeftPanel(ImVec2(2.0f, ImGui::GetFrameHeightWithSpacing()), ImVec2(leftPanelWidth, panelHeight));
		CenterPanel(ImVec2(leftPanelWidth + 2.0f, ImGui::GetFrameHeightWithSpacing()), ImVec2(centerPanelWidth, panelHeight));
		RightPanel(ImVec2(leftPanelWidth + centerPanelWidth + 2.0f, ImGui::GetFrameHeightWithSpacing()), ImVec2(rightPanelWidth, panelHeight));

		Options();

		ImGui::PopStyleColor();

		ImGui::PopStyleVar(7);
	}
	ImGui::EndFrame();

	{
		Render->Device()->SetRenderState(D3DRS_ZENABLE, FALSE);
		Render->Device()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		Render->Device()->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

		Render->Device()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(GetRValue(Window->GetColor()), GetGValue(Window->GetColor()), GetBValue(Window->GetColor())), 1.0f, 0);
		Render->Device()->BeginScene();

		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		Render->Device()->EndScene();
		Render->Device()->PresentEx(NULL, NULL, NULL, NULL, NULL);
	}
}

void Global_Application::Update(void)
{
	if (!ImGui::GetKeyData(ImGuiKey_Escape)->DownDuration)
	{
		PostMessage(Window->Get(), WM_CLOSE, 0, 0);
		return;
	}

	if (!ImGui::GetKeyData(ImGuiKey_F11)->DownDuration)
	{
		Window->AutoFullscreen();
	}

	if (!ImGui::GetKeyData(ImGuiKey_DownArrow)->DownDuration)
	{
	}

	if (!ImGui::GetKeyData(ImGuiKey_UpArrow)->DownDuration)
	{
	}

	if (!ImGui::GetKeyData(ImGuiKey_LeftArrow)->DownDuration)
	{
		if (ImGui::GetIO().KeyCtrl)
		{
			Player->iClip--;
			Player->iFrame = 0;
		}
		else
		{
			switch (Game)
			{
			case Video_Game::Resident_Evil_2_Trial:
			case Video_Game::Resident_Evil_2:
				if (Bio2->Rdt->IsOpen())
				{
					uint8_t i = Camera->SetImage(--Camera->Cut);
					Camera->Set(Bio2->Rdt->Rid->Get(i)->ViewR >> 7, Bio2->Rdt->Rid->Get(i)->View_p, Bio2->Rdt->Rid->Get(i)->View_r);
				}
				break;
			}
		}
	}

	if (!ImGui::GetKeyData(ImGuiKey_RightArrow)->DownDuration)
	{
		if (ImGui::GetIO().KeyCtrl)
		{
			Player->iClip++;
			Player->iFrame = 0;
		}
		else
		{
			switch (Game)
			{
			case Video_Game::Resident_Evil_2_Trial:
			case Video_Game::Resident_Evil_2:
				if (Bio2->Rdt->IsOpen())
				{
					uint8_t i = Camera->SetImage(++Camera->Cut);
					Camera->Set(Bio2->Rdt->Rid->Get(i)->ViewR >> 7, Bio2->Rdt->Rid->Get(i)->View_p, Bio2->Rdt->Rid->Get(i)->View_r);
				}
				break;
			}
		}
	}

	if (Camera->b_ViewTopDown)
	{
		if (ImGui::GetIO().KeyCtrl && Window->Device()->GetMouseDeltaZ())
		{
			Camera->m_Cy += Window->Device()->GetMouseDeltaZ() * 2.5f;
			Camera->SetTopDownPerspective();
		}
		else if (Window->Device()->GetMouseDeltaX())
		{
			Camera->m_Cx += Window->Device()->GetMouseDeltaX() * -0.25f;
			Camera->SetTopDownPerspective();
		}
		else if (Window->Device()->GetMouseDeltaZ())
		{
			Camera->m_Cz += Window->Device()->GetMouseDeltaZ() * -0.25f;
			Camera->SetTopDownPerspective();
		}
	}
}

void Global_Application::Shutdown(void)
{
	if (b_Shutdown) { return; }

	SaveConfig();

	Camera->Shutdown();

	//Player->Close();

	CloseRDT();

	Render->Shutdown();

	b_Shutdown = true;

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();

	//ImGui::DestroyContext(Context.get());

	//std::this_thread::sleep_for(std::chrono::milliseconds(10));

	PostQuitMessage(0);

	DestroyWindow(Window->Get());
}

void Global_Application::DragAndDrop(StrVecW Files) const
{
	/*for (std::size_t i = 0; i < Files.size(); i++)
	{
		StringW FileExtension = FS.GetFileExtension(Files[i]).wstring();

		Str.ToUpper(FileExtension);

		if ((FileExtension == L".TIM") || (FileExtension == L".BS"))
		{
			Open(Files[i]);
		}
	}*/

	//if (!Files.empty()) { TextureIO(Files[0], ImageIO::All); }
}

void Global_Application::Commandline(StrVecW Args)
{
	for (std::size_t i = 0; i < Args.size(); i++)
	{
		if (Str.ToUpper(Args[i]) == L"-FILE")
		{
			if ((i + 1 <= Args.size()) && (Standard_FileSystem().Exists(Args[i + 1])))
			{
				//b_OpenLastFileOnBoot = true;
				//m_Filename = Args[i + 1];
			}
		}
	}

	if (!Args.empty() && Standard_FileSystem().Exists(Args[0]))
	{
		//b_OpenLastFileOnBoot = true;
		//m_Filename = Args[0];
	}
}

int Global_Application::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	{
		if (Standard_FileSystem().Exists(GetConfigFilename()))
		{
			OpenConfig();
		}
		else
		{
			SaveConfig();

			std::filesystem::path Source = Str.FormatCStyle(L"%ws\\%ws", Window->GetCurrentWorkingDir().wstring().c_str(), L"icons.png");

			if (Standard_FileSystem().Exists(Source) && !Standard_FileSystem().Exists(GetToolbarIconFilename()))
			{
				std::filesystem::copy_file(Source, GetToolbarIconFilename(), std::filesystem::copy_options::overwrite_existing);
				std::filesystem::remove(Source);
			}
		}
	}
	{
		Commandline(Window->GetCommandline(lpCmdLine));
	}
	{
		m_ConfigStr = GetImGuiConfigFilename().string();

		IMGUI_CHECKVERSION();
		Context.reset(ImGui::CreateContext());
		ImGui::StyleColorsDark();

		{
			float CaptionRed = float(GetRValue(Window->GetCaptionColor())) / 255;
			float CaptionGreen = float(GetGValue(Window->GetCaptionColor())) / 255;
			float CaptionBlue = float(GetBValue(Window->GetCaptionColor())) / 255;

			m_BorderRed = float(GetRValue(Window->GetBorderColor())) / 255;
			m_BorderGreen = float(GetGValue(Window->GetBorderColor())) / 255;
			m_BorderBlue = float(GetBValue(Window->GetBorderColor())) / 255;

			ImGui::GetStyle().WindowBorderSize = 0.0f;

			ImGui::GetStyle().Colors[ImGuiCol_PopupBg] = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

			ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg] = ImVec4(CaptionRed, CaptionGreen, CaptionBlue, 1.0f);

			ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = ImVec4(CaptionRed, CaptionGreen, CaptionBlue, 1.0f);
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(CaptionRed, CaptionGreen, CaptionBlue, 1.0f);
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(CaptionRed, CaptionGreen, CaptionBlue, 1.0f);

			ImGui::GetStyle().Colors[ImGuiCol_Header] = ImVec4(CaptionRed, CaptionGreen, CaptionBlue, 1.0f);
			ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] = ImVec4(m_BorderRed, m_BorderGreen, m_BorderBlue, 1.0f);
			ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] = ImVec4(m_BorderRed * 2, m_BorderGreen * 2, m_BorderBlue * 2, 1.0f);

			ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(CaptionRed, CaptionGreen, CaptionBlue, 1.0f);
			ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(m_BorderRed, m_BorderGreen, m_BorderBlue, 1.0f);
			ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(m_BorderRed * 2, m_BorderGreen * 2, m_BorderBlue * 2, 1.0f);

			ImGui::GetStyle().Colors[ImGuiCol_Tab] = ImVec4(CaptionRed, CaptionGreen, CaptionBlue, 1.0f);
			ImGui::GetStyle().Colors[ImGuiCol_TabHovered] = ImVec4(m_BorderRed * 2, m_BorderGreen * 2, m_BorderBlue * 2, 1.0f);
			ImGui::GetStyle().Colors[ImGuiCol_TabSelected] = ImVec4(m_BorderRed, m_BorderGreen, m_BorderBlue, 1.0f);
			ImGui::GetStyle().Colors[ImGuiCol_TabSelectedOverline] = ImVec4(1, 0, 0, 0);

			ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImVec4(CaptionRed, CaptionGreen, CaptionBlue, 1.0f);
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] = ImVec4(m_BorderRed, m_BorderGreen, m_BorderBlue, 1.0f);
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] = ImVec4(m_BorderRed * 2, m_BorderGreen * 2, m_BorderBlue * 2, 1.0f);
		}

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = m_ConfigStr.c_str();
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.Fonts->Clear();
		io.Fonts->AddFontFromFileTTF(Window->GetFont().string().c_str(), Window->FontSize());
		io.FontDefault = io.Fonts->Fonts.back();
	}
	{
		extern LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
		extern LRESULT CALLBACK RenderProc(HWND, UINT, WPARAM, LPARAM);

		int DefaultWidth = 1920;
		int DefaultHeight = 1080;

		if (b_BootMaximized)
		{
			m_BootWidth = DefaultWidth;
			m_BootHeight = DefaultHeight;
		}

		if (b_BootFullscreen)
		{
			b_BootMaximized = false;
			m_BootWidth = DefaultWidth;
			m_BootHeight = DefaultHeight;
		}

		int Width = m_BootWidth;
		int Height = m_BootHeight;

		if ((!m_BootWidth) || (m_BootWidth < DefaultWidth) || (!m_BootHeight) || (m_BootHeight < DefaultHeight))
		{
			Width = DefaultWidth;
			Height = DefaultHeight;
		}

		Window->PresetStyle(WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
		Window->PresetStyleEx(WS_EX_ACCEPTFILES | WS_EX_APPWINDOW);
		Window->SetCaptionName(VER_PRODUCT_NAME_STR);
		Window->PresetClassName(VER_INTERNAL_NAME_STR);
		Window->SetIcon(hInstance, IDI_WINDOW);
		Window->SetIconSmall(hInstance, IDI_WINDOW);
		Window->Create(Width, Height, hInstance, SW_HIDE, WindowProc);

		Render->Initialize(Window->CreateChild(0, 0, Width, Height, hInstance, SW_SHOW, RenderProc, NULL, NULL), Width, Height, false);

		Window->SetTimer(60);

		while (!Render->NormalState()) { Window->SleepTimer(); }

		ShowWindow(Window->Get(), SW_SHOWDEFAULT);

		if (b_BootMaximized)
		{
			ShowWindow(Window->Get(), SW_SHOWMAXIMIZED);

			RECT Rect = G->Window->GetRect();
			WINDOWPOS WindowPos{};
			WindowPos.hwnd = G->Window->Get();
			WindowPos.hwndInsertAfter = nullptr;
			WindowPos.x = Rect.left;
			WindowPos.y = Rect.top;
			WindowPos.cx = (Rect.right - Rect.left);
			WindowPos.cy = (Rect.bottom - Rect.top);
			WindowPos.flags = SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_DEFERERASE | SWP_ASYNCWINDOWPOS;
			SendMessage(G->Render->Window()->Get(), WM_WINDOWPOSCHANGED, 0, (LPARAM)&WindowPos);
		}

		if (b_BootFullscreen)
		{
			Window->AutoFullscreen();
		}

		Str.hWnd = Window->Get();
	}
	{
		ImGui_ImplWin32_Init(Render->Window()->Get());
		ImGui_ImplDX9_Init(Render->Device());
	}

	{
		m_RenderTexture.reset(Render->CreateTexture((std::uint16_t)(m_RenderWidth = 1280), (std::uint16_t)(m_RenderHeight = 960), D3DUSAGE_RENDERTARGET));
		m_RenderTexture->GetLevelDesc(0, &m_RenderDesc);

		m_RenderSurface.reset(Render->CreateRenderSurface(m_RenderWidth, m_RenderHeight, D3DFMT_A8R8G8B8));

		Render->CreateAxisGrid();
	}

	{
		Geometry->Init();

		Camera->Str.hWnd = Window->Get();

		Camera->SetOrtho((float)m_RenderWidth, (float)m_RenderHeight);

		Camera->m_BackgroundVert.reset(Render->CreateVec4t(Camera->GetImageVert()));

		Camera->Set(Camera->m_FOV, Camera->m_Eye, Camera->m_At);
	}

	{
		Player->GTE = G->GTE;

		Player->Render = G->Render;

		Player->SetWindow(Window->Get());

		Player->SetGame(Game);
	}

	Standard_Thread_Pool ThreadPool;
	ThreadPool.ThreadPoolInit(1);

	MSG msg{};
	msg.message = NULL;
	msg.hwnd = Window->Get();

	static bool b_Active = true;
	
	while (b_Active)
	{
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

			if ((msg.message == WM_QUIT) || (msg.message == WM_DESTROY))
			{
				b_Active = false;
			}
		}

		ThreadPool.ThreadPoolEnqueue([this]()
			{
				if (Window->IsMinimized() || !Render->NormalState() || !Context) { return; }

				if (!Window->GetDroppedFiles().empty()) { DragAndDrop(Window->GetDroppedFiles()); Window->GetDroppedFiles().clear(); }

				Update();

				RenderScene();

				Draw();
			});

		if (b_ForceShutdown)
		{
			b_Active = false;
		}

		Window->SleepTimer();
	}

	Shutdown();

	return (int)msg.wParam;
}