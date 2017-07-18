// include
#include <Siv3D.hpp>
#include "Music.h"
#include "Bar.h"
#include "Detail.h"

// グローバル変数
static Texture music_albumImg, music_Gaussian, music_Main;
static String music_albumName = L"", music_musicName = L"", music_musicExp = L"";
static Sound music_Music;
static FFTResult fft;

// 曲 初期化
void Music_Init()
{
	setAlbumMusicName(music_albumName, music_musicName, music_albumImg, music_Music);
	TextReader music_reader(L"music\\" + music_albumName + L"\\" + music_musicName + L"\\" + music_musicName + L".txt");
	music_reader.readAll(music_musicExp);
	music_Gaussian = Texture(Image(L"music\\" + music_albumName + L"\\" + music_albumName + L".png").gaussianBlurred(10, 10));
	music_Music.play();
}

// 曲 更新
void Music_Update()
{

}

// 曲 描画
void Music_Draw()
{
	Rect(512, 512)(music_albumImg).drawAt(Window::Center().x, 256 + BAR_HEIGHT);
	Rect(512, 512)(music_Gaussian).drawAt(Window::Center().x, 256 + BAR_HEIGHT);
	fft = FFT::Analyze(music_Music);
	Rect(0, BAR_HEIGHT, 768, 128).draw(Color(64, 64, 64));
	for (auto i : step(320)) { RectF(i * 10, 128 + BAR_HEIGHT, 10, -Pow(fft.buffer[i], 0.6) * 500).draw(HSV(240 - i * 10)); }
}