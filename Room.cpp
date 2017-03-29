// include
#include <Siv3D.hpp>
#include "Room.h"

// define
#define MAX_LOAD_MUSIC		50

// 定数
static const int DRAW_X_START_POINT = -75;						// 画面外から登場させる
static const int DRAW_HEIGHT = 200;								// 画像の高さ
static const int DRAW_WIDTH_S = 50;								// 画像の幅（小さいほう）
static const int DRAW_WIDTH_L = 200;							// 画像の幅（大きいほう）
static const int DRAW_Y_TOP = 25;								// yの変形（上）
static const int DRAW_BOTTOM = DRAW_Y_TOP + DRAW_HEIGHT;		// yの変形（下）
static const int Y_DEFAULT_DIFF = 50;							// yの沈み込み度
static const int DRAW_X_DISTANCE = 50;							// 画像と次の画像のx座標の間隔
static const int DRAW_TIME_COST = 500;							// 何ミリ秒かけて画像の移動・変形を行うか
static const int MUSIC_PLAYPOS_CHANGE_LENGTH = 5000;			// 早送りを何ミリ秒行うか																// 構造体

// 構造体
struct Music_data
{
	String Name;
	Texture image;
	String Creator;
	String Comment;
	Sound sound;
};

// 変数
Music_data music[MAX_LOAD_MUSIC];
Texture G_main, G_mainFrame;
Texture G_button[11], G_updown[2];
Texture G_noimage, G_nowloading, G_imageFrame[2];
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

// 初期化
void Room_Init()
{

}

// 更新
void Room_Update()
{

}

// 描画
void Room_Draw()
{

}