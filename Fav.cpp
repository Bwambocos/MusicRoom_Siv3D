// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Fav.h"

// 曲リスト 構造体
struct List
{
	bool isPlaying;
	Sound music;
	String albumName, musicName;
	int32_t totalTime;
	bool isFav;
};

// グローバル変数
static std::vector<List>musics;

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
	
}

// お気に入りから削除する
void removeFav(String albumName, String musicName)
{

}