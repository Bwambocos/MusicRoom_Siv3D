#include <Siv3D.hpp>
#include "Room.h"

void Main()
{
	Window::SetTitle(L"MusicRoom v1.5");
	Window::Resize(640, 640);
	
	while (System::Update())
	{
		Room_Update();
		Room_Draw();
	}
}
