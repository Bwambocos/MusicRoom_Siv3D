// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Bar.h"
#include "Fav.h"
#include "Music.h"

// define
#define MAX_CELL_NUM 12

// 曲リスト 構造体
struct List_fav
{
	Sound music;
	String albumName;
	String musicDisplayName;
	String musicOriginName;
	int totalTime;
};

// グローバル変数
static std::vector<List_fav>musics;
static Texture main, playing, pausing, not_fav, fav;
static Font font_albumList;
static String selectedAlbumName = L"", selectedMusicName = L"";
static RoundRect rect_albumList_Flag(25, 25 + BAR_HEIGHT, 36, 36, 5);
static RoundRect rect_albumList_Name(64, 25 + BAR_HEIGHT, 537, 36, 5);
static RoundRect rect_albumList_Time(604, 25 + BAR_HEIGHT, 100, 36, 5);
static RoundRect rect_albumList_Fav(707, 25 + BAR_HEIGHT, 36, 36, 5);
static RoundRect rect_albumListAll(25, 25 + BAR_HEIGHT, 718, 456, 5);
static RoundRect rect_albumListCell(64, 25 + BAR_HEIGHT, 582, 36, 5);
static Triangle goUp({ 384,75 }, { 414,85 }, { 354,85 });
static Triangle goDown({ 354,560 }, { 414,560 }, { 384,570 });
static Sound selectedMusic;
static int albumList_begin;
static int selectedMusic_num;

// お気に入り 初期化
void Fav_Init()
{
	if (!main)
	{
		main = Texture(L"data\\Fav\\main.png");
		playing = Texture(L"data\\Detail\\playing.png");
		pausing = Texture(L"data\\Detail\\pausing.png");
		not_fav = Texture(L"data\\Detail\\not_fav.png");
		fav = Texture(L"data\\Detail\\fav.png");
		font_albumList = Font(16);
	}
}

// お気に入り 更新
void Fav_Update()
{
	if (Input::KeyB.pressed) { SceneMgr_ChangeScene(Scene_Select); }

	// 曲リスト 更新
	{
		if (goUp.leftClicked) { --albumList_begin; }
		if (goDown.leftClicked) { ++albumList_begin; }
		if (rect_albumListAll.mouseOver)
		{
			bool scr_flag = ((albumList_begin + MAX_CELL_NUM <= (signed)musics.size()) || (albumList_begin > 0) ? true : false);
			if (scr_flag) { albumList_begin += Mouse::Wheel(); }
		}
		albumList_begin = Max(albumList_begin, 0);
		albumList_begin = Min<int>(albumList_begin, Max<int>((int)musics.size() - MAX_CELL_NUM, 0));

		for (int i = albumList_begin; ((i - albumList_begin) < Min<int>(MAX_CELL_NUM, (signed)musics.size())) && (i < (signed)musics.size()); ++i)
		{
			auto num = i - albumList_begin;
			auto music = musics[i];
			RoundRect rect(rect_albumList_Flag.x, rect_albumList_Flag.y + num * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r);
			if (rect.leftClicked)
			{
				(music.music.isPlaying() ? music.music.pause() : music.music.play());
				if (selectedMusic_num != i) { musics[selectedMusic_num].music.stop(); }
				selectedMusic_num = i;
				selectedAlbumName = music.albumName;
				selectedMusicName = music.musicOriginName;
				selectedMusic = music.music;
				giveMusicData(music.albumName, music.musicOriginName, music.music);
			}
			rect = RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + num * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r);
			if (rect.leftClicked)
			{
				(isFav(music.albumName, music.musicOriginName) ? removeFav(music.albumName, music.musicOriginName) : addFav(music.albumName, music.musicDisplayName, music.musicOriginName, music.music));
			}
			rect = RoundRect(rect_albumListCell.x, rect_albumListCell.y + num * 39, rect_albumListCell.w, rect_albumListCell.h, rect_albumListCell.r);
			if (rect.leftClicked)
			{
				if (selectedMusic_num != i) { musics[selectedMusic_num].music.stop(); }
				selectedMusic_num = i;
				selectedAlbumName = music.albumName;
				selectedMusicName = music.musicOriginName;
				selectedMusic = music.music;
				set_stillFlag(true);
				SceneMgr_ChangeScene(Scene_Music);
			}
		}
	}
}

// お気に入り 描画
void Fav_Draw()
{
	main.draw(0, BAR_HEIGHT);

	// 曲リスト 描画
	{
		if (albumList_begin > 0)
		{
			goUp.draw((goUp.mouseOver ? Palette::Orange : Palette::White));
			goUp.drawFrame(2, Palette::Black);
		}
		if (albumList_begin + MAX_CELL_NUM < (signed)musics.size())
		{
			goDown.draw((goDown.mouseOver ? Palette::Orange : Palette::White));
			goDown.drawFrame(2, Palette::Black);
		}
		for (int i = 0; i < MAX_CELL_NUM; ++i)
		{
			RoundRect(rect_albumList_Flag.x, rect_albumList_Flag.y + i * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r).draw(Color(32, 32, 32, 200));
			RoundRect(rect_albumList_Name.x, rect_albumList_Name.y + i * 39, rect_albumList_Name.w, rect_albumList_Name.h, rect_albumList_Name.r).draw(Color(32, 32, 32, 200));
			RoundRect(rect_albumList_Time.x, rect_albumList_Time.y + i * 39, rect_albumList_Time.w, rect_albumList_Time.h, rect_albumList_Time.r).draw(Color(32, 32, 32, 200));
			RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + i * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r).draw(Color(32, 32, 32, 200));
		}
		for (int i = albumList_begin; (i - albumList_begin) < Min<int>(MAX_CELL_NUM, (int)musics.size() - albumList_begin); ++i)
		{
			auto num = i - albumList_begin;
			auto tmp = musics[i];
			RoundRect tmpRRect(rect_albumList_Flag.x, rect_albumList_Flag.y + num * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r);
			if (tmp.music.isPlaying()) { pausing.drawAt(43, 43 + BAR_HEIGHT + num * 39, (tmpRRect.mouseOver ? Palette::Orange : Palette::White)); }
			else { playing.drawAt(43, 43 + BAR_HEIGHT + num * 39, (tmpRRect.mouseOver ? Palette::Orange : Palette::White)); }
			font_albumList(tmp.musicDisplayName).draw(70, 29 + BAR_HEIGHT + num * 39);
			auto str = Format(Pad(tmp.totalTime / 60, { 2,L'0' }), L":", Pad(tmp.totalTime % 60, { 2,L'0' }));
			font_albumList(str).draw(610, 29 + BAR_HEIGHT + num * 39);
			tmpRRect = RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + num * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r);
			((isFav(tmp.albumName, tmp.musicOriginName) || tmpRRect.mouseOver) ? fav : not_fav).drawAt(725, 43 + BAR_HEIGHT + num * 39);
		}
	}
}

// お気に入りか確認する
bool isFav(String albumName, String musicName)
{
	for (auto i : musics)
	{
		if (i.albumName == albumName && i.musicOriginName == musicName) { return true; }
	}
	return false;
}

// お気に入りに追加する
void addFav(String albumName, String musicName, String fileName, Sound music)
{
	auto temp_totalTime = (int)music.lengthSec();
	musics.push_back({ music,albumName,Fav_musicNameBeShort(musicName),fileName,temp_totalTime });
}

// お気に入りから削除する
void removeFav(String albumName, String musicName)
{
	for (int i = 0; i < (signed)musics.size(); ++i)
	{
		if (musics[i].albumName == albumName && musics[i].musicOriginName == musicName)
		{
			musics.erase(musics.begin() + i);
			break;
		}
	}
}

// 曲情報 受け渡し
void setFavMusicName(String& album_Name, String& musicName, Sound& music)
{
	album_Name = selectedAlbumName;
	musicName = selectedMusicName;
	music = selectedMusic;
}
void setFavMusicName(int flag, String& album_Name, String& musicName, Sound& music)
{
	selectedMusic_num = (selectedMusic_num + flag + (int)musics.size()) % (int)musics.size();
	const auto data = musics[selectedMusic_num];
	album_Name = data.albumName;
	musicName = data.musicOriginName;
	music = data.music;
}

// 曲名短縮
String Fav_musicNameBeShort(String text)
{
	static const String dots(L"...");
	const double dotsWidth = font_albumList(dots).region().w;
	const size_t num_chars = font_albumList.drawableCharacters(text, rect_albumList_Name.w - dotsWidth);

	if (font_albumList(text).region().w <= rect_albumList_Name.w) { return text; }
	if (dotsWidth > rect_albumList_Name.w) { return String(); }
	return text.substr(0, num_chars) + dots;
}