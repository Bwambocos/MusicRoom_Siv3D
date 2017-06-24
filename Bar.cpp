// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Bar.h"

// define
#define DEFAULT_mainRectWidth 256

static RoundRect fieldRect(0, 0, 768, BAR_HEIGHT, 16);
static RoundRect mainRect(256, 0, 256, BAR_HEIGHT, 16);
static int32_t mainRectWidth = DEFAULT_mainRectWidth;
static Texture originPlay[2], originBrief[2], originStop[2], originSeek[2], originRep[2], originPrev[2], originNext[2];
static Texture displayPlay, displayBrief, displayStop, displaySeek, displayRep, displayPrev, displayNext;
static Sound nowMusic;
static String mainText;
static int32_t nowTime, totalTime;
static Font mainFont, timeFont;

// �o�[ ������
void Bar_Init()
{
	// �{�^�� ������
	{
		originPlay[0] = Texture(L"data\\Bar\\play\\normal.png");
		originPlay[1] = Texture(L"data\\Bar\\play\\active.png");
		originBrief[0] = Texture(L"data\\Bar\\brief\\normal.png");
		originBrief[1] = Texture(L"data\\Bar\\brief\\active.png");
		originStop[0] = Texture(L"data\\Bar\\stop\\normal.png");
		originStop[1] = Texture(L"data\\Bar\\stop\\active.png");
		originSeek[0] = Texture(L"data\\Bar\\seek\\normal.png");
		originSeek[1] = Texture(L"data\\Bar\\seek\\active.png");
		originRep[0] = Texture(L"data\\Bar\\rep\\normal.png");
		originRep[1] = Texture(L"data\\Bar\\rep\\active.png");
		originPrev[0] = Texture(L"data\\Bar\\prev\\normal.png");
		originPrev[1] = Texture(L"data\\Bar\\prev\\active.png");
		originNext[0] = Texture(L"data\\Bar\\next\\normal.png");
		originNext[1] = Texture(L"data\\Bar\\next\\active.png");
		displayPlay = originPlay[0];
		displayBrief = originBrief[0];
		displayStop = originStop[0];
		displaySeek = originSeek[0];
		displayRep = originRep[0];
		displayPrev = originPrev[0];
		displayNext = originNext[0];
	}

	mainFont = Font(18);
	timeFont = Font(12);
}

// �o�[ �X�V
void Bar_Update()
{
	const Rect rect = mainFont(mainText).region();
	mainRectWidth = rect.w + 18 * 2;
	mainRectWidth = Max(DEFAULT_mainRectWidth, mainRectWidth);
	mainRect = RoundRect(256 - (mainRectWidth - DEFAULT_mainRectWidth) / 2, 0, mainRectWidth, BAR_HEIGHT, 16);
	
	// �{�^�� �X�V
	{
		int32_t x = 768 / 2 - mainRectWidth / 2 - 40 * 3;
		for (int32_t cou = 0; cou < 5; ++cou)
		{
			const Circle button(x + 20, 12 + 20, 20);
			switch (cou)
			{
			case 0:
				if (button.mouseOver) { displayPrev = originPrev[1]; }
				else { displayPrev = originPrev[0]; }
				break;
			case 1:
				if (button.mouseOver) { displayPlay = originPlay[1]; }
				else { displayPlay = originPlay[0]; }
				break;
			case 2:
				if (button.mouseOver) { displayRep = originRep[1]; }
				else { displayRep = originRep[0]; }
				x += mainRectWidth;
				break;
			case 3:
				if (button.mouseOver) { displayStop = originStop[1]; }
				else { displayStop = originStop[0]; }
				break;
			case 4:
				if (button.mouseOver) { displayNext = originNext[1]; }
				else { displayNext = originNext[0]; }
				break;
			}
			x += 40;
		}
	}

	// ���C���e�L�X�g �X�V
	{
		if (!nowMusic.isPlaying())
		{
			auto nowScene = get_nowScene();
			switch (nowScene)
			{
			case Scene_Select:
				mainText = L"�A���o����I�����Ă�������";
				break;
			case Scene_Detail:
				mainText = L"�Ȃ�I�����Ă�������";
				break;
			}
		}
	}

	// �Đ��ʒu�e�L�X�g �X�V
	{
		if (nowMusic.isPlaying())
		{
			totalTime = nowMusic.lengthSec();
			nowTime = nowMusic.streamPosSec();
		}
	}
}

// �o�[ �`��
void Bar_Draw()
{
	fieldRect.draw(Color(200, 200, 200));
	mainRect.draw(Color(125, 125, 125));
	
	// �{�^�� �`��
	{
		int32_t x = 768 / 2 - mainRectWidth / 2 - 40 * 3;
		for (int32_t cou = 0; cou < 5; ++cou)
		{
			switch (cou)
			{
			case 0:
				displayPrev.draw(x, 12);
				break;
			case 1:
				if (nowMusic.isPlaying()) { displayBrief.draw(x, 12); }
				else { displayPlay.draw(x, 12); }
				break;
			case 2:
				displayRep.draw(x, 12);
				x += mainRectWidth;
				break;
			case 3:
				displayStop.draw(x, 12);
				break;
			case 4:
				displayNext.draw(x, 12);
				break;
			}
			x += 40;
		}
	}

	// ���C���e�L�X�g �`��
	{
		const Rect rect = mainFont(mainText).region();
		mainFont.draw(mainText, 768 / 2 - rect.w / 2, 15);
	}

	// �Đ��ʒu�e�L�X�g �`��
	{
		if (nowMusic.isPlaying())
		{
			const Rect rect = timeFont(nowTime + L":" + totalTime).region();
			timeFont.draw(nowTime + L":" + totalTime, (768 / 2 + mainRectWidth / 2) - rect.w, 12, Color(0, 0, 0));
		}
	}
}

// �Đ������擾
bool is_nowMusicPlaying()
{
	return nowMusic.isPlaying();
}