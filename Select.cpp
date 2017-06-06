// include
#include <Siv3D.hpp>
#include <vector>
#include "Select.h"
#include "Bar.h"

// define
#define WELCOME_MESSAGE_MILLISEC 3000
#define MAIN_SKEL_LIMIT 80

// 関数プロトタイプ宣言
Rect MakeRect(int32_t x, int32_t y);

// アルバム構造体
struct Album
{
	String name;
	String creator;
	String comment;
	Texture image;
};

// グローバル定数・変数
static Image main_tmp;
static Texture main;
static Texture Gaussian;
static const RectF main_rect(0, BAR_HEIGHT, 768, 768);
static int64_t startTime;
static int64_t nowTime;
static int32_t skel;
static Grid<double_t> z;
static TextReader reader;
static std::vector<Album> AlbumList;

// アルバム選択 初期化
void Select_Init()
{
	// メイン背景
	{
		main_tmp = Image(L"data\\Select\\main.png");
		main = Texture(main_tmp);
		Gaussian = Texture(main_tmp.gaussianBlurred(20, 20));
	}

	// album_list 読み込み
	{
		String temp;
		reader = TextReader(L"music\\album_list.txt");
		while (reader.readLine(temp))
		{
			String name, creator, comment;
			TextReader text(L"music\\" + temp + L"\\" + temp + L".txt");
			text.readLine(name);
			text.readLine(creator);
			String temp_of_temp;
			while (text.readLine(temp_of_temp)) { comment += temp_of_temp; }
			Texture image(L"music\\" + temp + L"\\" + temp + L".png");
			AlbumList.push_back({ name,creator,comment,image });
		}
		z = Grid<double>(3, AlbumList.size() / 3 + 1);
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
		main.draw(0, BAR_HEIGHT);
		Gaussian.draw(0, BAR_HEIGHT);
		main_rect.draw(Color(255, skel));
	}

	// album_list 描画
	{
		int32_t cou = 0;
		for (int32_t y = 0; y < z.height; ++y)
		{
			for (int32_t x = 0; x < z.width; ++x)
			{
				const Rect rect = MakeRect(x, y);
				if (!rect.mouseOver)
				{
					rect(AlbumList[cou].image.resize(216, 216)).draw();
					rect.drawFrame(2, 0, Color(0, 0, 0));
					z[y][x] = Max(z[y][x] - 0.02, 0.0);
				}
				cou = Min<int>(AlbumList.size() - 1, cou + 1);
			}
		}
		cou = 0;
		for (int32_t y = 0; y < z.height; ++y)
		{
			for (int32_t x = 0; x < z.width; ++x)
			{
				const Rect rect = MakeRect(x, y);
				if (rect.mouseOver || z[y][x])
				{
					if (rect.mouseOver) { z[y][x] = Max(z[y][x] + 0.05, 0.5); }
				}
				const double s = z[y][x];
				RectF(rect).stretched(s * 1).drawShadow({ 0,15 * s }, 32 * s, 10 * s);
				RectF(rect).stretched(s * 1)(AlbumList[cou].image.resize(216, 216)).draw();
				cou = Min<int>(AlbumList.size() - 1, cou + 1);
			}
		}
	}
}

Rect MakeRect(int32_t x, int32_t y)
{
	return { 30 + x * 246,BAR_HEIGHT + 30 + y * 246,216,216 };
}