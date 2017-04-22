#include <Siv3D.hpp>
#include "Room.h"

void Main()
{
	Window::SetTitle(L"MusicRoom v1.5");
	Window::Resize(640, 640);
	Profiler::EnableWarning(false);

	Room_Init();
	while (System::Update())
	{
		Room_Update();
		Room_Draw();
	}
}
