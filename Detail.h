#pragma once

// アルバム詳細 初期化
void Detail_Init();

// アルバム詳細 更新
void Detail_Update();

// アルバム詳細 描画
void Detail_Draw();

// アルバム説明 描画
void albumExpl_Draw();

// アルバム・曲情報 受け渡し
void setAlbumMusicName(String& album_Name, String& musicName, Texture& album_Img, Sound& music);