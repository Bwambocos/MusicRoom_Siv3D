#include <Siv3D.hpp>
#include "SceneMgr.h"

void Main()
{
	Window::Resize({ 768,768 }, true);
	Window::SetTitle(L"MusicRoom v2.0");

	SceneMgr_ChangeScene(Scene_Select);
	while (System::Update())
	{
		SceneMgr_Update();
		SceneMgr_Draw();
	}
}