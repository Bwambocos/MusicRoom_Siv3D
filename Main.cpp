#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Bar.h"

void Main()
{
	Window::SetStyle(WindowStyle::NonFrame);
	Window::Resize({ 768,512 + BAR_HEIGHT }, true);
	Window::SetTitle(L"MusicRoom v2.0");

	Bar_Init();
	SceneMgr_ChangeScene(Scene_Select);
	while (System::Update())
	{
		Bar_Update();
		SceneMgr_Update();
		Bar_Draw();
		SceneMgr_Draw();
	}
}