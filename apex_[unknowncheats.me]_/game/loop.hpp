#include "../main.h"
#include <chrono>
#include "offsets.hpp"

static int keystatus = 0;
static int keystatus2 = 0;
namespace hotkeys
{
	int aimkey;
	int triggerkey;
}



static const char* keyNames[] =
{
	(""),
	("Left Mouse"),
	("Right Mouse"),
	("Cancel"),
	("Middle Mouse"),
	("Mouse 5"),
	("Mouse 4"),
	(""),
	("Backspace"),
	("Tab"),
	(""),
	(""),
	("Clear"),
	("Enter"),
	(""),
	(""),
	("Shift"),
	("Control"),
	("Alt"),
	("Pause"),
	("Caps"),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	("Escape"),
	(""),
	(""),
	(""),
	(""),
	("Space"),
	("Page Up"),
	("Page Down"),
	("End"),
	("Home"),
	("Left"),
	("Up"),
	("Right"),
	("Down"),
	(""),
	(""),
	(""),
	("Print"),
	("Insert"),
	("Delete"),
	(""),
	("0"),
	("1"),
	("2"),
	("3"),
	("4"),
	("5"),
	("6"),
	("7"),
	("8"),
	("9"),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	("A"),
	("B"),
	("C"),
	("D"),
	("E"),
	("F"),
	("G"),
	("H"),
	("I"),
	("J"),
	("K"),
	("L"),
	("M"),
	("N"),
	("O"),
	("P"),
	("Q"),
	("R"),
	("S"),
	("T"),
	("U"),
	("V"),
	("W"),
	("X"),
	("Y"),
	("Z"),
	(""),
	(""),
	(""),
	(""),
	(""),
	("Numpad 0"),
	("Numpad 1"),
	("Numpad 2"),
	("Numpad 3"),
	("Numpad 4"),
	("Numpad 5"),
	("Numpad 6"),
	("Numpad 7"),
	("Numpad 8"),
	("Numpad 9"),
	("Multiply"),
	("Add"),
	(""),
	("Subtract"),
	("Decimal"),
	("Divide"),
	("F1"),
	("F2"),
	("F3"),
	("F4"),
	("F5"),
	("F6"),
	("F7"),
	("F8"),
	("F9"),
	("F10"),
	("F11"),
	("F12")
};
static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}
void HotkeyButton(int aimkey, void* changekey, int status)
{
	const char* preview_value = NULL;
	if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, aimkey, &preview_value);

	std::string aimkeys;
	if (preview_value == NULL)
		aimkeys = ("Select Key");
	else
		aimkeys = preview_value;

	if (status == 1)
	{
		aimkeys = ("Press the key");
	}
	if (ImGui::Button(aimkeys.c_str(), ImVec2(125, 25)))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
}

void Hotkeytrigger(int triggerkey, void* changekey, int status)
{
	const char* preview_value = NULL;
	if (triggerkey >= 0 && triggerkey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, triggerkey, &preview_value);

	std::string triggerkeys;
	if (preview_value == NULL)
		triggerkeys = ("Select Key");
	else
		triggerkeys = preview_value;

	if (status == 1)
	{
		triggerkeys = ("Press the key");
	}
	if (ImGui::Button(triggerkeys.c_str(), ImVec2(125, 25)))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
}

namespace settings
{
	bool menu = true;
}

auto InputHandler() -> void {
	for (int i = 0; i < 5; i++) {
		ImGui::GetIO().MouseDown[i] = false;
	}

	int Button = -1;
	if (GetAsyncKeyState(VK_LBUTTON)) {
		Button = 0;
	}

	if (Button != -1) {
		ImGui::GetIO().MouseDown[Button] = true;
	}
}

namespace settings
{
	bool aimbot = false;
	bool showfov = false;
	bool vischeck = true;
	bool skeleton = false;
	int fov = 0;
	float smoothing = 1.f;
	bool box = false;
	bool name = false;
	bool headdot = false;
	bool health = false;
	bool sheild = false;
	bool streamproof = true;
	int key = 0x02;
	
	bool engine = false;
	bool glow = false;
	bool skinchanger = false;
	int skinid;

	bool superglide = false;

	bool playerskinchanger = false;
	int playerskinid;

	bool modelchanger = false;
	int modelid;

	float rgb[3] = { 255, 255, 255 };

	enum AimPart
	{
		HEAD,
		PELVIS
	};

	AimPart aimpart = HEAD;
}

void ChangeKey(void* blank)
{
	keystatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				settings::key = i;
				keystatus = 0;
				return;
			}
		}
	}
}

extern void (*move_mouse_ptr)(int, int);

void aimbot(int x, int y)
{


	float centerX = Width / 2.0f;
	float centerY = Height / 2.0f;

	float dx = x - centerX;
	float dy = y - centerY;

	const float deadzone = 0.00f;

	if (fabsf(dx) < deadzone && fabsf(dy) < deadzone)
		return;

	dx /= settings::smoothing;
	dy /= settings::smoothing;

	move_mouse_ptr(dx, dy);
}


void drawmenu()
{
	// Static tab index for tracking active page
	static int activeTab = 0;

	// Sidebar dimensions
	ImVec2 sidebar_size = ImVec2(120, 0); // width of sidebar
	ImVec2 content_size = ImVec2(0, 0);   // fill rest of window

	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	if (GetAsyncKeyState(VK_INSERT) & 1) settings::menu = !settings::menu;
	ImGui::Begin("eax (by cen)", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	{

		// make a side bar that has butttons aimbot esp 
		ImGui::Columns(2, nullptr, true);

		ImGui::SetColumnWidth(0, 75);
		{
			ImGui::SetCursorPosX((75 - 50) * 0.5f);
			ImGui::Image((void*)sillycar, ImVec2(50, 50));
			ImGui::Dummy(ImVec2(0, 10));
			if (ImGui::Button("Aimbot", ImVec2(-1, 30))) activeTab = 0;
			if (ImGui::Button("ESP", ImVec2(-1, 30))) activeTab = 1;
			if (ImGui::Button("Engine", ImVec2(-1, 30))) activeTab = 2;
		}
		ImGui::NextColumn();

		// Main content column
		{
			if (activeTab == 0)
			{
				ImGui::Text("eax");
				ImGui::Checkbox("Enabled", &settings::aimbot);
				ImGui::Checkbox("Visible Only", &settings::vischeck);
				ImGui::Checkbox("Show FOV", &settings::showfov);
				ImGui::SliderInt("FOV", &settings::fov, 0, 500);
				ImGui::SliderFloat("Smoothing", &settings::smoothing, 1.0f, 50.f);

				const char* parts[] = { "Head", "Pelvis" };
				ImGui::Combo("Aim Part", (int*)&settings::aimpart, parts, IM_ARRAYSIZE(parts));
				HotkeyButton(settings::key, ChangeKey, keystatus);
			}
			else if (activeTab == 1)
			{
				ImGui::Text("eax");
				ImGui::Checkbox("Box", &settings::box);
				ImGui::Checkbox("Name", &settings::name);
				ImGui::Checkbox("Head Dot", &settings::headdot);
				ImGui::Checkbox("Health Bar", &settings::health);
				ImGui::Checkbox("Shield Bar", &settings::sheild);
				ImGui::Checkbox("Streamproof", &settings::streamproof);

				if (ImGui::Button("Exit"))
					exit(0);
			}

			else if (activeTab == 2)
			{
				ImGui::Text("eax");
				ImGui::Checkbox("Enable Engine", &settings::engine);
				if (settings::engine)
				{
					ImGui::Text("WARNING THIS IS DTC");
					ImGui::Checkbox("Glow (does not work rn)", &settings::glow);
					ImGui::Checkbox("Skin changer", &settings::skinchanger);
					if (settings::skinchanger)
					{
						ImGui::SliderInt("Skin ID", &settings::skinid, 0, 25);
					}

					ImGui::Checkbox("Super Glide", &settings::superglide);

					/* 
					ImGui::Checkbox("Model changer", &settings::modelchanger);
					if (settings::modelchanger)
					{
						
						ImGui::SliderInt("Model ID", &settings::modelid, 0, 25);
					}
					*/
				}
			}
		}

		ImGui::Columns(1); // Reset columns after use
	}
	ImGui::End();

}

inline std::vector<c_player> player_list;

void loop()
{
	if (settings::showfov) ImGui::GetForegroundDrawList()->AddCircle(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), settings::fov, ImColor(255, 0, 0), 100, 1.0f);

	SetWindowDisplayAffinity(Overlay.Hwnd, settings::streamproof ? WDA_EXCLUDEFROMCAPTURE : WDA_NONE);

	c_player local_player(read<DWORD_PTR>(Base + OFF_LOCAL_PLAYER));
	if (!local_player.valid()) {
		return;
	}
	int best_target_index = 0;

	if (settings::engine)
	{
		if (settings::skinchanger)
		{
			uint64_t localplayerr = read<uint64_t>(Base + OFF_LOCAL_PLAYER);
			DWORD64 lasweapon = read<DWORD64>(localplayerr + 0x1994) & 0xFFFF;
			DWORD64 Weapon = read<DWORD64>(Base + 0x619aaa8 + (lasweapon << 5));
			write<int>(Weapon + 0xd40, settings::skinid);


		}

		if (settings::modelchanger)
		{
			// does not work rn have fun w this
			uint64_t localplayerr = read<uint64_t>(Base + OFF_LOCAL_PLAYER);
			auto vmhandle = read<uint64_t>(localplayerr + OFF_VIEWMODEL);
			auto view_model_ptr = read<uint64_t>(Base + OFF_ENTITY_LIST + (vmhandle << 5));

			char modelName[200] = { 0 };
			auto name_ptr = read<uint64_t>(view_model_ptr + OFF_MODELNAME);
			SofMain::ReadPhysicalMemory((PVOID)name_ptr, modelName, 200);
			std::string model_name_str = std::string(modelName);

			//int cur_sequence = read<int>(view_model_ptr + m_currentFrameBaseAnimating_animSequence);
			//
			
		}
	}

	float closest_distance = FLT_MAX;
	c_player closest_player;
	for (auto x = 0; x < player_list.size(); ++x) {
		c_player& player(player_list[x]);
		if (player.i_team() == local_player.i_team()) continue;

		auto distance = player.distance() / 100.0f;
		auto name = player.s_name();
		auto health = player.i_health();
		auto shield = player.i_shield();


		auto head = player.bone_position(8);
		auto feet = player.bone_position(0);
		auto origin = player.vec_origin();
		origin = world_to_screen(origin);
		head = world_to_screen(head);
		feet = world_to_screen(feet);

		float box_height = abs(head.y - origin.y);
		float box_width = box_height * 0.50f;
		float box_x = head.x - (box_width / 2);
		float box_y = head.y;

		if (settings::headdot)
		{
			draw_filled_circle(head.x, head.y, ImColor(255, 255, 255), 1.5f);
		}

		if (settings::box)
		{
			draw_cornered_box(head.x - (box_width / 2), head.y, box_width, box_height, ImColor(255, 255, 0), 1.f);
		}

		if (settings::health)
		{
			
			int delta = health * box_height / 100;

			draw_rectangle_filled(box_x - 4, box_y + (box_height - delta) + 1, 2, delta - 1, IM_COL32(20, 20, 20, 255));
			draw_rectangle_filled(box_x - 4, box_y + (box_height - delta) + 1, 2, delta - 1, IM_COL32(0, 255, 0, 255));
		}

		if (settings::sheild)
		{
			int delta = shield * box_height / 100;

			float shield_bar_x = settings::health ? box_x - 6 : box_x - 2;

			draw_rectangle_filled(shield_bar_x, box_y + (box_height - delta) + 1, 2, delta - 1, IM_COL32(20, 20, 20, 255));
			draw_rectangle_filled(shield_bar_x, box_y + (box_height - delta) + 1, 2, delta - 1, IM_COL32(40, 40, 255, 255));
		}

		if (settings::name) {
			ImVec2 text_size = ImGui::CalcTextSize(name.c_str());
			float text_x = box_x + (box_width / 2) - (text_size.x / 2);
			float text_y = box_y - text_size.y - 2;

			draw_text(text_x, text_y, ImColor(255, 255, 255), name.c_str());
		}

		if (settings::engine)
		{
			if (settings::glow)
			{
				int glow = 83;
				if (player.visible(x))
				{
					glow = 76;
				}
				else
				{
					glow = 83;
				}

				write<int>(player.get_address() + 0x28C, 1);
				write<int>(player.get_address() + 0x29c, glow);
				write<int>(player.get_address() + 0x26c, 2);
			}
		}

		double dx = head.x - Width / 2;
		double dy = head.y - Height / 2;
		float dist = sqrtf(static_cast<float>(dx * dx + dy * dy));

		if (dist <= settings::fov && dist < closest_distance && dist != 100.f)
		{
			closest_distance = dist;
			closest_player = player;
			best_target_index = x;
		}
	}

	if (settings::aimbot && !settings::menu)
	{
		if (closest_distance <= settings::fov && closest_player.valid())
		{
			c_vec3 target_pos;

			if (settings::aimpart == settings::HEAD)
			{
				auto head = closest_player.bone_position(8);
				head = world_to_screen(head);
				target_pos = head;
			}
			else if (settings::aimpart == settings::PELVIS)
			{
				auto pelvis = closest_player.bone_position(3);
				pelvis = world_to_screen(pelvis);
				target_pos = pelvis;
			}

			if (GetAsyncKeyState(settings::key) & 0x8000) // Key is held
			{
				if (settings::vischeck && !closest_player.visible(best_target_index))
					return;

				aimbot(target_pos.x, target_pos.y);
			}
		}
	}
}



void setup()
{
	while (true)
	{
		std::vector<c_player> players;
		c_player local_player(read<DWORD_PTR>(Base + OFF_LOCAL_PLAYER));
		if (local_player.valid()) {
			for (auto x = 0; x < 100 /*g_pglobalvars.maxClients + 1*/; ++x) {
				auto entity = read<DWORD_PTR>(Base + OFF_ENTITY_LIST + (x * 0x20));
				if (!entity) continue;
				c_player player(entity);

				if (player != local_player && player.alive()) {
					auto c_signifier = class_signifier(entity);
					if (c_signifier != ("player")) // block all apart from players
						continue;
				}

				players.push_back(player);
			}
		}

		player_list = players;
		players.clear();
		Sleep(10);
	}
}

void render() {

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	loop();

	ImGui::GetIO().MouseDrawCursor = settings::menu;
	if (GetAsyncKeyState(VK_INSERT) & 1) settings::menu = !settings::menu;
	if (settings::menu == true)
	{
		InputHandler();
		drawmenu();
		SetWindowLong(Overlay.Hwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
		UpdateWindow(Overlay.Hwnd);
		SetFocus(Overlay.Hwnd);
	}
	else {
		SetWindowLong(Overlay.Hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
		UpdateWindow(Overlay.Hwnd);
	}
	ImGui::EndFrame();

	DirectX9.pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (DirectX9.pDevice->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		DirectX9.pDevice->EndScene();
	}

	HRESULT result = DirectX9.pDevice->Present(NULL, NULL, NULL, NULL);
	if (result == D3DERR_DEVICELOST && DirectX9.pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		ImGui_ImplDX9_InvalidateDeviceObjects();
		DirectX9.pDevice->Reset(&DirectX9.pParameters);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}


void render_loop()
{
	static RECT OldRect = { 0 };
	static HWND LastTempHwnd = nullptr;
	static DWORD lastSizeCheck = GetTickCount();
	static const int sizeCheckIntervalMs = 1000;
	static const int targetFPS = 144;
	static const DWORD frameDelay = 1000 / targetFPS;

	ZeroMemory(&DirectX9.Message, sizeof(MSG));

	while (DirectX9.Message.message != WM_QUIT) {
		DWORD frameStart = GetTickCount();

		if (PeekMessage(&DirectX9.Message, Overlay.Hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&DirectX9.Message);
			DispatchMessage(&DirectX9.Message);
		}

		SetWindowPos(
			Overlay.Hwnd,
			HWND_TOPMOST,
			0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW
		);

		DWORD now = GetTickCount();
		if (now - lastSizeCheck > sizeCheckIntervalMs) {
			lastSizeCheck = now;

			RECT TempRect = { 0 };
			POINT TempPoint = { 0 };
			GetClientRect(Process.Hwnd, &TempRect);
			ClientToScreen(Process.Hwnd, &TempPoint);

			TempRect.left = TempPoint.x;
			TempRect.top = TempPoint.y;

			if (memcmp(&TempRect, &OldRect, sizeof(RECT)) != 0) {
				OldRect = TempRect;
				Width = TempRect.right;
				Height = TempRect.bottom;

				DirectX9.pParameters.BackBufferWidth = Width;
				DirectX9.pParameters.BackBufferHeight = Height;

				SetWindowPos(
					Overlay.Hwnd,
					HWND_TOPMOST,
					TempPoint.x, TempPoint.y,
					Width, Height,
					SWP_NOACTIVATE | SWP_SHOWWINDOW
				);

				if (DirectX9.pDevice) {
					HRESULT resetResult = DirectX9.pDevice->Reset(&DirectX9.pParameters);
					if (FAILED(resetResult)) {
					}
				}
			}
		}

		render();
		Sleep(0);
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (DirectX9.pDevice) {
		DirectX9.pDevice->EndScene();
		DirectX9.pDevice->Release();
		DirectX9.pDevice = nullptr;
	}

	if (DirectX9.IDirect3D9) {
		DirectX9.IDirect3D9->Release();
		DirectX9.IDirect3D9 = nullptr;
	}

	DestroyWindow(Overlay.Hwnd);
	UnregisterClass(Overlay.WindowClass.lpszClassName, Overlay.WindowClass.hInstance);
}