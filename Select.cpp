// include
#include <Siv3D.hpp>
#include <vector>
#include "SceneMgr.h"
#include "Select.h"
#include "Bar.h"

// define
#define WELCOME_MESSAGE_MILLISEC 3000
#define MAIN_SKEL_LIMIT 80
#define COM_MESSAGE_MILLISEC 1250

// �A���o���\����
struct Album
{
	String name;
	String creator;
	String comment;
	Texture image;
};

// �O���[�o���萔�E�ϐ�
static std::vector<std::pair<int64_t, int64_t>>comTime;
static std::vector<Album> AlbumList;
static const RectF main_rect(0, BAR_HEIGHT, 768, 768);
static Image main_tmp;
static Texture main, no_img;
static Texture fav, power;
static Texture Gaussian;
static String setAlbum = L"";
static Grid<double_t> z;
static TextReader reader;
static int64_t startTime;
static int64_t nowTime;
static int32_t skel;
static int32_t first_cou;
static bool scr_flag = true;

// �A���o���I�� ������
void Select_Init()
{
	if (main_tmp) { return; }

	// ���C���w�i
	{
		main_tmp = Image(L"data\\Select\\main.png");
		main = Texture(main_tmp);
		Gaussian = Texture(main_tmp.gaussianBlurred(20, 20));
	}

	fav = Texture(L"data\\Select\\fav.png");
	power = Texture(L"data\\Select\\power.png");
	no_img = Texture(L"data\\Select\\no_img.png");

	// album_list �ǂݍ���
	{
		String temp;
		reader = TextReader(L"music\\album_list.txt");
		while (reader.readLine(temp))
		{
			String name, creator, comment;
			TextReader text(L"music\\" + temp + L"\\" + temp + L".txt");
			text.readLine(name);
			text.readLine(creator);
			String temp_of_temp;
			while (text.readLine(temp_of_temp)) { comment += temp_of_temp; }
			Texture image(L"music\\" + temp + L"\\" + temp + L".png");
			if (!image) { image = no_img; }
			AlbumList.push_back({ name,creator,comment,image });
		}
		AlbumList.push_back({ L"�V���O����",L"��Ȏ�",L"�R�����g",Texture(L"data\\Select\\single.png") });
		z = Grid<double>(3, (AlbumList.size() + 2) / 3 + 1);
	}

	startTime = Time::GetMillisec64();
	comTime.resize(AlbumList.size() + 2);
}

// �A���o���I�� �X�V
void Select_Update()
{
	skel = (skel < MAIN_SKEL_LIMIT ? skel + 1 : skel);
	nowTime = Time::GetMillisec64();

	// �X�N���[�� �X�V
	{
		scr_flag = ((first_cou + 5 <= (signed)AlbumList.size()) || (first_cou > 0) ? true : false);
		if (scr_flag)
		{
			first_cou += Mouse::Wheel() * 3;
			first_cou = Max(first_cou, 0);
			first_cou = Min<int32_t>(first_cou, AlbumList.size() / 3 * 3);
		}
	}

	// album_list �X�V
	int32_t cou = first_cou;
	for (int32_t y = 0; y < (signed)z.height; ++y)
	{
		for (int32_t x = 0; x < (signed)z.width; ++x)
		{
			const Rect rect = MakeRect(x, y);
			if (!SelectImage(cou)) { break; }
			if (Input::MouseL.clicked && rect.mouseOver)
			{
				setAlbum = AlbumList[cou].name;
				SceneMgr_ChangeScene(Scene_Detail);
			}
			++cou;
		}
		if (!SelectImage(cou)) { break; }
	}
}

// �A���o���I�� �`��
void Select_Draw()
{
	// ���C���w�i
	{
		main.draw(0, BAR_HEIGHT);
		Gaussian.draw(0, BAR_HEIGHT);
		// main_rect.draw(Color(255, skel));
	}

	// album_list �`��
	{
		int32_t cou = first_cou;
		for (int32_t y = 0; y < (signed)z.height; ++y)
		{
			for (int32_t x = 0; x < (signed)z.width; ++x)
			{
				const Rect rect = MakeRect(x, y);
				if (!SelectImage(cou)) { break; }
				if (!rect.mouseOver)
				{
					rect(SelectImage(cou).resize(216, 216)).draw();
					rect.drawFrame(3, 0, Color(0, 0, 0));
					z[y][x] = Max(z[y][x] - 0.02, 0.0);
				}
				++cou;
			}
			if (!SelectImage(cou)) { break; }
		}
		cou = first_cou;
		for (int32_t y = 0; y < (signed)z.height; ++y)
		{
			for (int32_t x = 0; x < (signed)z.width; ++x)
			{
				const Rect rect = MakeRect(x, y);
				if (rect.mouseOver || z[y][x])
				{
					if (rect.mouseOver) { z[y][x] = Min(z[y][x] + 0.05, 0.5); }
				}
				const double s = z[y][x];
				if (!SelectImage(cou)) { break; }
				RectF(rect).stretched(s * 2).drawShadow({ 0,15 * s }, 32 * s, 10 * s);
				RectF(rect).stretched(s * 2)(SelectImage(cou).resize(216, 216)).draw();
				RectF(rect).stretched(s * 2).drawFrame(3, 0, Color(0, 0, 0));
				++cou;
			}
			if (!SelectImage(cou)) { break; }
		}
		cou = first_cou;
		for (int32_t y = 0; y < (signed)z.height; ++y)
		{
			for (int32_t x = 0; x < (signed)z.width; ++x)
			{
				const Rect rect = MakeRect(x, y);
				if (!SelectImage(cou)) { break; }
				if (rect.mouseOver)
				{
					comTime[cou].first = (comTime[cou].first == 0 ? Time::GetMillisec64() : comTime[cou].first);
					comTime[cou].second = Time::GetMillisec64();
					if (comTime[cou].second - comTime[cou].first >= COM_MESSAGE_MILLISEC) { DrawDetails(cou); }
				}
				else { comTime[cou].first = comTime[cou].second = 0; }
				++cou;
			}
			if (!SelectImage(cou)) { break; }
		}
	}
}

// �A���o���ꗗ �����`�����쐬
Rect MakeRect(int32_t x, int32_t y)
{
	return { 30 + x * 246,BAR_HEIGHT + 30 + y * 246,216,216 };
}

// �A���o���摜��Ԃ�
Texture SelectImage(int32_t cou)
{
	Texture res;

	// �A���o��
	if (cou < (signed)AlbumList.size()) { res = AlbumList[cou].image; }

	// ���C�ɓ���
	else if (cou == (signed)AlbumList.size()) { res = fav; }

	// �I��
	else if (cou == (signed)AlbumList.size() + 1) { res = power; }

	return res;
}

// �I�����ꂽ�A���o���̖��O��Ԃ�
String getSetAlbum()
{
	return setAlbum;
}

// �A���o���ڍ� �`��
void DrawDetails(int32_t cou)
{
	const Point pos = Mouse::Pos();
	static Font font(16);
	static String name, creator;
	if (cou == (signed)AlbumList.size())
	{
		name = L"���C�ɓ���";
		creator = L"���C�ɓ���o�^�����Ȃ�\�����܂��B";
	}
	else if (cou == (signed)AlbumList.size() + 1)
	{
		name = L"�I��";
		creator = L"MusicRoom v2.0 ���I�����܂��B";
	}
	else
	{
		name = AlbumList[cou].name;
		creator = AlbumList[cou].creator;
	}
	const auto name_width = font(name).region();
	const auto creator_width = font(creator).region();
	const auto width = Max(name_width.w, creator_width.w);

	static int32_t x_addtion;
	if (cou % 3 == 0) { x_addtion = 13; }
	if (cou % 3 == 1) { x_addtion = (-width / 2); }
	if (cou % 3 == 2) { x_addtion = -width; }
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).drawShadow({ 0,15 }, 32, 10);
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).draw(Color({ 255,255,255 }, 120));
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).drawFrame(3);
	font(name).draw(pos.x + x_addtion + 14, pos.y + 20, Color(16, 16, 16));
	font(creator).draw(pos.x + x_addtion + 14, pos.y + 50, Color(16, 16, 16));
}