// include
#include <Siv3D.hpp>
#include "Main.h"
#include "SceneMgr.h"
#include "Bar.h"
#include "Fav.h"

void Main()
{
	Window::Resize({ WINDOW_WIDTH,WINDOW_HEIGHT + BAR_HEIGHT }, true);
	Window::SetTitle(L"MusicRoom v2.2.5");

	Bar_Init(); Fav_Init();
	SceneMgr_ChangeScene(Scene_Select);
	while (System::Update())
	{
		SceneMgr_Update();
		Bar_Update();
		SceneMgr_Draw();
		Bar_Draw();
	}
	saveFavList();
}