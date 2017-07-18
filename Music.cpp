// include
#include <Siv3D.hpp>
#include "Music.h"
#include "Bar.h"
#include "Detail.h"

// �O���[�o���ϐ�
static Texture music_albumImg;
static String music_albumName = L"", music_musicName = L"", music_musicExp = L"";

// �� ������
void Music_Init()
{
	setAlbumMusicName(music_albumName, music_musicName, music_albumImg);
	TextReader music_reader(L"music\\" + music_albumName + L"\\" + music_musicName + L"\\" + music_musicName + L".txt");
	music_reader.readAll(music_musicExp);
}

// �� �X�V
void Music_Update()
{

}

// �� �`��
void Music_Draw()
{
	Rect(300, 300)(music_albumImg).drawAt(Window::Center().x, Window::Center().y + BAR_HEIGHT);
}