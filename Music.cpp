// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Music.h"
#include "Bar.h"
#include "Detail.h"
#include "Fav.h"

// グローバル変数
static Texture music_albumImg, music_Gaussian, music_Main, faved, not_faved;
static Texture originPlay[2], originBrief[2], originStop[2], originSeek[2], originRep[2];
static Texture displayPlay, displayBrief, displayStop, displaySeek, displayRep;
static String music_albumName = L"", music_musicName = L"", music_musicExp = L"", music_musicLength = L"";
static Sound music_Music;
static Font music_NameTime, music_Exp;
static RoundRect rect_musicName(25, 25 + BAR_HEIGHT, 468, 48, 10);
static RoundRect rect_musicTime(496, 25 + BAR_HEIGHT, 199, 48, 10);
static RoundRect rect_music_isFav(698, 25 + BAR_HEIGHT, 48, 48, 10);
static RoundRect rect_musicBar(127, 91 + BAR_HEIGHT, 565, 21, 5);
static RoundRect rect_musicExp(25, 130 + BAR_HEIGHT, 718, 357, 10);
static FFTResult fft;
static int32_t music_musicTime;

// 曲 初期化
void Music_Init()
{
	// 背景 初期化
	{
		music_Main = Texture(L"data\\Music\\main.png");
		music_Gaussian = Texture(Image(L"music\\" + music_albumName + L"\\" + music_albumName + L".png").gaussianBlurred(10, 10));
		originPlay[0] = Texture(L"data\\Music\\play\\normal.png");
		originPlay[1] = Texture(L"data\\Music\\play\\active.png");
		originBrief[0] = Texture(L"data\\Music\\brief\\normal.png");
		originBrief[1] = Texture(L"data\\Music\\brief\\active.png");
		originStop[0] = Texture(L"data\\Music\\stop\\normal.png");
		originStop[1] = Texture(L"data\\Music\\stop\\active.png");
		originSeek[0] = Texture(L"data\\Music\\seek\\normal.png");
		originSeek[1] = Texture(L"data\\Music\\seek\\active.png");
		originRep[0] = Texture(L"data\\Music\\rep\\normal.png");
		originRep[1] = Texture(L"data\\Music\\rep\\active.png");
		displayPlay = originPlay[0];
		displayBrief = originBrief[0];
		displayStop = originStop[0];
		displaySeek = originSeek[0];
		displayRep = originRep[0];
	}

	// 曲情報 初期化
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

// 曲 更新
void Music_Update()
{
	if (Input::KeyB.clicked) { SceneMgr_ChangeScene(Scene_Detail); }

	// 再生バー 更新
	{
		// バー
		if (rect_musicBar.leftPressed)
		{
			const Point tmpPoint = Mouse::Pos();
			music_Music.setPosSample(music_Music.lengthSample()*(tmpPoint.x - rect_musicBar.x) / rect_musicBar.w);
		}

		// ボタン
		Circle tmpCircle(45, rect_musicBar.y + rect_musicBar.h / 2, 15);
		displayPlay = originPlay[(tmpCircle.mouseOver ? 1 : 0)];
		displayBrief = originBrief[(tmpCircle.mouseOver ? 1 : 0)];
		if (tmpCircle.leftClicked)
		{
			(music_Music.isPlaying() ? music_Music.pause() : music_Music.play());
		}
		tmpCircle = Circle(90, rect_musicBar.y + rect_musicBar.h / 2, 15);
		displayRep = originRep[((tmpCircle.mouseOver || music_Music.isLoop()) ? 1 : 0)];
		if (tmpCircle.leftClicked)
		{
			const int64 tmpTime = music_Music.streamPosSample();
			music_Music.pause();
			music_Music.setLoop(music_Music.isLoop() ? false : true);
			music_Music.play();
			music_Music.setPosSample(tmpTime);
		}
		tmpCircle = Circle(723, rect_musicBar.y + rect_musicBar.h / 2, 15);
		displayStop = originStop[(tmpCircle.mouseOver ? 1 : 0)];
		if (tmpCircle.leftClicked) { music_Music.stop(); }
	}

	// 曲情報 更新
	{
		music_musicLength = Format(Pad(music_musicTime / 60, { 2,L'0' }), L":", Pad(music_musicTime % 60, { 2,L'0' }));
		if (rect_music_isFav.leftClicked)
		{
			(isFav(music_albumName, music_musicName) ? removeFav : addFav)(music_albumName, music_musicName);
		}
	}
}

// 曲 描画
void Music_Draw()
{
	// 背景 描画
	{
		music_Main.draw(0, BAR_HEIGHT);
		fft = FFT::Analyze(music_Music);
		for (auto i : step(51)) { RectF(1 + i * 15, Window::Height(), 15, -Pow(fft.buffer[i], 0.8) * 750).draw(Color(200, 200, 200)); }
		rect_musicName.drawShadow({ 0,15 }, 32, 10);
		rect_musicName.drawFrame(3);
		rect_musicName.draw(Color(32, 32, 32, 120));
		rect_musicTime.drawShadow({ 0,15 }, 32, 10);
		rect_musicTime.drawFrame(3);
		rect_musicTime.draw(Color(32, 32, 32, 120));
		rect_music_isFav.drawShadow({ 0,15 }, 32, 10);
		rect_music_isFav.drawFrame(3);
		rect_music_isFav.draw(Color(32, 32, 32, 120));

		// 再生バー
		{
			// バー
			rect_musicBar.drawShadow({ 0,15 }, 32, 10);
			rect_musicBar.drawFrame(3);
			rect_musicBar.draw(Color(32, 32, 32, 120));
			const RoundRect tmpRect(rect_musicBar.x, rect_musicBar.y, rect_musicBar.w*music_Music.streamPosSample() / music_Music.lengthSample(), rect_musicBar.h, rect_musicBar.r);
			tmpRect.draw(Color(0, 200, 0, 120));

			// Seek
			auto x = rect_musicBar.x + rect_musicBar.w*music_Music.streamPosSample() / music_Music.lengthSample();
			(music_Music.isPlaying() ? originSeek[1] : originSeek[0]).drawAt(x, rect_musicBar.y + rect_musicBar.h / 2);

			// ボタン
			(music_Music.isPlaying() ? displayBrief : displayPlay).drawAt(45, rect_musicBar.y + rect_musicBar.h / 2);
			displayRep.drawAt(90, rect_musicBar.y + rect_musicBar.h / 2);
			displayStop.drawAt(723, rect_musicBar.y + rect_musicBar.h / 2);
		}

		rect_musicExp.drawShadow({ 0,15 }, 32, 10);
		rect_musicExp.drawFrame(3);
		rect_musicExp.draw(Color(32, 32, 32, 120));
	}

	// 曲情報 描画
	{
		music_NameTime(music_musicName).draw(33, 31 + BAR_HEIGHT);
		music_NameTime(music_musicLength).draw(504, 31 + BAR_HEIGHT);
		((isFav(music_albumName, music_musicName) || rect_music_isFav.mouseOver) ? faved : not_faved).drawAt(722, 49 + BAR_HEIGHT);
		musicExpl_Draw();
	}
}

// 曲説明 描画
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