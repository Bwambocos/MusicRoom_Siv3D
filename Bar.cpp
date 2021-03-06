// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Bar.h"
#include "Music.h"
#include "Detail.h"
#include "Main.h"
#include "Fav.h"

// const
const int mainRectWidth = 384;
const int BAR_DRAW_STAYMSEC = 3500;
const int DEFAULT_mainText_X = 200;
const int DRAW_mainText_MOVE_X = 48;

struct Bar_Music
{
	Sound music;
	String albumName, text;
	int nowTime, totalTime;
};

static Bar_Music music;
static Texture originPlay[2], originBrief[2], originStop[2], originSeek[2], originRep[2], originPrev[2], originNext[2], originShare[2], originBack[2], originGo[2];
static Texture displayPlay, displayBrief, displayStop, displaySeek, displayRep, displayPrev, displayNext, displayShare, displayBack, displayGo;
static Rect fieldRect;
static RoundRect mainRect(192, 0, mainRectWidth, BAR_HEIGHT, 16);
static Sound nowMusic, dog;
static String mainText = L"", hoge = L"";
static Font mainFont, timeFont;
static bool stop_flag = false, draw_back_flag = false, draw_go_flag = false, draw_mainText_stayFlag;
static int draw_mainText_startMSec, draw_mainText_stayMSec;
static double draw_mainText_x;

// バー 初期化
void Bar_Init()
{
	// ボタン 初期化
	{
		originPlay[0] = Texture(L"data\\Bar\\play\\normal.png");
		originPlay[1] = Texture(L"data\\Bar\\play\\active.png");
		originBrief[0] = Texture(L"data\\Bar\\brief\\normal.png");
		originBrief[1] = Texture(L"data\\Bar\\brief\\active.png");
		originStop[0] = Texture(L"data\\Bar\\stop\\normal.png");
		originStop[1] = Texture(L"data\\Bar\\stop\\active.png");
		originSeek[0] = Texture(L"data\\Bar\\seek\\normal.png");
		originSeek[1] = Texture(L"data\\Bar\\seek\\active.png");
		originRep[0] = Texture(L"data\\Bar\\rep\\normal.png");
		originRep[1] = Texture(L"data\\Bar\\rep\\active.png");
		originPrev[0] = Texture(L"data\\Bar\\prev\\normal.png");
		originPrev[1] = Texture(L"data\\Bar\\prev\\active.png");
		originNext[0] = Texture(L"data\\Bar\\next\\normal.png");
		originNext[1] = Texture(L"data\\Bar\\next\\active.png");
		originBack[0] = Texture(L"data\\Bar\\Back\\normal.png");
		originBack[1] = Texture(L"data\\Bar\\Back\\active.png");
		originGo[0] = Texture(L"data\\Bar\\Go\\normal.png");
		originGo[1] = Texture(L"data\\Bar\\Go\\active.png");
		originShare[0] = Texture(L"data\\Bar\\Share\\normal.png");
		originShare[1] = Texture(L"data\\Bar\\Share\\active.png");
		displayPlay = originPlay[0];
		displayBrief = originBrief[0];
		displayStop = originStop[0];
		displaySeek = originSeek[0];
		displayRep = originRep[0];
		displayPrev = originPrev[0];
		displayNext = originNext[0];
		displayBack = originBack[0];
		displayGo = originGo[0];
		displayShare = originShare[0];
	}
	fieldRect = Rect(0, 0, WINDOW_WIDTH, BAR_HEIGHT);
	mainFont = Font(18);
	timeFont = Font(12);
	dog = Sound(L"data\\Bar\\dog.mp3");
}

// バー 更新
void Bar_Update()
{
	if (!music.music.isEmpty() && !music.music.isPlaying() && !stop_flag
		&& music.music.samplesPlayed() % music.music.lengthSample() == 0)
	{
		if (get_prevScene() == Scene_Fav || get_nowScene() == Scene_Fav) setFavMusicName(1, music.albumName, hoge, music.text, music.music);
		else changeMusic(1);
	}
	if (Input::KeyShift.pressed&&Input::KeyD.pressed&&Input::KeyO.pressed&&Input::KeyG.pressed) dog.play();

	// ボタン 更新
	{
		const Rect backRect(10, 10, 44, 44);
		const Rect goRect(WINDOW_WIDTH - 54, 10, 44, 44);
		switch (get_nowScene())
		{
		case Scene_Select:
			draw_back_flag = draw_go_flag = false;
			break;

		case Scene_Detail:
			draw_back_flag = true;
			draw_go_flag = music.music.isPlaying();
			if (backRect.leftClicked)
			{
				music.music.stop();
				stop_flag = true;
				SceneMgr_ChangeScene(Scene_Select);
			}
			if (draw_go_flag && goRect.leftClicked) SceneMgr_ChangeScene(Scene_Music);
			break;

		case Scene_Music:
			draw_back_flag = true;
			draw_go_flag = false;
			if (backRect.leftClicked)
			{
				SceneMgr_ChangeScene((get_prevScene() == Scene_Fav || isFavLooping() ? Scene_Fav : Scene_Detail));
			}
			break;

		case Scene_Fav:
			draw_back_flag = true;
			draw_go_flag = music.music.isPlaying();
			if (backRect.leftClicked)
			{
				music.music.stop();
				stop_flag = true;
				SceneMgr_ChangeScene(Scene_Select);
			}
			if (draw_go_flag && goRect.leftClicked)
			{
				SceneMgr_ChangeScene(Scene_Music);
				setFavMusicName(0, music.albumName, hoge, music.text, music.music);
			}
			break;
		}
		displayBack = originBack[(backRect.mouseOver ? 1 : 0)];
		displayGo = originGo[(goRect.mouseOver ? 1 : 0)];

		if (music.music.isPaused() || music.music.isPlaying())
		{
			int x = 768 / 2 - mainRectWidth / 2 - 40 * 3;
			for (int cou = 0; cou < 6; ++cou)
			{
				const Circle button(x + 20, 12 + 20, 20);
				switch (cou)
				{
				case 0:
					if (button.mouseOver) displayPrev = originPrev[1];
					else displayPrev = originPrev[0];
					if (button.leftClicked)
					{
						changeMusicStats(0);
						setMusicStats(0);
						changeFavMusicStats(0);
						changeMusic(-1);
						stop_flag = false;
					}
					break;
				case 1:
					if (music.music.isPlaying())
					{
						if (button.mouseOver) displayBrief = originBrief[1];
						else displayBrief = originBrief[0];
						if (button.leftClicked)
						{
							(get_nowScene() == Scene_Detail ? setMusicStats(0) : changeMusicStats(0));
							stop_flag = false;
						}
					}
					else
					{
						if (button.mouseOver) displayPlay = originPlay[1];
						else displayPlay = originPlay[0];
						if (button.leftClicked)
						{
							(get_nowScene() == Scene_Detail ? setMusicStats(1) : changeMusicStats(1));
							stop_flag = false;
						}
					}
					break;
				case 2:
					if (button.mouseOver) displayRep = originRep[1];
					else displayRep = originRep[0];
					if (music.music.isLoop()) displayRep = originRep[1];
					if (button.leftClicked)
					{
						const int tmpTime = (int)music.music.streamPosSample();
						music.music.pause();
						music.music.setLoop(music.music.isLoop() ? false : true);
						music.music.play();
						music.music.setPosSample(tmpTime);
						stop_flag = false;
					}
					x += mainRectWidth;
					break;
				case 3:
					if (button.mouseOver) displayStop = originStop[1];
					else displayStop = originStop[0];
					if (button.leftClicked)
					{
						changeMusicStats(2);
						stop_flag = true;
					}
					break;
				case 4:
					if (button.mouseOver) displayShare = originShare[1];
					else displayShare = originShare[0];
					if (button.leftClicked) Twitter::OpenTweetWindow(L"#MusicRoom v3.0 でアルバム『" + music.albumName + L"』の曲「" + music.text + L"」を聴いています！ダウンロードはこちらから：https://github.com/Bwambocos/MusicRoom_Siv3D/releases");
					break;
				case 5:
					if (button.mouseOver) displayNext = originNext[1];
					else displayNext = originNext[0];
					if (button.leftClicked)
					{
						changeMusicStats(0);
						setMusicStats(0);
						changeFavMusicStats(0);
						changeMusic(1);
						stop_flag = false;
					}
					break;
				}
				x += 40;
			}
		}
	}

	// メインテキスト 更新
	{
		if (!music.music.isPlaying())
		{
			auto nowScene = get_nowScene();
			switch (nowScene)
			{
			case Scene_Select:
				mainText = L"アルバムを選択してください";
				break;
			case Scene_Detail:
				mainText = L"曲を選択してください";
				break;
			case Scene_Fav:
				mainText = L"曲を選択してください";
				break;
			}
		}
		else mainText = L"『" + music.albumName + L"』" + music.text;
	}

	Update_drawMainText();
}

// バー 描画
void Bar_Draw()
{
	fieldRect.draw(Color(204, 61, 104));
	mainRect.draw(Color(224,67,114));

	// ボタン 描画
	{
		if (draw_back_flag) displayBack.draw(10, 10);
		if (draw_go_flag) displayGo.draw(WINDOW_WIDTH - 10 - displayGo.width, 10);
		int x = 768 / 2 - mainRectWidth / 2 - 40 * 3;
		for (int cou = 0; cou < 6; ++cou)
		{
			switch (cou)
			{
			case 0:
				displayPrev.draw(x, 12);
				break;
			case 1:
				if (music.music.isPlaying()) displayBrief.draw(x, 12);
				else displayPlay.draw(x, 12);
				break;
			case 2:
				displayRep.draw(x, 12);
				x += mainRectWidth;
				break;
			case 3:
				displayStop.draw(x, 12);
				break;
			case 4:
				displayShare.draw(x, 12);
				break;
			case 5:
				displayNext.draw(x, 12);
				break;
			}
			x += 40;
		}
	}

	// メインテキスト 描画
	{
		RasterizerState rasterizer = RasterizerState::Default2D;
		rasterizer.scissorEnable = true;
		Graphics2D::SetRasterizerState(rasterizer);
		Graphics2D::SetScissorRect(Rect((int)mainRect.x, (int)mainRect.y, (int)mainRect.w, (int)mainRect.h));
		mainFont(mainText).draw(draw_mainText_x, 15);
		Graphics2D::SetScissorRect(Rect(0, 0, Window::Width(), Window::Height()));
	}
}

// 再生中か取得
bool is_nowMusicPlaying()
{
	return nowMusic.isPlaying();
}

// 曲詳細データ受け渡し
void giveMusicData(String albumName, String musicName, Sound musicData)
{
	music.albumName = albumName;
	music.text = musicName;
	music.music = musicData;
	stop_flag = false;

	// 描画位置 初期化
	draw_mainText_startMSec = (int)Time::GetMillisec();
	draw_mainText_stayFlag = true;
	draw_mainText_x = DEFAULT_mainText_X;
}

// 停止フラグ 設定
void set_stopFlag(bool flag)
{
	stop_flag = flag;
}

// 曲名描画位置 更新
void Update_drawMainText()
{
	auto rect = mainRect;
	auto width = mainFont(mainText).region().w + rect.r;
	if (width > rect.w)
	{
		if (!draw_mainText_stayFlag)
		{
			if (draw_mainText_x + width > rect.x + rect.w) draw_mainText_x -= (double)DRAW_mainText_MOVE_X*(Time::GetMillisec64() - draw_mainText_stayMSec) / 1000;
			else
			{
				draw_mainText_startMSec = draw_mainText_stayMSec = (int)Time::GetMillisec64();
				draw_mainText_stayFlag = true;
			}
		}
		if (draw_mainText_stayFlag)
		{
			if (draw_mainText_stayMSec - draw_mainText_startMSec >= BAR_DRAW_STAYMSEC)
			{
				draw_mainText_startMSec = draw_mainText_stayMSec;
				const Rect tmpRect = mainFont(mainText).region();
				if (draw_mainText_x == DEFAULT_mainText_X) draw_mainText_stayFlag = false;
				else draw_mainText_x = DEFAULT_mainText_X;
			}
		}
		draw_mainText_stayMSec = (int)Time::GetMillisec();
	}
	else
	{
		const Rect tempRect = mainFont(mainText).region();
		draw_mainText_x = 384 - (int)tempRect.w / 2;
	}
}