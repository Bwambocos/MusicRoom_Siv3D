#pragma once

// define
#define BAR_HEIGHT 64

// �o�[ ������
void Bar_Init();

// �o�[ �X�V
void Bar_Update();

// �o�[ �`��
void Bar_Draw();

// �Đ������擾
bool is_nowMusicPlaying();

// �ȏڍ׃f�[�^�󂯓n��
void giveMusicData(String albumName, String musicName, Sound musicData);

// ��~�t���O �ݒ�
void set_stopFlag(bool flag);