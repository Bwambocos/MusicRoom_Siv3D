// include
#include <Siv3D.hpp>
#pragma once

// アルバム選択 初期化
void Select_Init();

// アルバム選択 更新
void Select_Update();

// アルバム選択 描画
void Select_Draw();

// アルバム一覧 正方形区画を作成
Rect MakeRect(int x, int y);

// アルバム画像を返す
Texture SelectImage(int cou);

// 選択されたアルバム名を返す
String getSetAlbum();

// 次のアルバムを返す
void getNextAlbum();

// アルバム詳細 描画
void DrawDetails(int cou);