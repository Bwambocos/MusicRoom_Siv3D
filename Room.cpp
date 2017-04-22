// include
#include <Siv3D.hpp>
#include "Room.h"
#include "Mouse.h"

// define
#define MAX_LOAD_MUSIC		50

// 定数
static const int32 DRAW_X_START_POINT = -75;						// 画面外から登場させる
static const int32 DRAW_HEIGHT = 200;								// 画像の高さ
static const int32 DRAW_WIDTH_S = 50;								// 画像の幅（小さいほう）
static const int32 DRAW_WIDTH_L = 200;							// 画像の幅（大きいほう）
static const int32 DRAW_Y_TOP = 25;								// yの変形（上）
static const int32 DRAW_BOTTOM = DRAW_Y_TOP + DRAW_HEIGHT;		// yの変形（下）
static const int32 Y_DEFAULT_DIFF = 50;							// yの沈み込み度
static const int32 DRAW_X_DISTANCE = 50;							// 画像と次の画像のx座標の間隔
static const int32 DRAW_TIME_COST = 500;							// 何ミリ秒かけて画像の移動・変形を行うか
static const int32 MUSIC_PLAYPOS_CHANGE_LENGTH = 5000;			// 早送りを何ミリ秒行うか																// 構造体

// 構造体
struct Music_data
{
	String Name;
	Image image;
	String Creator;
	String Comment;
	Sound sound;
};

// 変数
Music_data music[MAX_LOAD_MUSIC];
Texture G_main, G_mainFrame;
Texture G_button[11], G_updown[2];
Image G_noimage;
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
bool Is_music_loop = false, Is_music_changed_by_end = false;
bool render_flagL = true, render_flagR = true;

// 初期化
void Room_Init()
{
	String Name;
	TextReader FP_music_list(L"music\\music_list.txt");
	G_noimage = Image(L"data\\noimage.png");
	for (int32 i = 0; i < MAX_LOAD_MUSIC; ++i)
	{
		Name[0] = L'\0';
		FP_music_list.readLine(Name);
		if (Name[0] == '\0')
		{
			totalMusicNum = i;
			break;
		}
		music[i].image = Image(L"music\\" + Name + L"\\" + Name + L".png");
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
	F_Name = Font(32, L"メイリオ");
	F_Creator = Font(28, L"メイリオ");
	F_Comment = Font(24, L"Meiryo UI");
	firstTime = Time::GetMillisec();
}

// 更新
void Room_Update()
{
	image_lasttime = Time::GetMillisec()&INT_MAX;
	musicName_lasttime = Time::GetMillisec()&INT_MAX;
	musicCreator_lasttime = Time::GetMillisec()&INT_MAX;
	nowTime = Time::GetMillisec() - firstTime;

	// 音楽選択
	if (changeImage_flag == false)
	{
		if (Input::KeyLeft.pressed)
		{
			music[nowMusicNum].sound.stop();
			Music_totalTime = 1;
			Music_nowTime = 0;
			changeImage_for = 1;
			changeImage_flag = true;
			image_starttime = image_lasttime;
			musicName_starttime = musicName_lasttime;
			musicCreator_starttime = musicCreator_lasttime;
		}
		else if (Input::KeyRight.pressed)
		{
			music[nowMusicNum].sound.stop();
			Music_totalTime = 1;
			Music_nowTime = 0;
			changeImage_for = 2;
			changeImage_flag = true;
			image_starttime = image_lasttime;
			musicName_starttime = musicName_lasttime;
			musicCreator_starttime = musicCreator_lasttime;
		}
	}

	// 音楽再生
	PlayMusic_Update();

	// 作者＆コメント（更新）
	CreatorAndComment_Update();

	// クリックで音楽変更
	if (changeImage_flag == false)
	{
		int32 draw_x = DRAW_X_START_POINT;
		for (int32 i = 0; i < 7; ++i)
		{
			if (i < 3)
			{
				if (CheckMouseClick(draw_x - 5, DRAW_Y_TOP - 5, draw_x + DRAW_WIDTH_S + 5, DRAW_Y_TOP + DRAW_HEIGHT + 5))
				{
					changeImage_flag = true;
					changeImage_for = 1;
					changeMusicNum = abs(nowMusicNum + totalMusicNum - (3 - i)) % totalMusicNum;
					music[nowMusicNum].sound.stop();
					Music_totalTime = 1;
					Music_nowTime = 0;
					image_starttime = image_lasttime;
					musicName_starttime = musicName_lasttime;
					musicCreator_starttime = musicCreator_lasttime;
					break;
				}
				draw_x += DRAW_WIDTH_S + DRAW_X_DISTANCE;
			}
			else if (i == 3)
			{
				draw_x += DRAW_WIDTH_L + DRAW_X_DISTANCE;
			}
			else
			{
				if (CheckMouseClick(draw_x - 5, DRAW_Y_TOP - 5, draw_x + DRAW_WIDTH_S + 5, DRAW_Y_TOP + DRAW_HEIGHT + 5) == true)
				{
					changeImage_flag = true;
					changeImage_for = 2;
					changeMusicNum = abs(nowMusicNum + (i - 3)) % totalMusicNum;
					music[nowMusicNum].sound.stop();
					Music_totalTime = 1;
					Music_nowTime = 0;
					image_starttime = image_lasttime;
					musicName_starttime = musicName_lasttime;
					musicCreator_starttime = musicCreator_lasttime;
					break;
				}
				draw_x += DRAW_WIDTH_S + DRAW_X_DISTANCE;
			}
		}

	}
}

int Solve2(double a, double b, double c, double& x1, double& x2)
{
	if (a == 0.0)
	{
		if (c == 0.0)
		{
			return false;
		}

		x1 = -b / c;
		x2 = -b / c;

		return true;
	}

	double dd = b*b - 4.0*a*c;
	x1 = 0.0;
	x2 = 0.0;

	if (dd < 0.0)
	{
		return false;
	}

	x1 = (-b + sqrt(dd)) / (2.0*a);
	x2 = (-b - sqrt(dd)) / (2.0*a);

	return true;
}

void Solve(
	double a, double b, double c, double d,
	double e, double f, double g, double h,
	double& xa, double& ya, double& xb, double& yb
)
{
	double p1 = a*h - f*c;
	double p2 = d*f - c*g + b*h - a*e;
	double p3 = d*g - b*e;

	if (p1 == 0.0)
	{
		xa = d / b;
		ya = (b*e - d*g) / (b*h + d*f);
		xb = -1000;
		yb = -1000;
		return;
	}

	if (!Solve2(p1, p2, p3, ya, yb))
	{
		xa = -1000;
		xb = -1000;
		return;
	}

	double tm = a* ya + b;

	if (tm)
	{
		xa = (d - c * ya) / tm;
	}
	else
	{
		xa = -1000;
	}

	tm = a* yb + b;

	if (tm)
	{
		xb = (d - c * yb) / tm;
	}
	else
	{
		xb = -1000.0;
	}
}

void Transform(const Image& src, Image& dst, const double(&px)[4], const double(&py)[4])
{
	const int rx1 = 0;
	const int ry1 = 0;
	const int rx2 = src.height - 1;
	const int ry2 = src.width - 1;
	double aa = px[2] - px[3] - px[1] + px[0];
	double bb = px[1] - px[0];
	double cc = px[3] - px[0];
	double ff = py[2] - py[3] - py[1] + py[0];
	double gg = py[1] - py[0];
	double hh = py[3] - py[0];

	if (!aa) aa = 0.001;
	if (!ff) ff = 0.001;

	for (int y = 0; y < dst.height; ++y)
	{
		for (int x = 0; x < dst.width; ++x)
		{
			const double xx = x + 0.5;
			const double yy = y + 0.5;

			if (!Quad(px[0], py[0], px[3], py[3], px[2], py[2], px[1], py[1]).intersects(Vec2(xx, yy)))
			{
				continue;
			}

			double s1, t1, s2, t2, tx, ty;
			const double dd = xx - px[0];
			const double ee = yy - py[0];

			Solve(aa, bb, cc, dd, ee, ff, gg, hh, s1, t1, s2, t2);

			if (s1 >= 0 && t1 >= 0 && s1 <= 1.0 && t1 <= 1.0)
			{
				tx = s1*(rx2 - rx1) + rx1;
				ty = t1*(ry2 - ry1) + ry1;
			}
			else
			{
				tx = s2*(rx2 - rx1) + rx1;
				ty = t2*(ry2 - ry1) + ry1;
			}

			if (tx < 0 || ty < 0)
			{
				continue;
			}

			dst[y][x] = src.sample(tx - 0.5, ty - 0.5);
		}
	}
}

void Transform(const Image& src, Image& dst, const Quad& to)
{
	if (&src == &dst)
	{
		return;
	}

	const double xs[4] = { to.tl.x, to.bl.x, to.br.x, to.tr.x };
	const double ys[4] = { to.tl.y, to.bl.y, to.br.y, to.tr.y };

	Transform(src, dst, xs, ys);
}

Image canvas(50, 200);
DynamicTexture render_res;
static void DrawMI_L(Image gr, int32 draw_x, int32 y2_diff = 0, int32 x2_diff = 0, bool flag = false)
{
	int32 Draw_Width = (flag ? DRAW_WIDTH_L : DRAW_WIDTH_S);
	Quad quad({ 0, 0 },
	{ 0 + Draw_Width + x2_diff, 0 + Y_DEFAULT_DIFF - y2_diff },
	{ 0 + Draw_Width + x2_diff, DRAW_BOTTOM - DRAW_Y_TOP - Y_DEFAULT_DIFF + y2_diff },
	{ 0, DRAW_BOTTOM - DRAW_Y_TOP });
	if (render_flagL == true)
	{
		canvas.clear();
		canvas.resize(200, 200);
		Transform(gr, canvas, quad);
		render_res.fill(canvas);
	}
	else { render_res.fill(gr); }
	render_res.draw(draw_x, DRAW_Y_TOP);
}

static void DrawMI_R(Image gr, int32 draw_x, int32 y1_diff = 0, int32 x1_diff = 0, bool flag = false)
{
	int32 Draw_Width = (flag ? DRAW_WIDTH_L : DRAW_WIDTH_S);
	Quad quad({ 0 + x1_diff, 0 + Y_DEFAULT_DIFF - y1_diff },
	{ 0 + Draw_Width, 0 },
	{ 0 + Draw_Width, DRAW_BOTTOM - 0 },
	{ 0 + x1_diff, DRAW_BOTTOM - Y_DEFAULT_DIFF + y1_diff - DRAW_Y_TOP });
	if (render_flagR == true)
	{
		canvas.clear();
		canvas.resize(200, 200);
		Transform(gr, canvas, quad);
		render_res.fill(canvas);
	}
	else { render_res.fill(gr); }
	render_res.draw(draw_x + x1_diff, DRAW_Y_TOP);
}

// 描画
void Room_Draw()
{
	// 画像
	G_main.draw();
	if (changeImage_flag == true || (changeMusicNum != -1 && changeMusicNum != nowMusicNum))
	{
		ChangeMusicImageGraph(changeMusicNum);
	}
	else if (changeImage_flag == false)
	{
		int32 draw_x = DRAW_X_START_POINT;
		for (int32 i = 0; i < 7; ++i)
		{
			auto& music_data = music[(totalMusicNum - (3 - i) + nowMusicNum) % totalMusicNum];
			auto image_handle = music_data.image;
			if (i < 3)
			{
				DrawMI_L(image_handle, draw_x);
				draw_x += DRAW_WIDTH_S + DRAW_X_DISTANCE;
			}
			else if (i == 3)
			{
				Texture tmp(image_handle);
				Quad({ draw_x, DRAW_Y_TOP }, { draw_x + 200, DRAW_Y_TOP }, { draw_x + 200, 225 }, { draw_x, 225 })(tmp).draw();
				draw_x += DRAW_WIDTH_L + DRAW_X_DISTANCE;
			}
			else
			{
				DrawMI_R(image_handle, draw_x);
				draw_x += DRAW_WIDTH_S + DRAW_X_DISTANCE;
			}
		}
		render_flagL = render_flagR = false;
	}
	G_mainFrame.draw();

	// 音楽再生
	PlayMusic_Draw();

	// 作者＆コメント（描画）
	CreatorAndComment_Draw();
}

// 音楽イメージ画像の変化描画
void ChangeMusicImageGraph(int32 toward)
{
	auto per = (image_lasttime - image_starttime) * 100 / DRAW_TIME_COST;
	render_flagL = render_flagR = true;
	if (changeImage_for == 1)
	{
		auto draw_x = DRAW_X_START_POINT + 6 * (DRAW_X_DISTANCE + DRAW_WIDTH_S);
		for (int32 i = 6; i >= 0; --i)
		{
			auto& music_data = music[(totalMusicNum - (3 - i) + nowMusicNum) % totalMusicNum];
			auto image_handle = music_data.image;
			if (i < 2)
			{
				DrawMI_L(image_handle, draw_x + (DRAW_WIDTH_S + DRAW_X_DISTANCE)*per / 100);
			}
			else if (i == 2)
			{
				DrawMI_L(image_handle, draw_x + (DRAW_WIDTH_S + DRAW_X_DISTANCE)*per / 100, Y_DEFAULT_DIFF * per / 100, (DRAW_WIDTH_L - DRAW_X_DISTANCE)*per / 100);
			}
			else if (i == 3)
			{
				DrawMI_R(image_handle, draw_x + (DRAW_WIDTH_S + DRAW_X_DISTANCE)*per / 100, Y_DEFAULT_DIFF * abs(per - 100) / 100, (DRAW_WIDTH_L - DRAW_X_DISTANCE)*per / 100, true);
			}
			else if (i > 3)
			{
				DrawMI_R(image_handle, draw_x + 150 + (DRAW_WIDTH_S + DRAW_X_DISTANCE)*per / 100);
			}
			draw_x -= DRAW_X_DISTANCE + DRAW_WIDTH_S;
		}
	}
	else
	{
		auto draw_x = DRAW_X_START_POINT + 6 * (DRAW_X_DISTANCE + DRAW_WIDTH_S);
		for (int i = 6; i >= 0; --i)
		{
			auto& music_data = music[(totalMusicNum - (3 - i) + nowMusicNum) % totalMusicNum];
			auto image_handle = music_data.image;
			if (i < 3)
			{
				DrawMI_L(image_handle, draw_x - (DRAW_WIDTH_S + DRAW_X_DISTANCE)*per / 100);
			}
			else if (i == 3)
			{
				DrawMI_L(image_handle, draw_x - (DRAW_WIDTH_S + DRAW_X_DISTANCE)*per / 100, Y_DEFAULT_DIFF * abs(per - 100) / 100, -(DRAW_WIDTH_L - DRAW_WIDTH_S)*per / 100, true);
			}
			else if (i == 4)
			{
				DrawMI_R(image_handle, draw_x + (DRAW_WIDTH_L - DRAW_WIDTH_S) - (DRAW_WIDTH_S + DRAW_X_DISTANCE) *per / 100, Y_DEFAULT_DIFF * per / 100, -(DRAW_WIDTH_L - DRAW_WIDTH_S)*per / 100);
			}
			else if (i > 4)
			{
				DrawMI_R(image_handle, draw_x + (DRAW_WIDTH_L - DRAW_WIDTH_S) - (DRAW_WIDTH_S + DRAW_X_DISTANCE)*per / 100);
			}
			draw_x -= DRAW_X_DISTANCE + DRAW_WIDTH_S;
		}
	}
	if (image_lasttime - image_starttime >= DRAW_TIME_COST)
	{
		image_starttime = 0;
		if (changeImage_for == 1)
		{
			nowMusicNum--;
			if (nowMusicNum < 0)
			{
				nowMusicNum = totalMusicNum - 1;
			}
		}
		else
		{
			nowMusicNum++;
			if (nowMusicNum >= totalMusicNum)
			{
				nowMusicNum = 0;
			}
		}
		if (toward != -1)
		{
			if (nowMusicNum == toward)
			{
				changeImage_flag = false;
				changeImage_for = 0;
				changeMusicNum = -1;
			}
			image_starttime = image_lasttime;
		}
		else
		{
			changeImage_flag = false;
			changeImage_for = 0;
			changeMusicNum = -1;
		}
		Music_totalTime = -1;
		Music_nowTime = -1;
		draw_musicName_x = 30, draw_musicCreator_x = 400;
		now_musicComment_limitY = 0, Is_musicComment_Y_over = false;
	}
}

// 音楽再生（更新）
void PlayMusic_Update()
{

}

// 音楽再生（描画）
void PlayMusic_Draw()
{

}

const int32 MAX_MUSICNAME_WIDTH = 340;					// 曲の名前の最大幅
const int32 MAX_MUSICCREATOR_WIDTH = 200;					// 曲の作者の最大幅
const int32 WAITINGTIME = 2000;							// 何ミリ秒静止させるか
const int32 MUSICNAME_SIZE = 32;
const int32 MUSICCREATOR_SIZE = 28;

// 作者＆コメント（更新）
void CreatorAndComment_Update()
{

}

// 作者＆コメント（描画）
void CreatorAndComment_Draw()
{

}