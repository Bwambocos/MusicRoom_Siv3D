#include <Siv3D.hpp>
#pragma once

// �A���o���I�� ������
void Select_Init();

// �A���o���I�� �X�V
void Select_Update();

// �A���o���I�� �`��
void Select_Draw();

Rect MakeRect(int32_t x, int32_t y);

// �I�����ꂽ�A���o������Ԃ�
String getSetAlbum();

// �I�����ꂽ�A���o���摜��Ԃ�
Texture SelectImage(int32_t cou);

// �A���o���ڍ� �`��
void DrawDetails(int32_t cou);