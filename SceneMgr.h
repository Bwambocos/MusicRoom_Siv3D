#pragma once

// �V�[���\����
typedef enum
{
	Scene_Select,
	Scene_Detail,
	Scene_Music,
}Scene_S;

// �X�V
void SceneMgr_Update();

// �`��
void SceneMgr_Draw();

// �V�[���ύX
void SceneMgr_ChangeScene(Scene_S);

// �O�̃V�[�����擾
Scene_S get_prevScene();

// ���̃V�[�����擾
Scene_S get_nowScene();