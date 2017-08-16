#pragma once

// 曲 初期化
void Music_Init();

// 曲 更新
void Music_Update();

// 曲 描画
void Music_Draw();

// 曲説明 描画
void musicExpl_Draw();

// 曲名描画位置 更新
void Update_drawMusicDetailStrings();

// 曲手動遷移
void changeMusic(int flag);

// 曲操作
// kind: 0->一時停止, 1->再生, 2->停止, 3->繰り返し切り替え
void changeMusicStats(int kind);

// 他画面中フラグセット
void set_stillFlag(bool flag);

// お気に入りループ中か否か
bool isFavLooping();