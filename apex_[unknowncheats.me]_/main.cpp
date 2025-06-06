#include "main.h"
#include <thread>
#include "interception.h"

InterceptionContext mouse_context;
InterceptionDevice mouse_device;
InterceptionStroke mouse_stroke;


void run_mouse_loop() {
	while (interception_receive(mouse_context, mouse_device = interception_wait(mouse_context), &mouse_stroke, 1) > 0) {
		if (interception_is_mouse(mouse_device)) {
			InterceptionMouseStroke& mstroke = *(InterceptionMouseStroke*)&mouse_stroke;
			interception_send(mouse_context, mouse_device, &mouse_stroke, 1);
		}
	}
}

void initialize_mouse() {
	mouse_context = interception_create_context();
	interception_set_filter(mouse_context, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_MOVE);
	mouse_device = interception_wait(mouse_context);

	while (interception_receive(mouse_context, mouse_device = interception_wait(mouse_context), &mouse_stroke, 1) > 0) {
		if (interception_is_mouse(mouse_device)) {
			InterceptionMouseStroke& mstroke = *(InterceptionMouseStroke*)&mouse_stroke;
			interception_send(mouse_context, mouse_device, &mouse_stroke, 1);
			break;
		}
	}
	std::thread([&]() { run_mouse_loop(); }).detach();
}
void move_mouse(int x, int y) {
	InterceptionMouseStroke& mstroke = *(InterceptionMouseStroke*)&mouse_stroke;
	mstroke.flags = 0;
	mstroke.information = 0;
	mstroke.x = x;
	mstroke.y = y;
	interception_send(mouse_context, mouse_device, &mouse_stroke, 1);
}
void (*move_mouse_ptr)(int, int) = &move_mouse;

void mouse_click() {
	InterceptionMouseStroke& mstroke = *(InterceptionMouseStroke*)&mouse_stroke;
	mstroke.state = INTERCEPTION_MOUSE_LEFT_BUTTON_DOWN;
	interception_send(mouse_context, mouse_device, &mouse_stroke, 1);
	mstroke.state = INTERCEPTION_MOUSE_LEFT_BUTTON_UP;
	interception_send(mouse_context, mouse_device, &mouse_stroke, 1);
}


void check()
{
	while (TRUE)
	{
		SofMain::ProcessID = SofMain::FindProcess("r5apex_dx12.exe");
		if (!SofMain::ProcessID)
		{
			exit(1);
		}
		std::this_thread::sleep_for(std::chrono::seconds(15));
	}
}

auto init() -> bool
{
	if (!SofMain::Init())
	{
		cout << "<!> failed to find driver" << endl;
		return false;
	}

	cout << "press f1 in lobby" << endl;	

	while (true)
	{
		if (GetAsyncKeyState(VK_F1))
		{
			break;
		}

	}

	SofMain::ProcessID = SofMain::FindProcess("r5apex_dx12.exe");
	if (!SofMain::ProcessID)
	{
		cout << "<!> failed to find process" << endl;
		return false;
	}


	auto cr3 = SofMain::CR3();
	if (!cr3)
	{
		cout << "<!> Failed to resolve cr3" << endl;
		return false;
	}

	Base = SofMain::GetBase();
	if (!Base)
	{
		cout << "<!> Failed to get base address" << endl;
		return false;
	}

	if (!input::init())
	{
		cout << "<!> Failed to Initialize input" << endl;
	}

	initialize_mouse();

	bool WindowFocus = false;
	while (WindowFocus == false) {
		DWORD ForegroundWindowProcessID;
		GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
		if (SofMain::FindProcess("r5apex_dx12.exe") == ForegroundWindowProcessID) {
			Process.ID = GetCurrentProcessId();
			Process.Handle = GetCurrentProcess();
			Process.Hwnd = GetForegroundWindow();

			RECT TempRect;
			GetWindowRect(Process.Hwnd, &TempRect);
			Width = TempRect.right - TempRect.left;
			Height = TempRect.bottom - TempRect.top;
			Process.WindowLeft = TempRect.left;
			Process.WindowRight = TempRect.right;
			Process.WindowTop = TempRect.top;
			Process.WindowBottom = TempRect.bottom;

			char TempTitle[MAX_PATH];
			GetWindowText(Process.Hwnd, TempTitle, sizeof(TempTitle));
			Process.Title = TempTitle;

			char TempClassName[MAX_PATH];
			GetClassName(Process.Hwnd, TempClassName, sizeof(TempClassName));
			Process.ClassName = TempClassName;

			char TempPath[MAX_PATH];
			GetModuleFileNameEx(Process.Handle, NULL, TempPath, sizeof(TempPath));
			Process.Path = TempPath;

			WindowFocus = true;
		}
	}

	setupoverlay();

	std::thread(setup).detach();
	std::thread(check).detach();

	FreeConsole();
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);

	while (TRUE)
	{
		render_loop();
	}

}

auto main() -> void
{

	cout << "letslpay's apex cheat" << endl;

	if (!init())
	{
		cout << "<!> failed to intialize" << endl;
		Sleep(3000);
		exit(0);
	}
}