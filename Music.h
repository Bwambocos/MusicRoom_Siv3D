#pragma once

// �� ������
void Music_Init();

// �� �X�V
void Music_Update();

// �� �`��
void Music_Draw();

// �Ȑ��� �`��
void musicExpl_Draw();

// �Ȗ��`��ʒu �X�V
void Update_drawMusicDetailStrings();

// �Ȏ蓮�J��
void changeMusic(int flag);

// �ȑ���
// kind: 0->�ꎞ��~, 1->�Đ�, 2->��~, 3->�J��Ԃ��؂�ւ�
void changeMusicStats(int kind);

// ����ʒ��t���O�Z�b�g
void set_stillFlag(bool flag);

// ���C�ɓ��胋�[�v�����ۂ�
bool isFavLooping();