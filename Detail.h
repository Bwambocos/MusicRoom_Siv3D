#pragma once

// �A���o���ڍ� ������
void Detail_Init();

// �A���o���ڍ� �X�V
void Detail_Update();

// �A���o���ڍ� �`��
void Detail_Draw();

// �A���o������ �`��
void albumExpl_Draw();

// �A���o���E�ȏ�� �󂯓n���iflag == 1 -> �� : -1 -> �O�j
void setAlbumMusicName(String& album_Name, String& musicName, Sound& music);
void setAlbumMusicName(int flag, String& album_Name, String& musicName, Sound& music);

// ���O�`��ʒu �X�V
void Update_drawAlbumDetailStrings();

// �Ȗ��Z�k
String Detail_musicNameBeShort(String text);