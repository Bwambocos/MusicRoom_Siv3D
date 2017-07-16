// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Bar.h"

// define
#define DEFAULT_mainRectWidth 256

static RoundRect fieldRect(0, 0, 768, BAR_HEIGHT, 16);
static RoundRect mainRect(256, 0, 256, BAR_HEIGHT, 16);
static int32_t mainRectWidth = DEFAULT_mainRectWidth;
static int32_t nowTime, totalTime;
static Texture displayPlay, displayBrief, displayStop, displaySeek, displayRep, displayPrev, displayNext;
static String mainText = L"";
static Sound nowMusic;

// バー 初期化
void Bar_Init()
{
	// ボタン 初期化
	{
		TextureAsset::Register(L"originPlay_normal", L"data\\Bar\\play\\normal.png");
		TextureAsset::Register(L"originPlay_active", L"data\\Bar\\play\\active.png");
		TextureAsset::Register(L"originBrief_normal", L"data\\Bar\\Brief\\normal.png");
		TextureAsset::Register(L"originBrief_active", L"data\\Bar\\Brief\\active.png");
		TextureAsset::Register(L"originStop_normal", L"data\\Bar\\Stop\\normal.png");
		TextureAsset::Register(L"originStop_active", L"data\\Bar\\Stop\\active.png");
		TextureAsset::Register(L"originSeek_normal", L"data\\Bar\\Seek\\normal.png");
		TextureAsset::Register(L"originSeek_active", L"data\\Bar\\Seek\\active.png");
		TextureAsset::Register(L"originRep_normal", L"data\\Bar\\Rep\\normal.png");
		TextureAsset::Register(L"originRep_active", L"data\\Bar\\Rep\\active.png");
		TextureAsset::Register(L"originPrev_normal", L"data\\Bar\\Prev\\normal.png");
		TextureAsset::Register(L"originPrev_active", L"data\\Bar\\Prev\\active.png");
		TextureAsset::Register(L"originNext_normal", L"data\\Bar\\Next\\normal.png");
		TextureAsset::Register(L"originNext_active", L"data\\Bar\\Next\\active.png");
		displayPlay = TextureAsset(L"originPlay_normal");
		displayBrief = TextureAsset(L"originBrief_normal");
		displayStop = TextureAsset(L"originStop_normal");
		displaySeek = TextureAsset(L"originSeek_normal");
		displayRep = TextureAsset(L"originRep_normal");
		displayPrev = TextureAsset(L"originPrev_normal");
		displayNext = TextureAsset(L"originNext_normal");
	}

	// フォント 初期化
	{
		FontAsset::Register(L"mainFont", 18);
		FontAsset::Register(L"timeFont", 12);
	}
}

// バー 更新
void Bar_Update()
{
	const Rect rect = FontAsset(L"mainFont")(mainText).region();
	mainRectWidth = rect.w + 18 * 2;
	mainRectWidth = Max(DEFAULT_mainRectWidth, mainRectWidth);
	mainRect = RoundRect(256 - (mainRectWidth - DEFAULT_mainRectWidth) / 2, 0, mainRectWidth, BAR_HEIGHT, 16);
	
	// ボタン 更新
	{
		int32_t x = 768 / 2 - mainRectWidth / 2 - 40 * 3;
		for (int32_t cou = 0; cou < 5; ++cou)
		{
			const Circle button(x + 20, 12 + 20, 20);
			switch (cou)
			{
			case 0:
				if (button.mouseOver) { displayPrev = TextureAsset(L"originPrev_active"); }
				else { displayPrev = TextureAsset(L"originPrev_normal"); }
				break;
			case 1:
				if (button.mouseOver) { displayPlay = TextureAsset(L"originPlay_active");}
				else { displayPlay = TextureAsset(L"originPlay_normal"); }
				break;
			case 2:
				if (button.mouseOver) { displayRep = TextureAsset(L"originRep_active"); }
				else { displayRep = TextureAsset(L"originRep_normal"); }
				x += mainRectWidth;
				break;
			case 3:
				if (button.mouseOver) { displayStop = TextureAsset(L"originStop_active"); }
				else { displayStop = TextureAsset(L"originStop_normal"); }
				break;
			case 4:
				if (button.mouseOver) { displayNext = TextureAsset(L"originNext_active"); }
				else { displayNext = TextureAsset(L"originNext_normal"); }
				break;
			}
			x += 40;
		}
	}

	// メインテキスト 更新
	{
		if (!nowMusic.isPlaying())
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
			}
		}
	}

	// 再生位置テキスト 更新
	{
		if (nowMusic.isPlaying())
		{
			totalTime = (int32_t)nowMusic.lengthSec();
			nowTime = (int32_t)nowMusic.streamPosSec();
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
				if (nowMusic.isPlaying()) { displayBrief.draw(x, 12); }
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
		const Rect rect = FontAsset(L"mainFont")(mainText).region();
		FontAsset(L"mainFont").draw(mainText, 768 / 2 - rect.w / 2, 15);
	}

	// 再生位置テキスト 描画
	{
		if (nowMusic.isPlaying())
		{
			const Rect rect = FontAsset(L"timeFont")(nowTime + L":" + totalTime).region();
			FontAsset(L"timeFont").draw(nowTime + L":" + totalTime, (768 / 2 + mainRectWidth / 2) - rect.w, 12, Color(0, 0, 0));
		}
	}
}

// 再生中か取得
bool is_nowMusicPlaying()
{
	return nowMusic.isPlaying();
}