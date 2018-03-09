// include
#include <Siv3D.hpp>
#include <vector>
#include "SceneMgr.h"
#include "Select.h"
#include "Bar.h"

// const
const int COM_MESSAGE_MILLISEC = 1000;
const int SCROLL_MSEC = 500;

// �A���o���\����
struct Album
{
	String name, dname, creator, comment;
	Texture image;
};

// �O���[�o���萔�E�ϐ�
static std::vector<std::pair<int, int>>comTime;
static std::vector<Album> AlbumList;
static Texture main, no_img, fav;
static String setAlbum = L"", setAlbumB = L"";
static Grid<double_t> z;
static TextReader reader;
static Triangle goUp({ 384,75 }, { 414,85 }, { 354,85 }), goDown({ 354,560 }, { 414,560 }, { 384,570 });
static int startTime, nowTime, first_cou, selectedAlbumNum, scrollStartTime, scrollNowTime, scrollY;
static bool scr_flag = false, scr_upflag;

// �A���o���I�� ������
void Select_Init()
{
	if (main) return;
	// ���C���w�i
	{
		main = Texture(L"data\\Select\\main.png");
		fav = Texture(L"data\\Detail\\fav.png");
	}

	fav = Texture(L"data\\Select\\fav.png");
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
			while (text.readLine(temp_of_temp)) comment += temp_of_temp;
			Texture image(L"music\\" + temp + L"\\" + temp + L".png");
			if (!image) image = no_img;
			AlbumList.push_back({ name,temp,creator,comment,image });
		}
		z = Grid<double>(3, (AlbumList.size() + 1) / 3 + 1);
	}
	startTime = (int)Time::GetMillisec();
	comTime.resize(AlbumList.size() + 8);
}

// �A���o���I�� �X�V
void Select_Update()
{
	nowTime = (int)Time::GetMillisec();
	if (Input::KeyF5.pressed)
	{
		const Rect temprect(0, BAR_HEIGHT, Window::Width(), Window::Height());
		const Font tempfont(32, Typeface::Bold);
		Bar_Draw();
		main.draw(0, BAR_HEIGHT);
		temprect.draw(Color(64, 64, 64, 100));
		tempfont(L"�ēǂݍ��ݒ��E�E�E").drawCenter(Window::Height() / 2);
		System::Update();
		Select_Init();
	}
	// �X�N���[�� �X�V
	{
		if (!scr_flag)
		{
			if (first_cou > 0)
			{
				if (goUp.leftClicked)
				{
					scr_flag = true;
					scr_upflag = true;
				}
				if (Mouse::Wheel() < 0)
				{
					scr_flag = true;
					scr_upflag = true;
				}
			}
			if (first_cou + 5 <= (signed)AlbumList.size())
			{
				if (goDown.leftClicked)
				{
					scr_flag = true;
					scr_upflag = false;
				}
				if (Mouse::Wheel() > 0)
				{
					scr_flag = true;
					scr_upflag = false;
				}
			}
			if (scr_flag) scrollNowTime = scrollStartTime = Time::GetMillisec();
		}
		else
		{
			if (scrollNowTime - scrollStartTime > SCROLL_MSEC)
			{
				first_cou += (!scr_upflag ? 3 : -3);
				scr_flag = false;
				scrollY = 0;
				first_cou = Max(first_cou, 0);
				first_cou = Min<int>(first_cou, (int)AlbumList.size() / 3 * 3);
			}
			else scrollY = (!scr_upflag ? -246 * ((scrollNowTime - scrollStartTime)) / SCROLL_MSEC : 246 * ((scrollNowTime - scrollStartTime)) / SCROLL_MSEC);
			scrollNowTime = Time::GetMillisec();
		}
	}

	// album_list �X�V
	int cou = first_cou;
	for (int y = 0; y < (signed)z.height; ++y)
	{
		for (int x = 0; x < (signed)z.width; ++x)
		{
			const Rect rect = MakeRect(x, y);
			if (cou == (signed)AlbumList.size() + 2) break;
			if (Input::MouseL.clicked && rect.mouseOver)
			{
				if (cou == (signed)AlbumList.size()) SceneMgr_ChangeScene(Scene_Fav);
				else
				{
					setAlbum = AlbumList[cou].name;
					setAlbumB = AlbumList[cou].dname;
					selectedAlbumNum = cou;
					const Rect temprect(0, BAR_HEIGHT, Window::Width(), Window::Height());
					const Font tempfont(32, Typeface::Bold);
					Bar_Draw();
					main.draw(0, BAR_HEIGHT);
					temprect.draw(Color(64, 64, 64, 100));
					tempfont(L"�ǂݍ��ݒ��E�E�E").drawCenter(Window::Height() / 2);
					System::Update();
					SceneMgr_ChangeScene(Scene_Detail);
				}
			}
			++cou;
		}
		if (cou == (signed)AlbumList.size() + 1) break;
	}
}

// �A���o���I�� �`��
void Select_Draw()
{
	// ���C���w�i
	{
		main.draw(0, BAR_HEIGHT);
		if (first_cou > 0)
		{
			goUp.draw((goUp.mouseOver ? Palette::Orange : Palette::White));
			goUp.drawFrame(2, Palette::Black);
		}
		if (first_cou + 5 <= (signed)AlbumList.size())
		{
			goDown.draw((goDown.mouseOver ? Palette::Orange : Palette::White));
			goDown.drawFrame(2, Palette::Black);
		}
	}

	// album_list �`��
	{
		int cou = first_cou - 3;
		for (int y = -1; y <= (signed)z.height; ++y)
		{
			for (int x = 0; x < (signed)z.width; ++x)
			{
				Rect rect = MakeRect(x, y);
				if (scr_flag) rect.y += scrollY;
				if (cou == (signed)AlbumList.size() + 1) break;
				if (cou >= 0)
				{
					if (!rect.mouseOver)
					{
						rect(SelectImage(cou).resize(216, 216)).draw();
						rect.drawFrame(3, 0, Color(0, 0, 0));
						z[y + 1][x + 1] = Max(z[y + 1][x + 1] - 0.02, 0.0);
					}
				}
				++cou;
			}
			if (cou == (signed)AlbumList.size() + 1) break;
		}
		cou = first_cou - 3;
		for (int y = -1; y <= (signed)z.height; ++y)
		{
			for (int x = 0; x < (signed)z.width; ++x)
			{
				Rect rect = MakeRect(x, y);
				rect.y += scrollY;
				if (rect.mouseOver || z[y + 1][x + 1])
				{
					if (rect.mouseOver) z[y + 1][x + 1] = Min(z[y + 1][x + 1] + 0.05, 0.5);
				}
				const double s = z[y + 1][x + 1];
				if (cou == (signed)AlbumList.size() + 1) break;
				if (cou >= 0)
				{
					RectF(rect).stretched(s * 2).drawShadow({ 0,15 * s }, 32 * s, 10 * s);
					RectF(rect).stretched(s * 2)(SelectImage(cou).resize(216, 216)).draw();
					RectF(rect).stretched(s * 2).drawFrame(3, 0, Color(0, 0, 0));
				}
				++cou;
			}
			if (cou == (signed)AlbumList.size() + 1) break;
		}
		cou = first_cou - 3;
		for (int y = -1; y <= (signed)z.height; ++y)
		{
			for (int x = 0; x < (signed)z.width; ++x)
			{
				Rect rect = MakeRect(x, y);
				rect.y += scrollY;
				if (cou == (signed)AlbumList.size() + 1) break;
				if (cou >= 0)
				{
					if (rect.mouseOver)
					{
						comTime[cou].first = (comTime[cou].first == 0 ? (int)Time::GetMillisec() : comTime[cou].first);
						comTime[cou].second = (int)Time::GetMillisec();
						if (comTime[cou].second - comTime[cou].first >= COM_MESSAGE_MILLISEC) DrawDetails(cou);
					}
					else comTime[cou].first = comTime[cou].second = 0;
				}
				++cou;
			}
			if (cou == (signed)AlbumList.size() + 1) break;
		}
	}
}

// �A���o���ꗗ �����`�����쐬
Rect MakeRect(int x, int y)
{
	return { 30 + x * 246,BAR_HEIGHT + 30 + y * 246,216,216 };
}

// �A���o���摜��Ԃ�
Texture SelectImage(int cou)
{
	Texture res;
	if (cou < 0) return res;
	if (cou < (signed)AlbumList.size()) res = AlbumList[cou].image;	// �A���o��
	else if (cou == (signed)AlbumList.size()) res = fav;	// ���C�ɓ���
	return res;
}

// �I�����ꂽ�A���o���̖��O��Ԃ�
std::pair<String, String> getSetAlbum()
{
	return std::make_pair(setAlbum, setAlbumB);
}

// ���̃A���o����Ԃ�
void getNextAlbum()
{
	++selectedAlbumNum;
	setAlbum = AlbumList[selectedAlbumNum % AlbumList.size()].name;
	setAlbumB = AlbumList[selectedAlbumNum % AlbumList.size()].dname;
}

// �A���o���ڍ� �`��
void DrawDetails(int cou)
{
	const Point pos = Mouse::Pos();
	static Font font(16);
	static String name, creator;
	if (cou == (signed)AlbumList.size())
	{
		name = L"���C�ɓ���";
		creator = L"���C�ɓ���o�^�����Ȃ�\�����܂��B";
	}
	else
	{
		name = AlbumList[cou].name;
		creator = AlbumList[cou].creator;
	}
	const auto name_width = font(name).region();
	const auto creator_width = font(creator).region();
	const auto width = Max(name_width.w, creator_width.w);
	static int x_addtion;
	if (cou % 3 == 0) x_addtion = 13;
	if (cou % 3 == 1) x_addtion = (-width / 2);
	if (cou % 3 == 2) x_addtion = -width;
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).drawShadow({ 0,15 }, 32, 10);
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).draw(Color({ 255,255,255 }, 120));
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).drawFrame(3);
	font(name).draw(pos.x + x_addtion + 14, pos.y + 20, Color(16, 16, 16));
	font(creator).draw(pos.x + x_addtion + 14, pos.y + 50, Color(16, 16, 16));
}