#include <imgui_internal.h>
#include <windef.h>
#include <WinUser.h>
#include <process.hpp>
#include <iostream>

#define u64 uint64_t

bool openFlag = false;
bool line = false;
bool box = false;
ImColor draw_color = ImColor(1.00f, 0.55f, 0.60f);
static RECT WindowRectangle;
auto WindowX = 0;
auto WindowY = 0;
DWORD pid = GetProcessId("Plants vs. Zombies");
auto hwnd = FindWindowA(NULL, "Plants vs. Zombies");
HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
void DrawLine(DWORD x, DWORD y) {
	ImGui::GetBackgroundDrawList()->AddLine({
		(float)WindowX,(float)WindowY
		}
		, {
			(float)WindowX + x - 100 ,(float)WindowY + y
		}
		, draw_color);
}
void DrawBox(float zx, float zy) {
	ImGui::GetBackgroundDrawList()->AddRect({
		WindowX + zx - 120,WindowY + zy - 10
		}
		, {
			WindowX + zx - 70, WindowY + zy + 90
		}
		, draw_color);
}
void GetWindowInfo() {
	GetWindowRect(hwnd, &WindowRectangle);
	WindowX = (WindowRectangle.left);
	WindowY = (WindowRectangle.top) + 23;
}
DWORD GetZombieCount() {
	//DWORD count = read_mem<DWORD>(handle, FindPointer(handle, 0x006A9EC0, {
	//		0x768
	//	}
	//) + 0x94);
	uint64_t p = read_mem<uint64_t>(handle, 0x006A9EC0);
	p = read_mem<uint64_t>(handle, p + 0x768);
	DWORD count = read_mem<uint64_t>(handle, p + 0x94);
	return count;
}
struct postion {
	DWORD x;
	DWORD y;
	int row;
	int hp;
}
;
std::vector<postion> GetZomibiePos() {
	std::vector<postion> result = {
	}
	;
	auto count = GetZombieCount();
	if (count <= 0)return {};
	for (size_t i = 0; i < GetZombieCount(); i++) {
		uint64_t pointer = FindPointer(handle, 0x006A9EC0, { 0x768,0x90 });
		pointer = pointer + 0x15c * i;
		DWORD x, y;
		int hp, row;
		x = read_mem<DWORD>(handle, pointer + 0x8);
		y = read_mem<DWORD>(handle, pointer + 0xc);
		hp = read_mem<int>(handle, pointer + 0xc8);
		row = read_mem<int>(handle, pointer + 0x1c);
		result.push_back({
			x,y,row,hp
			});
	}
	return result;
}
void Draw() {
	if (openFlag) {
		GetWindowInfo();
		auto pos = GetZomibiePos();
		for (size_t i = 0; i < pos.size(); i++)
		{
			if (line) {
				if (pos[i].hp >= 1 && pos[i].y >= 1 && pos[i].x >= 1 && pos[i].x + WindowRectangle.left <= WindowRectangle.right + 100 && pos[i].row <= 5) {
					int window_width = WindowRectangle.right - WindowRectangle.left;
					long sub = (window_width - pos[i].x);
					float offset = 0.2f * sub;
					if (offset > 0.0f) {
						long raw_x = pos[i].x + offset;
						long raw_y = pos[i].y + pos[i].row * (-20);
						DrawLine(raw_x, raw_y);
					}
					else {
						DrawLine(pos[i].x, pos[i].y + pos[i].row * (-20));
					}
				}
			}
			if (box) {
				if (pos[i].hp >= 1 && pos[i].y >= 1 && pos[i].x >= 1 && pos[i].x + WindowRectangle.left <= WindowRectangle.right + 100 && pos[i].row <= 5) {
					int window_width = WindowRectangle.right - WindowRectangle.left;
					long sub = (window_width - pos[i].x);
					float offset = 0.2f * sub;
					if (offset > 0.0f) {
						long raw_x = pos[i].x + offset;
						long raw_y = pos[i].y + pos[i].row * (-20);
						DrawBox(raw_x, raw_y);
					}
					else {
						DrawBox(pos[i].x, pos[i].y + pos[i].row * (-20));
					}
				}
			}
		}
	}
}