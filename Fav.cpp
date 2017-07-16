// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Fav.h"

// �ȃ��X�g �\����
struct List_fav
{
	Sound music;
	String albumName;
	String musicName;
	int32_t totalTime;
};

// �O���[�o���ϐ�
static std::vector<List_fav>musics;

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
	const String extensions[] = { L".wav",L".ogg",L".mp3" };
	String tempName; Sound tempMusic; int32_t temp_totalTime;
	for (auto ext : extensions)
	{
		if (FileSystem::IsFile(L"music\\" + albumName + L"\\" + musicName + L"\\" + musicName + ext))
		{
			tempMusic = Sound(L"music\\" + albumName + L"\\" + musicName + L"\\" + musicName + L".mp3");
			break;
		}
	}
	temp_totalTime = (int32_t)tempMusic.lengthSample();
	musics.push_back({ tempMusic,albumName,musicName,temp_totalTime });
}

// ���C�ɓ��肩��폜����
void removeFav(String albumName, String musicName)
{
	for (int32_t i = 0; i < (signed)musics.size(); ++i)
	{
		if (musics[i].albumName == albumName && musics[i].musicName == musicName)
		{
			musics.erase(musics.begin() + i);
			break;
		}
	}
}