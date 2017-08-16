// include
#include <Siv3D.hpp>
#include <vector>
#include "SceneMgr.h"
#include "Select.h"
#include "Bar.h"

// define
#define WELCOME_MESSAGE_MILLISEC 3000
#define COM_MESSAGE_MILLISEC 1000

// アルバム構造体
struct Album
{
	String name;
	String creator;
	String comment;
	Texture image;
};

// グローバル定数・変数
static std::vector<std::pair<int, int>>comTime;
static std::vector<Album> AlbumList;
static Texture main, no_img;
static Texture fav, power;
static String setAlbum = L"";
static Grid<double_t> z;
static TextReader reader;
static Triangle goUp({ 384,75 }, { 414,85 }, { 354,85 });
static Triangle goDown({ 354,560 }, { 414,560 }, { 384,570 });
static int startTime;
static int nowTime;
static int first_cou;
static bool scr_flag = true;

// アルバム選択 初期化
void Select_Init()
{
	if (main) { return; }
	// メイン背景
	{
		main = Texture(L"data\\Select\\main.png");
		fav = Texture(L"data\\Detail\\fav.png");
	}

	fav = Texture(L"data\\Select\\fav.png");
	power = Texture(L"data\\Select\\power.png");
	no_img = Texture(L"data\\Select\\no_img.png");

	// album_list 読み込み
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
		z = Grid<double>(3, (AlbumList.size() + 2) / 3 + 1);
	}

	startTime = (int)Time::GetMillisec();
	comTime.resize(AlbumList.size() + 2);
}

// アルバム選択 更新
void Select_Update()
{
	nowTime = (int)Time::GetMillisec();

	// スクロール 更新
	{
		if (goUp.leftClicked) { first_cou -= 3; }
		if (goDown.leftClicked) { first_cou += 3; }
		scr_flag = ((first_cou + 5 <= (signed)AlbumList.size()) || (first_cou > 0) ? true : false);
		if (scr_flag) { first_cou += Mouse::Wheel() * 3; }
		first_cou = Max(first_cou, 0);
		first_cou = Min<int>(first_cou, (int)AlbumList.size() / 3 * 3);
	}

	// album_list 更新
	int cou = first_cou;
	for (int y = 0; y < (signed)z.height; ++y)
	{
		for (int x = 0; x < (signed)z.width; ++x)
		{
			const Rect rect = MakeRect(x, y);
			if (cou == (signed)AlbumList.size() + 2) { break; }
			if (Input::MouseL.clicked && rect.mouseOver)
			{
				if (cou == (signed)AlbumList.size()) { SceneMgr_ChangeScene(Scene_Fav); }
				else if (cou == (signed)AlbumList.size() + 1)
				{
					Bar_Draw();
					main.draw(0, BAR_HEIGHT);
					GUI gui(GUIStyle::Default);
					gui.setTitle(L"終了確認");
					gui.addln(GUIText::Create(L"終了しますか？"));
					gui.add(L"yes", GUIButton::Create(L"はい"));
					gui.add(L"no", GUIButton::Create(L"いいえ"));
					gui.setCenter(Window::Center());
					while (System::Update())
					{
						Bar_Draw();
						main.draw(0, BAR_HEIGHT);
						gui.show();
						if (gui.button(L"yes").pushed)
						{
							System::Exit();
							break;
						}
						else if (gui.button(L"no").pushed)
						{
							gui.hide();
							break;
						}
					}
				}
				else
				{
					setAlbum = AlbumList[cou].name;
					const Rect temprect(0, BAR_HEIGHT, Window::Width(), Window::Height());
					const Font tempfont(32, Typeface::Bold);
					Bar_Draw();
					main.draw(0, BAR_HEIGHT);
					temprect.draw(Color(64, 64, 64, 100));
					tempfont(L"読み込み中・・・").drawCenter(Window::Height() / 2);
					System::Update();
					SceneMgr_ChangeScene(Scene_Detail);
				}
			}
			++cou;
		}
		if (cou == (signed)AlbumList.size() + 2) { break; }
	}
}

// アルバム選択 描画
void Select_Draw()
{
	// メイン背景
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

	// album_list 描画
	{
		int cou = first_cou;
		for (int y = 0; y < (signed)z.height; ++y)
		{
			for (int x = 0; x < (signed)z.width; ++x)
			{
				const Rect rect = MakeRect(x, y);
				if (cou == (signed)AlbumList.size() + 2) { break; }
				if (!rect.mouseOver)
				{
					rect(SelectImage(cou).resize(216, 216)).draw();
					rect.drawFrame(3, 0, Color(0, 0, 0));
					z[y][x] = Max(z[y][x] - 0.02, 0.0);
				}
				++cou;
			}
			if (cou == (signed)AlbumList.size() + 2) { break; }
		}
		cou = first_cou;
		for (int y = 0; y < (signed)z.height; ++y)
		{
			for (int x = 0; x < (signed)z.width; ++x)
			{
				const Rect rect = MakeRect(x, y);
				if (rect.mouseOver || z[y][x])
				{
					if (rect.mouseOver) { z[y][x] = Min(z[y][x] + 0.05, 0.5); }
				}
				const double s = z[y][x];
				if (cou == (signed)AlbumList.size() + 2) { break; }
				RectF(rect).stretched(s * 2).drawShadow({ 0,15 * s }, 32 * s, 10 * s);
				RectF(rect).stretched(s * 2)(SelectImage(cou).resize(216, 216)).draw();
				RectF(rect).stretched(s * 2).drawFrame(3, 0, Color(0, 0, 0));
				++cou;
			}
			if (cou == (signed)AlbumList.size() + 2) { break; }
		}
		cou = first_cou;
		for (int y = 0; y < (signed)z.height; ++y)
		{
			for (int x = 0; x < (signed)z.width; ++x)
			{
				const Rect rect = MakeRect(x, y);
				if (cou == (signed)AlbumList.size() + 2) { break; }
				if (rect.mouseOver)
				{
					comTime[cou].first = (comTime[cou].first == 0 ? (int)Time::GetMillisec() : comTime[cou].first);
					comTime[cou].second = (int)Time::GetMillisec();
					if (comTime[cou].second - comTime[cou].first >= COM_MESSAGE_MILLISEC) { DrawDetails(cou); }
				}
				else { comTime[cou].first = comTime[cou].second = 0; }
				++cou;
			}
			if (cou == (signed)AlbumList.size() + 2) { break; }
		}
	}
}

// アルバム一覧 正方形区画を作成
Rect MakeRect(int x, int y)
{
	return { 30 + x * 246,BAR_HEIGHT + 30 + y * 246,216,216 };
}

// アルバム画像を返す
Texture SelectImage(int cou)
{
	Texture res;

	// アルバム
	if (cou < (signed)AlbumList.size()) { res = AlbumList[cou].image; }

	// お気に入り
	else if (cou == (signed)AlbumList.size()) { res = fav; }

	// 終了
	else if (cou == (signed)AlbumList.size() + 1) { res = power; }

	return res;
}

// 選択されたアルバムの名前を返す
String getSetAlbum()
{
	return setAlbum;
}

// アルバム詳細 描画
void DrawDetails(int cou)
{
	const Point pos = Mouse::Pos();
	static Font font(16);
	static String name, creator;
	if (cou == (signed)AlbumList.size())
	{
		name = L"お気に入り";
		creator = L"お気に入り登録した曲を表示します。";
	}
	else if (cou == (signed)AlbumList.size() + 1)
	{
		name = L"終了";
		creator = L"MusicRoom v2.0 を終了します。";
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
	if (cou % 3 == 0) { x_addtion = 13; }
	if (cou % 3 == 1) { x_addtion = (-width / 2); }
	if (cou % 3 == 2) { x_addtion = -width; }
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).drawShadow({ 0,15 }, 32, 10);
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).draw(Color({ 255,255,255 }, 120));
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).drawFrame(3);
	font(name).draw(pos.x + x_addtion + 14, pos.y + 20, Color(16, 16, 16));
	font(creator).draw(pos.x + x_addtion + 14, pos.y + 50, Color(16, 16, 16));
}