#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Bar.h"

void Main()
{
	Window::SetStyle(WindowStyle::NonFrame);
	Window::Resize({ 768,768 + BAR_HEIGHT }, true);
	Window::SetTitle(L"MusicRoom v2.0");

	SceneMgr_ChangeScene(Scene_Select);
	while (System::Update())
	{
		SceneMgr_Update();
		SceneMgr_Draw();
	}
}