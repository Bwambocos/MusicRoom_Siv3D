// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Music.h"
#include "Bar.h"
#include "Detail.h"
#include "Fav.h"

// ÉOÉçÅ[ÉoÉãïœêî
static Texture music_albumImg, music_Gaussian, music_Main, faved, not_faved;
static String music_albumName = L"", music_musicName = L"", music_musicExp = L"";
static Sound music_Music;
static Font music_NameTime, music_Exp;
static RoundRect rect_musicName(25, 25 + BAR_HEIGHT, 468, 48, 10);
static RoundRect rect_musicTime(496, 25 + BAR_HEIGHT, 199, 48, 10);
static RoundRect rect_music_isFav(698, 25 + BAR_HEIGHT, 48, 48, 10);
static RoundRect rect_musicExp(25, 130 + BAR_HEIGHT, 718, 357, 10);
static FFTResult fft;
static int32_t music_musicTime;

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
		music_musicExp += L'\n';
		music_musicTime = music_Music.lengthSec();
		faved = Texture(L"data\\Music\\fav.png");
		not_faved = Texture(L"data\\Music\\not_fav.png");
		music_NameTime = Font(18);
		music_Exp = Font(16);
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
		auto str = Format(Pad(music_musicTime / 60, { 2,L'0' }), L":", Pad(music_musicTime % 60, { 2,L'0' }));
		music_NameTime(music_musicName).draw(33, 31 + BAR_HEIGHT);
		music_NameTime(str).draw(504, 31 + BAR_HEIGHT);
		(isFav(music_albumName, music_musicName) ? faved : not_faved).drawAt(722, 49 + BAR_HEIGHT);
		musicExpl_Draw();
	}
}

// ã»ê‡ñæ ï`âÊ
void musicExpl_Draw()
{
	Array<String> texts;
	const int32 w = (int32)rect_musicExp.w - 10;
	const int32 h = (int32)rect_musicExp.h;
	size_t pos = 0;

	while (pos < music_musicExp.length)
	{
		for (int32_t i = 0; i + pos < music_musicExp.length; ++i)
		{
			if (music_Exp(music_musicExp.substr(pos, i)).region().w >= w)
			{
				texts.push_back(music_musicExp.substr(pos, i - 1));
				pos += (i - 1);
				break;
			}
			if (music_musicExp[pos + i] == L'\n')
			{
				texts.push_back(music_musicExp.substr(pos, i + 1));
				pos += (i + 1);
				break;
			}
		}
	}

	for (size_t i = 0; i < texts.size(); ++i)
	{
		const int32 y = static_cast<int32>(rect_musicExp.y + 10 + i * music_Exp.height);
		const bool overflow = (i + 1 < texts.size())
			&& (y + music_Exp.height * 2 + 10 > rect_musicExp.y + rect_musicExp.h);
		if (overflow)
		{
			if (texts[i].length > 2) { texts[i].resize(texts[i].length - (texts[i].length > 2 ? 2 : 1)); }
			if (texts[i][texts[i].length - 1] == L'\n') { texts[i][texts[i].length - 1] = L'\0'; }
			texts[i].append(L"...");
		}
		music_Exp(texts[i]).draw(rect_musicExp.x + 10, y);
		if (overflow) { break; }
	}
}