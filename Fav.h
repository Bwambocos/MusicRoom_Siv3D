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
void addFav(String albumName, String musicName, String fileName, Sound music);

// ���C�ɓ��肩��폜����
void removeFav(String albumName, String musicName);

// �ȏ�� �󂯓n���iflag == 1 -> �� : -1 -> �O�j
void setFavMusicName(String& album_Name, String& musicName, Sound& music);
void setFavMusicName(int flag, String& album_Name, String& musicName, Sound& music);

// �Ȗ��Z�k
String Fav_musicNameBeShort(String text);