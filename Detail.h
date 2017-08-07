#pragma once

// アルバム詳細 初期化
void Detail_Init();

// アルバム詳細 更新
void Detail_Update();

// アルバム詳細 描画
void Detail_Draw();

// アルバム説明 描画
void albumExpl_Draw();

// アルバム・曲情報 受け渡し（flag == 1 -> 次 : -1 -> 前）
void setAlbumMusicName(String& album_Name, String& musicName, Sound& music);
void setAlbumMusicName(int flag, String& album_Name, String& musicName, Sound& music);

// 名前描画位置 更新
void Update_drawAlbumDetailStrings();

// 曲名短縮
String Detail_musicNameBeShort(String text);