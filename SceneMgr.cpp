// include
#include "SceneMgr.h"
#include "Select.h"
#include "Detail.h"
#include "Music.h"

// �O���[�o���ϐ�
static Scene_S Scene = Scene_Select;
static Scene_S prevScene = Scene_Select;

// �X�V
void SceneMgr_Update()
{
	switch (Scene)
	{
	case Scene_Select:
		Select_Update();
		break;

	case Scene_Detail:
		Detail_Update();
		break;

	case Scene_Music:
		Music_Update();
		break;
	}
}

// �`��
void SceneMgr_Draw()
{
	switch (Scene)
	{
	case Scene_Select:
		Select_Draw();
		break;

	case Scene_Detail:
		Detail_Draw();
		break;

	case Scene_Music:
		Music_Draw();
		break;
	}
}

// �V�[���ύX
void SceneMgr_ChangeScene(Scene_S nextScene)
{
	prevScene = Scene;
	Scene = nextScene;
	switch (Scene)
	{
	case Scene_Select:
		Select_Init();
		break;

	case Scene_Detail:
		Detail_Init();
		break;

	case Scene_Music:
		Music_Init();
		break;
	}
}

// �O�̃V�[�����擾
Scene_S get_prevScene()
{
	return prevScene;
}

// ���̃V�[�����擾
Scene_S get_nowScene()
{
	return Scene;
}