#include "../main.h"
#include "fuck.h"


auto createwindow() -> bool
{

	// if u choose to create a window instead of hooking onto one
	/*
	WNDCLASSEXA wcex = {
   sizeof(WNDCLASSEXA), 0, DefWindowProcA, 0, 0, nullptr,
   LoadIcon(nullptr, IDI_APPLICATION), LoadCursor(nullptr, IDC_ARROW),
   nullptr, nullptr, (("Chrome_WidgetWin_1")), LoadIcon(nullptr, IDI_APPLICATION)
	};

	RECT Rect;
	GetWindowRect(GetDesktopWindow(), &Rect);

	RegisterClassExA(&wcex);

	Overlay.Hwnd = CreateWindowA(("Chrome_WidgetWin_1"), ("Utils"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);
	if (Overlay.Hwnd == NULL) {
		return false;
	}

	SetWindowLong(Overlay.Hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(Overlay.Hwnd, &margin);
	ShowWindow(Overlay.Hwnd, SW_SHOW);
	SetWindowPos(Overlay.Hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetLayeredWindowAttributes(Overlay.Hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	UpdateWindow(Overlay.Hwnd);
	return true;
	*/

	// turn off legacy overlay on fortnite and enable new overlay
	Overlay.Hwnd = FindWindowA(("Chrome_WidgetWin_1"), ("Discord Overlay"));

	if (!Overlay.Hwnd)
	{
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		MessageBoxA(0, ("Failed To Find Discord."), ("Overlay"), MB_OK);
		Sleep(5000);
		exit(1);
		return false;
	}

	ShowWindow(Overlay.Hwnd, SW_SHOW);

	SetWindowLongA(Overlay.Hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
	SetWindowLongA(
		Overlay.Hwnd,
		-20,
		static_cast<LONG_PTR>(
			static_cast<int>(GetWindowLongA(Overlay.Hwnd, -20)) | 0x20
			)
	);

	MARGINS margin = { -1, -1, -1, -1 };
	DwmExtendFrameIntoClientArea(
		Overlay.Hwnd,
		&margin
	);

	SetLayeredWindowAttributes(
		Overlay.Hwnd,
		NULL,
		0xFF,
		0x02
	);

	SetWindowPos(
		Overlay.Hwnd,
		HWND_TOPMOST,
		0, 0, 0, 0,
		0x0002 | 0x0001
	);



	UpdateWindow(Overlay.Hwnd);
	HWND(overlaywindow);
	return true;
}

auto dx9() -> bool {
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &DirectX9.IDirect3D9))) {
		return false;
	}


	D3DPRESENT_PARAMETERS Params = { 0 };
	Params.Windowed = TRUE;
	Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Params.hDeviceWindow = Overlay.Hwnd;
	Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	Params.BackBufferWidth = Width;
	Params.BackBufferHeight = Height;
	Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	Params.EnableAutoDepthStencil = TRUE;
	Params.AutoDepthStencilFormat = D3DFMT_D16;
	Params.FullScreen_RefreshRateInHz = D3DPRESENT_INTERVAL_DEFAULT;
	if (FAILED(DirectX9.IDirect3D9->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Overlay.Hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Params, 0, &DirectX9.pDevice))) {
		DirectX9.IDirect3D9->Release();
		return false;
	}

	D3DXCreateTextureFromFileInMemory(DirectX9.pDevice, (LPVOID)rawData, sizeof(rawData), &sillycar);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\\Fonts\\ebrima.ttf", 16.0f);

	// Style tweaks
	style.WindowRounding = 5.0f;
	style.FrameRounding = 2.0f;


	// Color definitions
	ImVec4 bg10 = ImVec4(10.0f / 255.0f, 10.0f / 255.0f, 10.0f / 255.0f, 1.00f); // Very dark gray
	ImVec4 midGray = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	ImVec4 orange = ImVec4(1.00f, 0.45f, 0.05f, 1.00f);
	ImVec4 orangeHover = ImVec4(1.00f, 0.55f, 0.15f, 0.85f);
	ImVec4 orangeActive = ImVec4(1.00f, 0.50f, 0.10f, 1.00f);

	// Assign colors
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = bg10;
	colors[ImGuiCol_ChildBg] = bg10;
	colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.25f, 0.60f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);  // Checkbox background (darker)
	colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.55f, 0.15f, 0.75f);  // Orange hover
	colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.45f, 0.05f, 0.90f);  // Orange active
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.60f, 0.20f, 1.00f);  // Checkmark color
	colors[ImGuiCol_TitleBgActive] = orange;
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 0.75f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.35f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = orangeHover;
	colors[ImGuiCol_ScrollbarGrabActive] = orangeActive;
	colors[ImGuiCol_CheckMark] = orange;
	colors[ImGuiCol_SliderGrab] = orange;
	colors[ImGuiCol_SliderGrabActive] = orangeHover;
	colors[ImGuiCol_Button] = ImVec4(orange.x, orange.y, orange.z, 0.65f);
	colors[ImGuiCol_ButtonHovered] = orangeHover;
	colors[ImGuiCol_ButtonActive] = orangeActive;
	colors[ImGuiCol_Header] = ImVec4(orange.x, orange.y, orange.z, 0.7f);
	colors[ImGuiCol_HeaderHovered] = orangeHover;
	colors[ImGuiCol_HeaderActive] = orangeActive;
	colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.60f);
	colors[ImGuiCol_SeparatorHovered] = orangeHover;
	colors[ImGuiCol_SeparatorActive] = orangeActive;
	colors[ImGuiCol_ResizeGrip] = ImVec4(orange.x, orange.y, orange.z, 0.3f);
	colors[ImGuiCol_ResizeGripHovered] = orangeHover;
	colors[ImGuiCol_ResizeGripActive] = orangeActive;
	colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 0.85f);
	colors[ImGuiCol_TabHovered] = orangeHover;
	colors[ImGuiCol_TabActive] = orangeActive;
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.12f, 0.90f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(orange.x, orange.y, orange.z, 0.4f);
	colors[ImGuiCol_DragDropTarget] = orange;
	colors[ImGuiCol_NavHighlight] = orangeHover;
	colors[ImGuiCol_NavWindowingHighlight] = orangeActive;
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.50f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.70f);



	ImGui_ImplWin32_Init(Overlay.Hwnd);
	ImGui_ImplDX9_Init(DirectX9.pDevice);
	DirectX9.IDirect3D9->Release();
	return true;
}

auto setupoverlay() -> bool {

	if (!createwindow())
	{
		cout << "<!> Overlay Failed" << endl;
		return false;
	}

	if (!dx9())
	{
		cout << "<!> DirectX Failed" << endl;
		return false;
	}

	return true;
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message) {
	case WM_DESTROY:
		if (DirectX9.pDevice != NULL) {
			DirectX9.pDevice->EndScene();
			DirectX9.pDevice->Release();
		}
		if (DirectX9.IDirect3D9 != NULL) {
			DirectX9.IDirect3D9->Release();
		}
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (DirectX9.pDevice != NULL && wParam != SIZE_MINIMIZED) {
			ImGui_ImplDX9_InvalidateDeviceObjects();
			DirectX9.pParameters.BackBufferWidth = LOWORD(lParam);
			DirectX9.pParameters.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = DirectX9.pDevice->Reset(&DirectX9.pParameters);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}