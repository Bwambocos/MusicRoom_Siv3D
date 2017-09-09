#pragma once

// お気に入り 初期化
void Fav_Init();

// お気に入り 更新
void Fav_Update();

// お気に入り 描画
void Fav_Draw();

// お気に入りか確認する
bool isFav(String albumName, String musicName);

// お気に入りに追加する
void addFav(String albumName, String musicName, String fileName, Sound music);

// お気に入りから削除する
void removeFav(String albumName, String musicName);

// 曲情報 受け渡し（flag == 1 -> 次 : -1 -> 前）
void setFavMusicName(String& album_Name, String& musicName, Sound& music);
void setFavMusicName(int flag, String& album_Name, String& musicName, Sound& music);

// 曲名短縮
String Fav_musicNameBeShort(String text);