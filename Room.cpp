// include
#include <Siv3D.hpp>
#include "Room.h"

// define
#define MAX_LOAD_MUSIC		50

// �萔
static const int DRAW_X_START_POINT = -75;						// ��ʊO����o�ꂳ����
static const int DRAW_HEIGHT = 200;								// �摜�̍���
static const int DRAW_WIDTH_S = 50;								// �摜�̕��i�������ق��j
static const int DRAW_WIDTH_L = 200;							// �摜�̕��i�傫���ق��j
static const int DRAW_Y_TOP = 25;								// y�̕ό`�i��j
static const int DRAW_BOTTOM = DRAW_Y_TOP + DRAW_HEIGHT;		// y�̕ό`�i���j
static const int Y_DEFAULT_DIFF = 50;							// y�̒��ݍ��ݓx
static const int DRAW_X_DISTANCE = 50;							// �摜�Ǝ��̉摜��x���W�̊Ԋu
static const int DRAW_TIME_COST = 500;							// ���~���b�����ĉ摜�̈ړ��E�ό`���s����
static const int MUSIC_PLAYPOS_CHANGE_LENGTH = 5000;			// ����������~���b�s����																// �\����

// �\����
struct Music_data
{
	String Name;
	Texture image;
	String Creator;
	String Comment;
	Sound sound;
};

// �ϐ�
Music_data music[MAX_LOAD_MUSIC];
Texture G_main, G_mainFrame;
Texture G_button[11], G_updown[2];
Texture G_noimage, G_nowloading;
Font F_Name, F_Creator, F_Comment;
int32 totalMusicNum = -1, nowMusicNum, changeMusicNum = -1;
int32 firstTime, nowTime;
int32 image_frametime, image_lasttime, image_starttime;
int32 musicName_frametime, musicName_lasttime, musicName_starttime;
int32 musicCreator_frametime, musicCreator_lasttime, musicCreator_starttime;
bool changeImage_flag = false;
int32 changeImage_for;
int32 Music_totalTime = -1, Music_nowTime = -1;
int32 draw_musicName_x = 30, draw_musicCreator_x = 400;
int32 now_musicComment_limitY;
bool Is_musicComment_Y_over = false;
bool Is_mouse_on_musicImage[MAX_LOAD_MUSIC];
bool Is_music_loop = false, Is_music_changed_by_end = false;

// ������
void Room_Init()
{
	String Name;
	TextReader FP_music_list(L"music\\music_list.txt");
	for (int32 i = 0; i < MAX_LOAD_MUSIC; ++i)
	{
		Name[0] = L'\0';
		FP_music_list.readLine(Name);
		if (Name[0] == '\0')
		{
			totalMusicNum = i;
			break;
		}
		music[i].image = Texture(L"music\\" + Name + L"\\" + Name + L".png");
		if (!music[i].image) { music[i].image = G_noimage; }
		TextReader FP_comment(L"music\\" + Name + L"\\" + Name + L".txt");
		FP_comment.readLine(music[i].Name);
		FP_comment.readLine(music[i].Creator);
		String temp;
		while (FP_comment.readLine(temp)) { music[i].Comment += temp; }
		const String extensions[] = { L".wav",L".ogg",L".mp3" };
		for (auto ext : extensions)
		{
			if (FileSystem::IsFile(L"music\\" + Name + L"\\" + Name + ext))
			{
				music[i].sound = Sound(L"music\\" + Name + L"\\" + Name + ext);
				break;
			}
		}
	}
	totalMusicNum = (totalMusicNum == -1 ? MAX_LOAD_MUSIC : totalMusicNum);
	G_main = Texture(L"data\\main.png");
	G_mainFrame = Texture(L"data\\main_frame.png");
	G_nowloading = Texture(L"data\\nowloading.png");
	G_noimage = Texture(L"data\\noimage.png");
	G_button[0] = Texture(L"data\\button\\play\\normal.png");
	G_button[1] = Texture(L"data\\button\\stop\\normal.png");
	G_button[2] = Texture(L"data\\button\\brief\\normal.png");
	G_button[3] = Texture(L"data\\button\\play\\active.png");
	G_button[4] = Texture(L"data\\button\\stop\\active.png");
	G_button[5] = Texture(L"data\\button\\brief\\active.png");
	G_button[6] = Texture(L"data\\button\\seek\\active.png");
	G_button[7] = Texture(L"data\\button\\seek\\normal.png");
	G_button[8] = Texture(L"data\\button\\rep\\normal.png");
	G_button[9] = Texture(L"data\\button\\rep\\active.png");
	G_button[10] = Texture(L"data\\button\\time.png");
	G_updown[0] = Texture(L"data\\button\\up.png");
	G_updown[1] = Texture(L"data\\button\\down.png");
	F_Name = Font(32, L"���C���I");
	F_Creator = Font(28, L"���C���I");
	F_Comment = Font(24, L"Meiryo UI");
	firstTime = Time::GetMillisec();
}

// �X�V
void Room_Update()
{

}

// �`��
void Room_Draw()
{

}