// include
#include <Siv3D.hpp>
#include "Select.h"

// define
#define WELCOME_MESSAGE_MILLISEC 3000
#define MAIN_SKEL_LIMIT 80

// グローバル定数・変数
static Image main_tmp;
static Texture main;
static Texture Gaussian;
static const RectF main_rect(768, 768);
static int64_t startTime;
static int64_t nowTime;
static int32_t skel;

// アルバム選択 初期化
void Select_Init()
{
	// メイン背景
	{
		main_tmp = Image(L"data\\Select\\main.png");
		main = Texture(main_tmp);
		Gaussian = Texture(main_tmp.gaussianBlurred(20, 20));
	}

	startTime = Time::GetMillisec64();
}

// アルバム選択 更新
void Select_Update()
{
	skel = (skel < MAIN_SKEL_LIMIT ? skel + 1 : skel);
	nowTime = Time::GetMillisec64();
}

// アルバム選択 描画
void Select_Draw()
{
	// メイン背景
	{
		main.draw();
		Gaussian.draw();
		main_rect.draw(Color(255, skel));
		main_rect.drawFrame(3);
	}
}