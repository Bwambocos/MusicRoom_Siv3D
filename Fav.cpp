// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Fav.h"

// �ȃ��X�g �\����
struct List
{
	bool isPlaying;
	Sound music;
	String albumName, musicName;
	int32_t totalTime;
	bool isFav;
};

// �O���[�o���ϐ�
static std::vector<List>musics;

// ���C�ɓ��� ������
void Fav_Init()
{

}

// ���C�ɓ��� �X�V
void Fav_Update()
{

}

// ���C�ɓ��� �`��
void Fav_Draw()
{

}

// ���C�ɓ��肩�m�F����
bool isFav(String albumName, String musicName)
{
	for (auto i : musics)
	{
		if (i.albumName == albumName && i.musicName == musicName) { return true; }
	}
	return false;
}

// ���C�ɓ���ɒǉ�����
void addFav(String albumName, String musicName)
{
	
}

// ���C�ɓ��肩��폜����
void removeFav(String albumName, String musicName)
{

}