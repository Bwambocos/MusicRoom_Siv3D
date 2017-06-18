// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Bar.h"

// define
#define DEFAULT_mainRectWidth 256

static RoundRect fieldRect(0, 0, 768, BAR_HEIGHT, 16);
static RoundRect mainRect(256, 0, 256, BAR_HEIGHT, 16);
static int32_t mainRectWidth = DEFAULT_mainRectWidth;
static Texture originPlay[2], originBrief[2], originStop[2], originSeek[2], originRep[2], originPrev[2], originNext[2];
static Texture displayPlay, displayBrief, displayStop, displaySeek, displayRep, displayPrev, displayNext;

// バー 初期化
void Bar_Init()
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
}

// バー 更新
void Bar_Update()
{
	mainRectWidth = Max(DEFAULT_mainRectWidth, mainRectWidth);
	mainRect = RoundRect(256 - (mainRectWidth - DEFAULT_mainRectWidth) / 2, 0, mainRectWidth, BAR_HEIGHT, 16);
}

// バー 描画
void Bar_Draw()
{
	fieldRect.draw(Color(200, 200, 200));
	mainRect.draw(Color(125, 125, 125));
}