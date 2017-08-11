// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Bar.h"
#include "Music.h"

// define
#define DEFAULT_mainRectWidth 256

struct Music
{
	Sound music;
	String albumName;
	String musicName;
	int32_t nowTime;
	int32_t totalTime;
};

static Music music;
static Texture originPlay[2], originBrief[2], originStop[2], originSeek[2], originRep[2], originPrev[2], originNext[2];
static Texture displayPlay, displayBrief, displayStop, displaySeek, displayRep, displayPrev, displayNext;
static RoundRect fieldRect(0, 0, 768, BAR_HEIGHT, 16);
static RoundRect mainRect(256, 0, 256, BAR_HEIGHT, 16);
static Sound nowMusic;
static String mainText = L"";
static Font mainFont, timeFont;
static int32_t mainRectWidth = DEFAULT_mainRectWidth;
static bool stop_flag = false;

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
		displayPlay = originPlay[0];
		displayBrief = originBrief[0];
		displayStop = originStop[0];
		displaySeek = originSeek[0];
		displayRep = originRep[0];
		displayPrev = originPrev[0];
		displayNext = originNext[0];
	}

	mainFont = Font(18);
	timeFont = Font(12);
}

// バー 更新
void Bar_Update()
{
	const Rect rect = mainFont(mainText).region();
	mainRectWidth = Max(DEFAULT_mainRectWidth, rect.w + 18 * 2);
	mainRect = RoundRect(256 - (mainRectWidth - DEFAULT_mainRectWidth) / 2, 0, mainRectWidth, BAR_HEIGHT, 16);

	if (!music.music.isEmpty() && !music.music.isPlaying() && !stop_flag
		&& music.music.samplesPlayed() % music.music.lengthSample() == 0) { changeMusic(1); }
	
	// ボタン 更新
	{
		int32_t x = 768 / 2 - mainRectWidth / 2 - 40 * 3;
		for (int32_t cou = 0; cou < 5; ++cou)
		{
			const Circle button(x + 20, 12 + 20, 20);
			switch (cou)
			{
			case 0:
				if (button.mouseOver) { displayPrev = originPrev[1]; }
				else { displayPrev = originPrev[0]; }
				if (button.leftClicked) { changeMusic(-1); }
				break;
			case 1:
				if (music.music.isPlaying())
				{
					if (button.mouseOver) { displayBrief = originBrief[1]; }
					else { displayBrief = originBrief[0]; }
					if (button.leftClicked)
					{
						changeMusicStats(0);
						stop_flag = false;
					}
				}
				else
				{
					if (button.mouseOver) { displayPlay = originPlay[1]; }
					else { displayPlay = originPlay[0]; }
					if (button.leftClicked)
					{
						changeMusicStats(1);
						stop_flag = false;
					}
				}
				break;
			case 2:
				if (button.mouseOver) { displayRep = originRep[1]; }
				else { displayRep = originRep[0]; }
				if (button.leftClicked)
				{
					changeMusicStats(3);
					stop_flag = false;
				}
				x += mainRectWidth;
				break;
			case 3:
				if (button.mouseOver) { displayStop = originStop[1]; }
				else { displayStop = originStop[0]; }
				if (button.leftClicked)
				{
					changeMusicStats(2);
					stop_flag = true;
				}
				break;
			case 4:
				if (button.mouseOver) { displayNext = originNext[1]; }
				else { displayNext = originNext[0]; }
				if (button.leftClicked) { changeMusic(1); }
				break;
			}
			x += 40;
		}
	}

	// メインテキスト 更新
	{
		auto nowScene = get_nowScene();
		if (nowScene != Scene_Music)
		{
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
		else { mainText = L"『" + music.albumName + L"』" + music.musicName; }
	}

	// 再生位置テキスト 更新
	{
		if (nowMusic.isPlaying())
		{
			music.totalTime = (int32_t)nowMusic.lengthSec();
			music.nowTime = (int32_t)nowMusic.streamPosSec();
		}
	}
}

// バー 描画
void Bar_Draw()
{
	fieldRect.draw(Color(200, 200, 200));
	mainRect.draw(Color(125, 125, 125));

	// ボタン 描画
	{
		int32_t x = 768 / 2 - mainRectWidth / 2 - 40 * 3;
		for (int32_t cou = 0; cou < 5; ++cou)
		{
			switch (cou)
			{
			case 0:
				displayPrev.draw(x, 12);
				break;
			case 1:
				if (music.music.isPlaying()) { displayBrief.draw(x, 12); }
				else { displayPlay.draw(x, 12); }
				break;
			case 2:
				displayRep.draw(x, 12);
				x += mainRectWidth;
				break;
			case 3:
				displayStop.draw(x, 12);
				break;
			case 4:
				displayNext.draw(x, 12);
				break;
			}
			x += 40;
		}
	}

	// メインテキスト 描画
	{
		const Rect rect = mainFont(mainText).region();
		mainFont.draw(mainText, 768 / 2 - rect.w / 2, 15);
	}

	// 再生位置テキスト 描画
	{
		if (nowMusic.isPlaying())
		{
			const Rect rect = timeFont(music.nowTime + L":" + music.totalTime).region();
			timeFont.draw(music.nowTime + L":" + music.totalTime, (768 / 2 + mainRectWidth / 2) - rect.w, 12, Color(0, 0, 0));
		}
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
	music.musicName = musicName;
	music.music = musicData;
}