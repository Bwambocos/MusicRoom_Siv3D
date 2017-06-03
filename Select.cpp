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
static const RectF welcome_rec({ 60,290 }, { 660,190 });
static int64_t startTime;
static int64_t nowTime;
static int32_t skel;
static Font font;
static bool welcome_flag = true;

// アルバム選択 初期化
void Select_Init()
{
	// メイン背景
	{
		main_tmp = Image(L"data\\Select\\main.png");
		main = Texture(main_tmp);
		Gaussian = Texture(main_tmp.gaussianBlurred(20, 20));
	}

	// Welcome メッセージ
	font = Font(48);

	startTime = Time::GetMillisec64();
}

// アルバム選択 更新
void Select_Update()
{
	nowTime = Time::GetMillisec64();

	// Welcome メッセージ
	if ((nowTime - startTime) >= WELCOME_MESSAGE_MILLISEC)
	{
		if (skel <= MAIN_SKEL_LIMIT) { ++skel; }
		else { welcome_flag = false; }
	}
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

	// Welcome メッセージ
	if (welcome_flag)
	{
		welcome_rec.draw(Color(255, 100));
		font(L"MusicRoom v2.0 へ\nようこそ！").drawCenter({ 384,384 }, Color(255, 255 - skel * 3));
	}
}