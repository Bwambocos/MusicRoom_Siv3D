#pragma once

// ���C�ɓ��� ������
void Fav_Init();

// ���C�ɓ��� �X�V
void Fav_Update();

// ���C�ɓ��� �`��
void Fav_Draw();

// ���C�ɓ��肩�m�F����
bool isFav(String albumName, String musicName);

// ���C�ɓ���ɒǉ�����
void addFav(String albumName, String musicName, Sound music);

// ���C�ɓ��肩��폜����
void removeFav(String albumName, String musicName);

// �ȏ�� �󂯓n��
void setFavMusicName(String& album_Name, String& musicName, Sound& music);

// �Ȗ��Z�k
String Fav_musicNameBeShort(String text);