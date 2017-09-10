// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Music.h"
#include "Bar.h"
#include "Detail.h"
#include "Fav.h"

// define
#define DEFAULT_musicName_X 33
#define DRAW_STAYMSEC 3500
#define DRAW_MUSICNAME_MOVE_X 48

// グローバル変数
static Texture music_Main, faved, not_faved;
static Texture originPlay[2], originBrief[2], originStop[2], originSeek[2], originRep[2];
static Texture displayPlay, displayBrief, displayStop, displaySeek, displayRep;
static String music_albumName = L"", music_musicName = L"", music_musicFileName = L"", music_musicExp = L"", music_musicLength = L"";
static Sound music_Music;
static Font music_NameTime, music_Exp;
static RoundRect rect_musicName(25, 25 + BAR_HEIGHT, 468, 48, 10);
static RoundRect rect_musicTime(496, 25 + BAR_HEIGHT, 199, 48, 10);
static RoundRect rect_music_isFav(698, 25 + BAR_HEIGHT, 48, 48, 10);
static RoundRect rect_musicBar(127, 91 + BAR_HEIGHT, 565, 21, 5);
static RoundRect rect_musicExp(25, 130 + BAR_HEIGHT, 718, 357, 10);
static FFTResult fft;
static int music_musicTime;
static double draw_musicName_x;
static int draw_musicName_startMSec, draw_musicName_stayMSec;
static bool draw_musicName_stayFlag;
static bool favLoop_flag = false, stop_flag = false, still_flag = true, button_flag = false;
static int prev_or_next;

// 曲 初期化
void Music_Init()
{
	// 背景 初期化
	{
		if (!music_Main)
		{
			music_Main = Texture(L"data\\Music\\main.png");
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
	}

	// 曲情報 初期化
	{
		switch (get_prevScene())
		{
		case Scene_Detail:
			setAlbumMusicName(music_albumName, music_musicFileName, music_Music);
			favLoop_flag = false;
			break;

		case Scene_Fav:
			setFavMusicName(music_albumName, music_musicFileName, music_Music);
			break;

		case Scene_Music:
			if (!favLoop_flag) { setAlbumMusicName(prev_or_next, music_albumName, music_musicFileName, music_Music); }
			else { setFavMusicName(prev_or_next, music_albumName, music_musicFileName, music_Music); }
			break;
		}
		TextReader music_reader(L"music\\" + music_albumName + L"\\" + music_musicFileName + L"\\" + music_musicFileName + L".txt");
		music_reader.readLine(music_musicName);
		music_musicExp.clear();
		String temp;
		while (music_reader.readLine(temp)) { music_musicExp += temp + L"\n"; }
		music_musicTime = (int)music_Music.lengthSec();
		faved = Texture(L"data\\Music\\fav.png");
		not_faved = Texture(L"data\\Music\\not_fav.png");
		music_NameTime = Font(18);
		music_Exp = Font(16);
	}

	// 描画位置 初期化
	draw_musicName_startMSec = (int)Time::GetMillisec();
	draw_musicName_stayFlag = true;
	draw_musicName_x = DEFAULT_musicName_X;

	giveMusicData(music_albumName, music_musicName, music_Music);
	music_Music.play();
	if (!still_flag) { SceneMgr_ChangeScene(get_prevScene()); }
}

// 曲 更新
void Music_Update()
{
	if (Input::KeyB.clicked)
	{
		SceneMgr_ChangeScene((favLoop_flag ? Scene_Fav : Scene_Detail));
		still_flag = false;
	}
	if (Input::KeyF5.clicked)
	{
		const Rect temprect(0, BAR_HEIGHT, Window::Width(), Window::Height());
		const Font tempfont(32, Typeface::Bold);
		Bar_Draw();
		music_Main.draw(0, BAR_HEIGHT);
		temprect.draw(Color(64, 64, 64, 100));
		tempfont(L"再読み込み中・・・").drawCenter(Window::Height() / 2);
		System::Update();
		Music_Init();
	}
	if (!music_Music.isPlaying() && !stop_flag
		&& music_Music.samplesPlayed() % music_Music.lengthSample() == 0) { changeMusic(1); }
	
	// 再生バー 更新
	{
		// バー
		if (rect_musicBar.leftPressed)
		{
			button_flag = true;
			music_Music.pause();
			const Point tmpPoint = Mouse::Pos();
			music_Music.setPosSample(music_Music.lengthSample()*(tmpPoint.x - (int)rect_musicBar.x) / (int)rect_musicBar.w);
		}
		else if (button_flag)
		{
			music_Music.play();
			button_flag = false;
		}

		// ボタン
		Circle tmpCircle(45, rect_musicBar.y + rect_musicBar.h / 2, 15);
		displayPlay = originPlay[(tmpCircle.mouseOver ? 1 : 0)];
		displayBrief = originBrief[(tmpCircle.mouseOver ? 1 : 0)];
		if (tmpCircle.leftClicked || Input::KeyEnter.clicked)
		{
			(music_Music.isPlaying() ? music_Music.pause() : music_Music.play());
			stop_flag = false;
			set_stopFlag(stop_flag);
		}
		tmpCircle = Circle(90, rect_musicBar.y + rect_musicBar.h / 2, 15);
		displayRep = originRep[((tmpCircle.mouseOver || music_Music.isLoop()) ? 1 : 0)];
		if (tmpCircle.leftClicked || Input::KeyShift.clicked)
		{
			const int tmpTime = (int)music_Music.streamPosSample();
			music_Music.pause();
			music_Music.setLoop(music_Music.isLoop() ? false : true);
			music_Music.play();
			music_Music.setPosSample(tmpTime);
			stop_flag = false;
			set_stopFlag(stop_flag);
		}
		tmpCircle = Circle(723, rect_musicBar.y + rect_musicBar.h / 2, 15);
		displayStop = originStop[(tmpCircle.mouseOver ? 1 : 0)];
		if (tmpCircle.leftClicked || Input::KeySpace.clicked)
		{
			music_Music.stop();
			stop_flag = true;
			set_stopFlag(stop_flag);
		}
	}

	// 曲情報 更新
	{
		Update_drawMusicDetailStrings();
		music_musicLength = Format(Pad(music_musicTime / 60, { 2,L'0' }), L":", Pad(music_musicTime % 60, { 2,L'0' }));
		if (rect_music_isFav.leftClicked)
		{
			(isFav(music_albumName, music_musicName) ? removeFav(music_albumName, music_musicName) : addFav(music_albumName, music_musicName, music_musicFileName = music_musicFileName, music_Music));
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
		for (auto i : step(51)) { RectF(1 + i * 15, Window::Height(), 15, -Pow(fft.buffer[i], 0.8) * 750).draw(Color(200, 200, 200, 200)); }
		rect_musicName.drawShadow({ 0,15 }, 32, 10);
		rect_musicName.draw(Color(32, 32, 32, 120));
		rect_musicTime.drawShadow({ 0,15 }, 32, 10);
		rect_musicTime.draw(Color(32, 32, 32, 120));
		rect_music_isFav.drawShadow({ 0,15 }, 32, 10);
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
		{
			RasterizerState rasterizer = RasterizerState::Default2D;
			rasterizer.scissorEnable = true;
			Graphics2D::SetRasterizerState(rasterizer);
			Graphics2D::SetScissorRect(Rect((int)rect_musicName.x, (int)rect_musicName.y, (int)rect_musicName.w, (int)rect_musicName.h));
			music_NameTime(music_musicName).draw(draw_musicName_x, 31 + BAR_HEIGHT);
			Graphics2D::SetScissorRect(Rect(0, 0, Window::Width(), Window::Height()));
		}
		music_NameTime(music_musicLength).draw(504, 31 + BAR_HEIGHT);
		((isFav(music_albumName, music_musicName) || rect_music_isFav.mouseOver) ? faved : not_faved).drawAt(722, 49 + BAR_HEIGHT);
		musicExpl_Draw();
		rect_musicName.drawFrame(0, 2, Palette::Gray);
		rect_musicTime.drawFrame(0, 2, Palette::Gray);
		rect_music_isFav.drawFrame(0, 2, Palette::Gray);
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
		for (int i = 0; i + pos < music_musicExp.length; ++i)
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

// 曲名描画位置 更新
void Update_drawMusicDetailStrings()
{
	auto rect = rect_musicName;
	auto width = music_NameTime(music_musicName).region().w + rect.r;
	if (width > rect_musicName.w)
	{
		if (!draw_musicName_stayFlag)
		{
			if (draw_musicName_x + width > rect.x + rect.w) { draw_musicName_x -= (double)DRAW_MUSICNAME_MOVE_X*(Time::GetMillisec64() - draw_musicName_stayMSec) / 1000; }
			else
			{
				draw_musicName_startMSec = draw_musicName_stayMSec = (int)Time::GetMillisec64();
				draw_musicName_stayFlag = true;
			}
		}
		if (draw_musicName_stayFlag)
		{
			if (draw_musicName_stayMSec - draw_musicName_startMSec >= DRAW_STAYMSEC)
			{
				draw_musicName_startMSec = draw_musicName_stayMSec;
				if (draw_musicName_x == DEFAULT_musicName_X) { draw_musicName_stayFlag = false; }
				else { draw_musicName_x = DEFAULT_musicName_X; }
			}
		}
		draw_musicName_stayMSec = (int)Time::GetMillisec();
	}
}

// 曲手動遷移
void changeMusic(int flag)
{
	favLoop_flag = (get_prevScene() == Scene_Fav || favLoop_flag);
	prev_or_next = flag;
	music_Music.stop();
	if (!favLoop_flag) { setAlbumMusicName(prev_or_next, music_albumName, music_musicFileName, music_Music); }
	else { setFavMusicName(prev_or_next, music_albumName, music_musicFileName, music_Music); }
	TextReader music_reader(L"music\\" + music_albumName + L"\\" + music_musicFileName + L"\\" + music_musicFileName + L".txt");
	music_reader.readLine(music_musicName);
	String temp;
	music_musicExp.clear();
	while (music_reader.readLine(temp)) { music_musicExp += temp + L"\n"; }
	music_musicTime = (int)music_Music.lengthSec();

	draw_musicName_startMSec = (int)Time::GetMillisec();
	draw_musicName_stayFlag = true;
	draw_musicName_x = DEFAULT_musicName_X;

	giveMusicData(music_albumName, music_musicName, music_Music);
	music_Music.play();
}

// 曲操作
// kind: 0->一時停止, 1->再生, 2->停止, 3->繰り返し切り替え
void changeMusicStats(int kind)
{
	switch (kind)
	{
	case 0:
		music_Music.pause();
		stop_flag = false;
		break;
	case 1:
		music_Music.play();
		stop_flag = false;
		break;
	case 2:
		music_Music.stop();
		stop_flag = true;
		break;
	case 3:
		const int tmpTime = (int)music_Music.streamPosSample();
		music_Music.pause();
		music_Music.setLoop(music_Music.isLoop() ? false : true);
		music_Music.play();
		music_Music.setPosSample(tmpTime);
		stop_flag = false;
		break;
	}
}

// 他画面中フラグセット
void set_stillFlag(bool flag)
{
	still_flag = flag;
}

// お気に入りループ中か否か
bool isFavLooping()
{
	return favLoop_flag;
}