// include
#include <Siv3D.hpp>
#include "Select.h"

// define
#define WELCOME_MESSAGE_MILLISEC 3000
#define MAIN_SKEL_LIMIT 80

// �O���[�o���萔�E�ϐ�
static Image main_tmp;
static Texture main;
static Texture Gaussian;
static const RectF main_rect(768, 768);
static const RectF welcome_rec({ 60,290 }, { 660,190 });
static int64_t startTime;
static int64_t nowTime;
static int32_t skel;
static Font font;
static bool welcome_flag = true;

// �A���o���I�� ������
void Select_Init()
{
	// ���C���w�i
	{
		main_tmp = Image(L"data\\Select\\main.png");
		main = Texture(main_tmp);
		Gaussian = Texture(main_tmp.gaussianBlurred(20, 20));
	}

	// Welcome ���b�Z�[�W
	font = Font(48);

	startTime = Time::GetMillisec64();
}

// �A���o���I�� �X�V
void Select_Update()
{
	nowTime = Time::GetMillisec64();

	// Welcome ���b�Z�[�W
	if ((nowTime - startTime) >= WELCOME_MESSAGE_MILLISEC)
	{
		if (skel <= MAIN_SKEL_LIMIT) { ++skel; }
		else { welcome_flag = false; }
	}
}

// �A���o���I�� �`��
void Select_Draw()
{
	// ���C���w�i
	{
		main.draw();
		Gaussian.draw();
		main_rect.draw(Color(255, skel));
		main_rect.drawFrame(3);
	}

	// Welcome ���b�Z�[�W
	if (welcome_flag)
	{
		welcome_rec.draw(Color(255, 100));
		font(L"MusicRoom v2.0 ��\n�悤�����I").drawCenter({ 384,384 }, Color(255, 255 - skel * 3));
	}
}