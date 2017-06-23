// include
#include <Siv3D.hpp>
#include "Detail.h"
#include "Bar.h"
#include "Select.h"

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
static RoundRect albumImage(25, 25 + BAR_HEIGHT, 250, 250, 12.5);
static RoundRect albumDetail(325, 25 + BAR_HEIGHT, 393, 250, 12.5);
static RoundRect albumList_Flag(25, 300 + BAR_HEIGHT, 36, 36, 5);
static RoundRect albumList_Name(64, 300 + BAR_HEIGHT, 537, 36, 5);
static RoundRect albumList_Time(604, 300 + BAR_HEIGHT, 100, 36, 5);
static RoundRect albumList_Fav(707, 300 + BAR_HEIGHT, 36, 36, 5);
static String albumName, albumCreator, albumExpl;
static Texture albumImg;
static std::vector<List>albumList;

// アルバム詳細 初期化
void Detail_Init()
{
	main = Texture(L"data\\Detail\\main.png");
	albumName = getSetAlbum();

	// 曲リスト 初期化
	{
		const String extensions[] = { L".wav",L".ogg",L".mp3" };
		TextReader reader(L"music\\" + albumName + L"\\music_list.txt");
		String tempName; Sound tempMusic; int32_t temp_totalTime;
		while (reader.readLine(tempName))
		{
			for (auto ext : extensions)
			{
				if (FileSystem::IsFile(L"music\\" + albumName + L"\\" + tempName + L"\\" + tempName + ext))
				{
					tempMusic = Sound(L"music\\" + albumName + L"\\" + tempName + L"\\" + tempName + L".mp3");
					break;
				}
			}
			temp_totalTime = tempMusic.lengthSample();
			albumList.push_back({ false,tempMusic,tempName,temp_totalTime,false });
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
		albumImage.drawShadow({ 0,15 }, 32, 10);
		albumImage.drawFrame(3);
		albumImage.draw(Color(32, 32, 32, 120));
		albumDetail.drawShadow({ 0,15 }, 32, 10);
		albumDetail.drawFrame(3);
		albumDetail.draw(Color(32, 32, 32, 120));
		for (int32_t i = 0; i < 5; ++i)
		{
			RoundRect(albumList_Flag.x, albumList_Flag.y + i * 39, albumList_Flag.w, albumList_Flag.h, albumList_Flag.r).draw(Color(32, 32, 32, 200));
			RoundRect(albumList_Name.x, albumList_Name.y + i * 39, albumList_Name.w, albumList_Name.h, albumList_Name.r).draw(Color(32, 32, 32, 200));
			RoundRect(albumList_Time.x, albumList_Time.y + i * 39, albumList_Time.w, albumList_Time.h, albumList_Time.r).draw(Color(32, 32, 32, 200));
			RoundRect(albumList_Fav.x, albumList_Fav.y + i * 39, albumList_Fav.w, albumList_Fav.h, albumList_Fav.r).draw(Color(32, 32, 32, 200));
		}
	}
}