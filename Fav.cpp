// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Fav.h"

// 曲リスト 構造体
struct List_fav
{
	Sound music;
	String albumName;
	String musicName;
	int32_t totalTime;
};

// グローバル変数
static std::vector<List_fav>musics;

// お気に入り 初期化
void Fav_Init()
{

}

// お気に入り 更新
void Fav_Update()
{

}

// お気に入り 描画
void Fav_Draw()
{

}

// お気に入りか確認する
bool isFav(String albumName, String musicName)
{
	for (auto i : musics)
	{
		if (i.albumName == albumName && i.musicName == musicName) { return true; }
	}
	return false;
}

// お気に入りに追加する
void addFav(String albumName, String musicName)
{
	const String extensions[] = { L".wav",L".ogg",L".mp3" };
	String tempName; Sound tempMusic; int32_t temp_totalTime;
	for (auto ext : extensions)
	{
		if (FileSystem::IsFile(L"music\\" + albumName + L"\\" + musicName + L"\\" + musicName + ext))
		{
			tempMusic = Sound(L"music\\" + albumName + L"\\" + musicName + L"\\" + musicName + L".mp3");
			break;
		}
	}
	temp_totalTime = (int32_t)tempMusic.lengthSample();
	musics.push_back({ tempMusic,albumName,musicName,temp_totalTime });
}

// お気に入りから削除する
void removeFav(String albumName, String musicName)
{
	for (int32_t i = 0; i < (signed)musics.size(); ++i)
	{
		if (musics[i].albumName == albumName && musics[i].musicName == musicName)
		{
			musics.erase(musics.begin() + i);
			break;
		}
	}
}