// include
#include <Siv3D.hpp>
#pragma once

// �A���o���I�� ������
void Select_Init();

// �A���o���I�� �X�V
void Select_Update();

// �A���o���I�� �`��
void Select_Draw();

// �A���o���ꗗ �����`�����쐬
Rect MakeRect(int x, int y);

// �A���o���摜��Ԃ�
Texture SelectImage(int cou);

// �I�����ꂽ�A���o������Ԃ�
String getSetAlbum();

// ���̃A���o����Ԃ�
void getNextAlbum();

// �A���o���ڍ� �`��
void DrawDetails(int cou);