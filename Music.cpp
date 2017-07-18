// include
#include <Siv3D.hpp>
#include "Music.h"
#include "Bar.h"
#include "Detail.h"

// グローバル変数
static Texture music_albumImg;
static String music_albumName = L"", music_musicName = L"", music_musicExp = L"";

// 曲 初期化
void Music_Init()
{
	setAlbumMusicName(music_albumName, music_musicName, music_albumImg);
	TextReader music_reader(L"music\\" + music_albumName + L"\\" + music_musicName + L"\\" + music_musicName + L".txt");
	music_reader.readAll(music_musicExp);
}

// 曲 更新
void Music_Update()
{

}

// 曲 描画
void Music_Draw()
{
	Rect(300, 300)(music_albumImg).drawAt(Window::Center().x, Window::Center().y + BAR_HEIGHT);
}