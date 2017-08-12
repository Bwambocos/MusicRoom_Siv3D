#pragma once

// define
#define BAR_HEIGHT 64

// バー 初期化
void Bar_Init();

// バー 更新
void Bar_Update();

// バー 描画
void Bar_Draw();

// 再生中か取得
bool is_nowMusicPlaying();

// 曲詳細データ受け渡し
void giveMusicData(String albumName, String musicName, Sound musicData);

// 停止フラグ 設定
void set_stopFlag(bool flag);