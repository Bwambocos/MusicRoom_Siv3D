// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
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
static std::map<String, std::vector<List>>albums;
static std::vector<List>albumList;
static Texture main, playing;
static RoundRect rect_albumImage(25, 25 + BAR_HEIGHT, 250, 250, 12.5);
static RoundRect rect_albumName(325, 25 + BAR_HEIGHT, 393, 54, 10);
static RoundRect rect_albumCreator(325, 82 + BAR_HEIGHT, 393, 48, 10);
static RoundRect rect_albumExpl(325, 133 + BAR_HEIGHT, 393, 142, 10);
static RoundRect rect_albumList_Flag(25, 300 + BAR_HEIGHT, 36, 36, 5);
static RoundRect rect_albumList_Name(64, 300 + BAR_HEIGHT, 537, 36, 5);
static RoundRect rect_albumList_Time(604, 300 + BAR_HEIGHT, 100, 36, 5);
static RoundRect rect_albumList_Fav(707, 300 + BAR_HEIGHT, 36, 36, 5);
static RoundRect rect_albumList(25, 300 + BAR_HEIGHT, 718, 190, 5);
static String albumName = L"", albumCreator = L"", albumExpl = L"";
static Texture albumImg;
static Font font_albumName, font_albumCreator, font_albumExpl;
static Font font_albumList;
static int32_t albumList_begin;

// アルバム詳細 初期化
void Detail_Init()
{
	if (!main) { main = Texture(L"data\\Detail\\main.png"); }
	if (!playing) { playing = Texture(L"data\\Detail\\playing.png"); }
	String temp_albumName = getSetAlbum();

	// アルバム情報 初期化
	{
		albumName.clear();
		albumCreator.clear();
		albumExpl.clear();
		albumList.clear();
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

	// まだ読み込まれていないアルバム
	if (albums.find(temp_albumName) == albums.end())
	{
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
				temp_totalTime = (int32_t)tempMusic.lengthSample();
				temp_isFav = isFav(albumName, tempName);
				albumList.push_back({ false,tempMusic,tempName,temp_totalTime,temp_isFav });
			}
			font_albumList = Font(16);
		}
		albums[temp_albumName] = albumList;
	}
	else { albumList = albums[temp_albumName]; }
}

// アルバム詳細 更新
void Detail_Update()
{
	if (Input::KeyB.pressed) { SceneMgr_ChangeScene(Scene_Select); }
	if (rect_albumList.mouseOver)
	{
		bool scr_flag = ((albumList_begin + 5 <= (signed)albumList.size()) || (albumList_begin > 0) ? true : false);
		if (scr_flag)
		{
			albumList_begin += Mouse::Wheel();
			Print(albumList_begin);
			albumList_begin = Max(albumList_begin, 0);
			albumList_begin = Min<int32_t>(albumList_begin, albumList.size() - 5);
		}
	}
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
		const Rect rect((int)37.5, (int)37.5 + BAR_HEIGHT, 225, 225);
		rect(albumImg).draw();
		rect.drawFrame(0, 2, Color(200, 200, 200));
		font_albumName(albumName).draw(333, 27 + BAR_HEIGHT);
		font_albumCreator(albumCreator).draw(333, 88 + BAR_HEIGHT);
		albumExpl_Draw();
	}

	// 曲リスト 描画
	{
		for (int32_t i = albumList_begin; (i - albumList_begin) < Min<int32_t>(5, albumList.size()); ++i)
		{
			auto num = i - albumList_begin;
			if (albumList[i].isPlaying) { playing.draw(30, 304 + BAR_HEIGHT + num * (36 + 2)); }
			else { font_albumList(i + 1).draw(30, 304 + BAR_HEIGHT + num * (36 + 2)); }
			font_albumList(albumList[i].name).draw(70, 304 + BAR_HEIGHT + num * (36 + 2));
		}
	}
}

// アルバム説明 描画
void albumExpl_Draw()
{
	Array<String> texts;
	const int32 w = (int32)rect_albumExpl.w - 10;
	const int32 h = (int32)rect_albumExpl.h;
	size_t pos = 0;

	while (pos < albumExpl.length)
	{
		for (int32_t i = 0; i + pos < albumExpl.length; ++i)
		{
			if (font_albumExpl(albumExpl.substr(pos, i)).region().w >= w)
			{
				texts.push_back(albumExpl.substr(pos, i - 1));
				pos += (i - 1);
				break;
			}
			if (albumExpl[pos + i] == L'\n')
			{
				texts.push_back(albumExpl.substr(pos, i + 1));
				pos += (i + 1);
				break;
			}
		}
	}

	for (size_t i = 0; i < texts.size(); ++i)
	{
		const int32 y = static_cast<int32>(rect_albumExpl.y + 10 + i * font_albumExpl.height);
		const bool overflow = (i + 1 < texts.size())
			&& (y + font_albumExpl.height * 2 + 10 > rect_albumExpl.y + rect_albumExpl.h);
		if (overflow)
		{
			if (texts[i].length > 2) { texts[i].resize(texts[i].length - (texts[i].length > 2 ? 2 : 1)); }
			if (texts[i][texts[i].length - 1] == L'\n') { texts[i][texts[i].length - 1] = L'\0'; }
			texts[i].append(L"...");
		}
		font_albumExpl(texts[i]).draw(rect_albumExpl.x + 10, y);
		if (overflow) { break; }
	}
}