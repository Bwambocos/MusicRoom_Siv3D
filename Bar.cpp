// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Bar.h"

// define
#define DEFAULT_mainRectWidth 256

static RoundRect fieldRect(0, 0, 768, BAR_HEIGHT, 16);
static RoundRect mainRect(256, 0, 256, BAR_HEIGHT, 16);
static int32_t mainRectWidth = DEFAULT_mainRectWidth;

// バー 初期化
void Bar_Init()
{

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