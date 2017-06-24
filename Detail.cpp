// include
#include <Siv3D.hpp>
#include "Detail.h"
#include "Bar.h"
#include "Select.h"
#include "Fav.h"

// 曲リスト 構造体
struct List
{
	bool isPlaying;
	Sound music;
	String name;
	int32_t totalTime;
	bool isFav;
};

// グローバル変数
static Texture main;
static RoundRect rect_albumImage(25, 25 + BAR_HEIGHT, 250, 250, 12.5);
static RoundRect rect_albumName(325, 25 + BAR_HEIGHT, 393, 54, 10);
static RoundRect rect_albumCreator(325, 82 + BAR_HEIGHT, 393, 48, 10);
static RoundRect rect_albumExpl(325, 133 + BAR_HEIGHT, 393, 142, 10);
static RoundRect rect_albumList_Flag(25, 300 + BAR_HEIGHT, 36, 36, 5);
static RoundRect rect_albumList_Name(64, 300 + BAR_HEIGHT, 537, 36, 5);
static RoundRect rect_albumList_Time(604, 300 + BAR_HEIGHT, 100, 36, 5);
static RoundRect rect_albumList_Fav(707, 300 + BAR_HEIGHT, 36, 36, 5);
static String albumName, albumCreator, albumExpl;
static Texture albumImg;
static std::vector<List>albumList;
Font font_albumName, font_albumCreator, font_albumExpl;

// アルバム詳細 初期化
void Detail_Init()
{
	main = Texture(L"data\\Detail\\main.png");
	String temp_albumName = L"T.Dragons ActⅡ";

	// アルバム情報 初期化
	{
		TextReader reader(L"music\\" + temp_albumName + L"\\" + temp_albumName + L".txt");
		reader.readLine(albumName);
		reader.readLine(albumCreator);
		albumImg = Texture(L"music\\" + temp_albumName + L"\\" + albumName + L".png");
		String temp;
		while (reader.readLine(temp)) { albumExpl += temp + L"\n"; }
		font_albumName = Font(24);
		font_albumCreator = Font(18);
		font_albumExpl = Font(12);
	}

	// 曲リスト 初期化
	{
		const String extensions[] = { L".wav",L".ogg",L".mp3" };
		TextReader reader(L"music\\" + temp_albumName + L"\\music_list.txt");
		String tempName; Sound tempMusic; int32_t temp_totalTime;
		bool temp_isFav;
		while (reader.readLine(tempName))
		{
			for (auto ext : extensions)
			{
				if (FileSystem::IsFile(L"music\\" + temp_albumName + L"\\" + tempName + L"\\" + tempName + ext))
				{
					tempMusic = Sound(L"music\\" + temp_albumName + L"\\" + tempName + L"\\" + tempName + L".mp3");
					break;
				}
			}
			temp_totalTime = tempMusic.lengthSample();
			temp_isFav = isFav(albumName, tempName);
			albumList.push_back({ false,tempMusic,tempName,temp_totalTime,temp_isFav });
		}
	}
}

// アルバム詳細 更新
void Detail_Update()
{

}

// アルバム詳細 描画
void Detail_Draw()
{
	// 背景 描画
	{
		main.draw(0, BAR_HEIGHT);
		rect_albumImage.drawShadow({ 0,15 }, 32, 10);
		rect_albumImage.drawFrame(3);
		rect_albumImage.draw(Color(32, 32, 32, 120));
		rect_albumName.drawShadow({ 0,15 }, 32, 10);
		rect_albumName.drawFrame(3);
		rect_albumName.draw(Color(32, 32, 32, 120));
		rect_albumCreator.drawShadow({ 0,15 }, 32, 10);
		rect_albumCreator.drawFrame(3);
		rect_albumCreator.draw(Color(32, 32, 32, 120));
		rect_albumExpl.drawShadow({ 0,15 }, 32, 10);
		rect_albumExpl.drawFrame(3);
		rect_albumExpl.draw(Color(32, 32, 32, 120));
		for (int32_t i = 0; i < 5; ++i)
		{
			RoundRect(rect_albumList_Flag.x, rect_albumList_Flag.y + i * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r).draw(Color(32, 32, 32, 200));
			RoundRect(rect_albumList_Name.x, rect_albumList_Name.y + i * 39, rect_albumList_Name.w, rect_albumList_Name.h, rect_albumList_Name.r).draw(Color(32, 32, 32, 200));
			RoundRect(rect_albumList_Time.x, rect_albumList_Time.y + i * 39, rect_albumList_Time.w, rect_albumList_Time.h, rect_albumList_Time.r).draw(Color(32, 32, 32, 200));
			RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + i * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r).draw(Color(32, 32, 32, 200));
		}
	}

	// アルバム情報 描画
	{
		const Rect rect(37.5, 37.5 + BAR_HEIGHT, 225, 225);
		rect(albumImg).draw();
		rect.drawFrame(0, 2, Color(200, 200, 200));
		font_albumName(albumName).draw(333, 27 + BAR_HEIGHT);
		font_albumCreator(albumCreator).draw(333, 88 + BAR_HEIGHT);
		font_albumExpl(albumExpl).draw(333, 142 + BAR_HEIGHT);
	}
}