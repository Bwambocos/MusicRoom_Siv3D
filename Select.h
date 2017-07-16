#include <Siv3D.hpp>
#pragma once

// アルバム選択 初期化
void Select_Init();

// アルバム選択 更新
void Select_Update();

// アルバム選択 描画
void Select_Draw();

Rect MakeRect(int32_t x, int32_t y);

// 選択されたアルバム名を返す
String getSetAlbum();

// 選択されたアルバム画像を返す
Texture SelectImage(int32_t cou);

// アルバム詳細 描画
void DrawDetails(int32_t cou);