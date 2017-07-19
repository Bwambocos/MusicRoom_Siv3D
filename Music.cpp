// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Music.h"
#include "Bar.h"
#include "Detail.h"

// ÉOÉçÅ[ÉoÉãïœêî
static Texture music_albumImg, music_Gaussian, music_Main;
static String music_albumName = L"", music_musicName = L"", music_musicExp = L"";
static Sound music_Music;
static RoundRect rect_musicName(25, 25 + BAR_HEIGHT, 468, 48, 10);
static RoundRect rect_musicTime(496, 25 + BAR_HEIGHT, 199, 48, 10);
static RoundRect rect_music_isFav(698, 25 + BAR_HEIGHT, 48, 48, 10);
static RoundRect rect_musicExp(25, 130 + BAR_HEIGHT, 718, 357, 10);
static FFTResult fft;

// ã» èâä˙âª
void Music_Init()
{
	// îwåi èâä˙âª
	{
		music_Main = Texture(L"data\\Music\\main.png");
		music_Gaussian = Texture(Image(L"music\\" + music_albumName + L"\\" + music_albumName + L".png").gaussianBlurred(10, 10));
	}

	// ã»èÓïÒ èâä˙âª
	{
		setAlbumMusicName(music_albumName, music_musicName, music_albumImg, music_Music);
		TextReader music_reader(L"music\\" + music_albumName + L"\\" + music_musicName + L"\\" + music_musicName + L".txt");
		music_reader.readAll(music_musicExp);
	}

	music_Music.play();
}

// ã» çXêV
void Music_Update()
{
	if (Input::KeyB.clicked) { SceneMgr_ChangeScene(Scene_Detail); }
}

// ã» ï`âÊ
void Music_Draw()
{
	// îwåi ï`âÊ
	{
		music_Main.draw(0, BAR_HEIGHT);
		fft = FFT::Analyze(music_Music);
		for (auto i : step(51)) { RectF(1 + i * 15, Window::Height(), 15, -Pow(fft.buffer[i], 0.8) * 750).draw(HSV(i * 7.0315)); }
		// Rect(500, 500)(music_albumImg).drawAt(Window::Center().x, BAR_HEIGHT + 256);
		// Rect(500, 500)(music_Gaussian).drawAt(Window::Center().x, BAR_HEIGHT + 256);
		rect_musicName.drawShadow({ 0,15 }, 32, 10);
		rect_musicName.drawFrame(3);
		rect_musicName.draw(Color(32, 32, 32, 120));
		rect_musicTime.drawShadow({ 0,15 }, 32, 10);
		rect_musicTime.drawFrame(3);
		rect_musicTime.draw(Color(32, 32, 32, 120));
		rect_music_isFav.drawShadow({ 0,15 }, 32, 10);
		rect_music_isFav.drawFrame(3);
		rect_music_isFav.draw(Color(32, 32, 32, 120));
		rect_musicExp.drawShadow({ 0,15 }, 32, 10);
		rect_musicExp.drawFrame(3);
		rect_musicExp.draw(Color(32, 32, 32, 120));
	}

	// ã»èÓïÒ ï`âÊ
	{

	}
}