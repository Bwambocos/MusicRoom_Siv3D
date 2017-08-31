// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Detail.h"
#include "Bar.h"
#include "Select.h"
#include "Fav.h"
#include "Music.h"

// define
#define DEFAULT_albumName_X 333
#define DEFAULT_albumCreator_X 333
#define DEFAULT_albumExpl_Y 143 + BAR_HEIGHT
#define DRAW_STAYMSEC 3500

// 曲リスト 構造体
struct List_detail
{
	Sound music;
	String displayName;
	String originName;
	int totalTime;
};

// グローバル変数
static std::map<String, std::vector<List_detail>>albums;
static std::vector<List_detail>albumList;
static Texture main, playing, pausing, not_fav, fav, albumImg;
static Font font_albumName, font_albumCreator, font_albumExpl;
static Font font_albumList;
static String albumName = L"", albumCreator = L"", albumExpl = L"", selectedAlbumName = L"", selectedMusicName = L"";
static RoundRect rect_albumImage(25, 25 + BAR_HEIGHT, 250, 250, 12.5);
static RoundRect rect_albumName(325, 25 + BAR_HEIGHT, 393, 54, 10);
static RoundRect rect_albumCreator(325, 82 + BAR_HEIGHT, 393, 48, 10);
static RoundRect rect_albumExpl(325, 133 + BAR_HEIGHT, 393, 142, 10);
static RoundRect rect_albumList_Flag(25, 300 + BAR_HEIGHT, 36, 36, 5);
static RoundRect rect_albumList_Name(64, 300 + BAR_HEIGHT, 537, 36, 5);
static RoundRect rect_albumList_Time(604, 300 + BAR_HEIGHT, 100, 36, 5);
static RoundRect rect_albumList_Fav(707, 300 + BAR_HEIGHT, 36, 36, 5);
static RoundRect rect_albumListAll(25, 300 + BAR_HEIGHT, 718, 190, 5);
static RoundRect rect_albumListCell(64, 300 + BAR_HEIGHT, 582, 36, 5);
static Triangle goUp({ 384,350 }, { 414,360 }, { 354,360 });
static Triangle goDown({ 354,560 }, { 414,560 }, { 384,570 });
static Sound selectedMusic;
static int albumList_begin;
static int draw_albumName_x, draw_albumCreator_x;
static double draw_albumExpl_y;
static int64 draw_albumName_startMSec, draw_albumCreator_startMSec, draw_albumExpl_startMSec, draw_albumName_stayMSec, draw_albumCreator_stayMSec, draw_albumExpl_stayMSec;
static bool draw_albumName_stayFlag, draw_albumCreator_stayFlag, draw_albumExpl_stayFlag;
static int selectedMusic_num;

// アルバム詳細 初期化
void Detail_Init()
{
	if (!main)
	{
		main = Texture(L"data\\Detail\\main.png");
		playing = Texture(L"data\\Detail\\playing.png");
		pausing = Texture(L"data\\Detail\\pausing.png");
		not_fav = Texture(L"data\\Detail\\not_fav.png");
		fav = Texture(L"data\\Detail\\fav.png");
	}

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
		albumList_begin = 0;
	}

	// まだ読み込まれていないアルバム
	if (albums.find(temp_albumName) == albums.end())
	{
		// 曲リスト 初期化
		{
			font_albumList = Font(16);
			const String extensions[] = { L".wav",L".ogg",L".mp3" };
			TextReader reader(L"music\\" + temp_albumName + L"\\music_list.txt");
			String tempName; Sound tempMusic; int temp_totalTime;
			while (reader.readLine(tempName))
			{
				for (auto ext : extensions)
				{
					if (FileSystem::IsFile(L"music\\" + temp_albumName + L"\\" + tempName + L"\\" + tempName + ext))
					{
						tempMusic = Sound(L"music\\" + temp_albumName + L"\\" + tempName + L"\\" + tempName + ext);
						break;
					}
				}
				if (!tempMusic) { tempName = L"！読み込み失敗！"; }
				temp_totalTime = (int)tempMusic.lengthSec();
				albumList.push_back({ tempMusic,Detail_musicNameBeShort(tempName),tempName,temp_totalTime });
			}
		}
		albums[temp_albumName] = albumList;
	}
	else { albumList = albums[temp_albumName]; }

	// 描画位置 初期化
	draw_albumName_startMSec = Time::GetMillisec64();
	draw_albumCreator_startMSec = Time::GetMillisec64();
	draw_albumExpl_startMSec = Time::GetMillisec64();
	draw_albumName_stayFlag = true;
	draw_albumCreator_stayFlag = true;
	draw_albumExpl_stayFlag = true;
	draw_albumName_x = DEFAULT_albumName_X;
	draw_albumCreator_x = DEFAULT_albumCreator_X;
	draw_albumExpl_y = DEFAULT_albumExpl_Y;
}

// アルバム詳細 更新
void Detail_Update()
{
	if (Input::KeyB.pressed) { SceneMgr_ChangeScene(Scene_Select); }

	// アルバム情報 更新
	Update_drawAlbumDetailStrings();

	// 曲リスト 更新
	{
		if (goUp.leftClicked) { --albumList_begin; }
		if (goDown.leftClicked) { ++albumList_begin; }
		if (rect_albumListAll.mouseOver)
		{
			bool scr_flag = ((albumList_begin + 5 <= (signed)albumList.size()) || (albumList_begin > 0) ? true : false);
			if (scr_flag) { albumList_begin += Mouse::Wheel(); }
		}
		albumList_begin = Max(albumList_begin, 0);
		albumList_begin = Min<int>(albumList_begin, Max<int>((int)albumList.size() - 5, 0));

		for (int i = albumList_begin; ((i - albumList_begin) < Min<int>(5, (signed)albumList.size())) && (i < (signed)albumList.size()); ++i)
		{
			auto num = i - albumList_begin;
			auto music = albumList[i];
			RoundRect rect(rect_albumList_Flag.x, rect_albumList_Flag.y + num * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r);
			if (rect.leftClicked)
			{
				if (selectedMusic_num != i) { albumList[selectedMusic_num].music.stop(); }
				(music.music.isPlaying() ? music.music.pause() : music.music.play());
				selectedMusic_num = i;
				selectedAlbumName = albumName;
				selectedMusicName = music.originName;
				selectedMusic = music.music;
				giveMusicData(albumName, music.originName, music.music);
			}
			rect = RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + num * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r);
			if (rect.leftClicked)
			{
				(isFav(albumName, music.originName) ? removeFav(albumName, music.originName) : addFav(albumName, music.originName, music.music));
			}
			rect = RoundRect(rect_albumListCell.x, rect_albumListCell.y + num * 39, rect_albumListCell.w, rect_albumListCell.h, rect_albumListCell.r);
			if(rect.leftClicked)
			{
				if (selectedMusic_num != i) { albumList[selectedMusic_num].music.stop(); }
				selectedMusic_num = i;
				selectedAlbumName = albumName;
				selectedMusicName = music.originName;
				selectedMusic = music.music;
				set_stillFlag(true);
				SceneMgr_ChangeScene(Scene_Music);
			}
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
		if (albumList_begin > 0)
		{
			goUp.draw((goUp.mouseOver ? Palette::Orange : Palette::White));
			goUp.drawFrame(2, Palette::Black);
		}
		if (albumList_begin + 5 < (signed)albumList.size())
		{
			goDown.draw((goDown.mouseOver ? Palette::Orange : Palette::White));
			goDown.drawFrame(2, Palette::Black);
		}
		for (int i = 0; i < 5; ++i)
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
		{
			RasterizerState rasterizer = RasterizerState::Default2D;
			rasterizer.scissorEnable = true;
			Graphics2D::SetRasterizerState(rasterizer);
			Graphics2D::SetScissorRect(Rect((int)rect_albumName.x, (int)rect_albumName.y, (int)rect_albumName.w, (int)rect_albumName.h));
			font_albumName(albumName).draw(draw_albumName_x, 27 + BAR_HEIGHT);
			Graphics2D::SetScissorRect(Rect((int)rect_albumCreator.x, (int)rect_albumCreator.y, (int)rect_albumCreator.w, (int)rect_albumCreator.h));
			font_albumCreator(albumCreator).draw(draw_albumCreator_x, 88 + BAR_HEIGHT);
			Graphics2D::SetScissorRect(Rect(0, 0, Window::Width(), Window::Height()));
		}
		albumExpl_Draw();
	}

	// 曲リスト 描画
	{
		for (int i = albumList_begin; (i - albumList_begin) < Min<int>(5, (int)albumList.size() - albumList_begin); ++i)
		{
			auto num = i - albumList_begin;
			auto tmp = albumList[i];
			RoundRect tmpRRect(rect_albumList_Flag.x, rect_albumList_Flag.y + num * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r);
			if (tmp.music.isPlaying()) { pausing.drawAt(43, 318 + BAR_HEIGHT + num * 39, (tmpRRect.mouseOver ? Palette::Orange : Palette::White)); }
			else { playing.drawAt(43, 318 + BAR_HEIGHT + num * 39, (tmpRRect.mouseOver ? Palette::Orange : Palette::White)); }
			font_albumList(tmp.displayName).draw(70, 304 + BAR_HEIGHT + num * 39);
			auto str = Format(Pad(tmp.totalTime / 60, { 2,L'0' }), L":", Pad(tmp.totalTime % 60, { 2,L'0' }));
			font_albumList(str).draw(610, 304 + BAR_HEIGHT + num * 39);
			tmpRRect = RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + num * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r);
			((isFav(albumName, tmp.originName) || tmpRRect.mouseOver) ? fav : not_fav).drawAt(725, 318 + BAR_HEIGHT + num * 39);
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
		for (int i = 0; i + pos < albumExpl.length; ++i)
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

	auto rect = rect_albumExpl;
	auto height = texts.size()*font_albumExpl.height;
	if (height > rect.h)
	{
		if (!draw_albumExpl_stayFlag)
		{
			if (draw_albumExpl_y + height > rect.y + rect.h) { draw_albumExpl_y -= 0.5; }
			else
			{
				draw_albumExpl_startMSec = draw_albumExpl_stayMSec = Time::GetMillisec64();
				draw_albumExpl_stayFlag = true;
			}
		}
		if (draw_albumExpl_stayFlag)
		{
			if (draw_albumExpl_stayMSec - draw_albumExpl_startMSec >= DRAW_STAYMSEC)
			{
				draw_albumExpl_startMSec = draw_albumExpl_stayMSec;
				if (draw_albumExpl_y == DEFAULT_albumExpl_Y) { draw_albumExpl_stayFlag = false; }
				else { draw_albumExpl_y = DEFAULT_albumExpl_Y; }
			}
			else { draw_albumExpl_stayMSec = Time::GetMillisec64(); }
		}
	}
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(Rect((int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h));
	for (size_t i = 0; i < texts.size(); ++i)
	{
		const int32 y = static_cast<int32>(draw_albumExpl_y + i * font_albumExpl.height);
		font_albumExpl(texts[i]).draw(rect_albumExpl.x + 10, y);
	}
	Graphics2D::SetScissorRect(Rect(0, 0, Window::Width(), Window::Height()));
}

// アルバム・曲情報 受け渡し（flag == 1 -> 次 : -1 -> 前）
void setAlbumMusicName(String& album_Name, String& musicName, Sound& musicData)
{
	album_Name = selectedAlbumName;
	musicName = selectedMusicName;
	musicData = selectedMusic;
}
void setAlbumMusicName(int flag, String& album_Name, String& musicName, Sound& music)
{
	selectedMusic_num = (selectedMusic_num + flag + (int)albumList.size()) % (int)albumList.size();
	const auto data = albumList[selectedMusic_num];
	album_Name = selectedAlbumName;
	musicName = data.originName;
	music = data.music;
}

// 各文字列 描画
void Update_drawAlbumDetailStrings()
{
	auto rect = rect_albumName;
	auto width = font_albumName(albumName).region().w + rect.r;
	if (width > rect_albumName.w)
	{
		if (!draw_albumName_stayFlag)
		{
			if (draw_albumName_x + width > rect.x + rect.w) { --draw_albumName_x; }
			else
			{
				draw_albumName_startMSec = draw_albumName_stayMSec = Time::GetMillisec64();
				draw_albumName_stayFlag = true;
			}
		}
		if (draw_albumName_stayFlag)
		{
			if (draw_albumName_stayMSec - draw_albumName_startMSec >= DRAW_STAYMSEC)
			{
				draw_albumName_startMSec = draw_albumName_stayMSec;
				if (draw_albumName_x == DEFAULT_albumName_X) { draw_albumName_stayFlag = false; }
				else { draw_albumName_x = DEFAULT_albumName_X; }
			}
			else { draw_albumName_stayMSec = Time::GetMillisec64(); }
		}
	}
	rect = rect_albumCreator;
	width = font_albumCreator(albumCreator).region().w + rect.r;
	if (width > rect_albumCreator.w)
	{
		if (!draw_albumCreator_stayFlag)
		{
			if (draw_albumCreator_x + width > rect.x + rect.w) { --draw_albumCreator_x; }
			else
			{
				draw_albumCreator_startMSec = draw_albumCreator_stayMSec = Time::GetMillisec64();
				draw_albumCreator_stayFlag = true;
			}
		}
		if (draw_albumCreator_stayFlag)
		{
			if (draw_albumCreator_stayMSec - draw_albumCreator_startMSec >= DRAW_STAYMSEC)
			{
				draw_albumCreator_startMSec = draw_albumCreator_stayMSec;
				if (draw_albumCreator_x == DEFAULT_albumCreator_X) { draw_albumCreator_stayFlag = false; }
				else { draw_albumCreator_x = DEFAULT_albumCreator_X; }
			}
			else { draw_albumCreator_stayMSec = Time::GetMillisec64(); }
		}
	}
}

// 曲名短縮
String Detail_musicNameBeShort(String text)
{
	static const String dots(L"...");
	const double dotsWidth = font_albumList(dots).region().w;
	const size_t num_chars = font_albumList.drawableCharacters(text, rect_albumList_Name.w - dotsWidth);

	if (font_albumList(text).region().w <= rect_albumList_Name.w) { return text; }
	if (dotsWidth > rect_albumList_Name.w) { return String(); }
	return text.substr(0, num_chars) + dots;
}

// 曲操作
// kind: 0->一時停止, 1->再生
void setMusicStats(int kind)
{
	switch (kind)
	{
	case 0:
		albumList[selectedMusic_num].music.pause();
		break;
	case 1:
		albumList[selectedMusic_num].music.play();
		break;
	}
}