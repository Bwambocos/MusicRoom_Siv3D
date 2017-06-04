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
static int64_t startTime;
static int64_t nowTime;
static int32_t skel;

// �A���o���I�� ������
void Select_Init()
{
	// ���C���w�i
	{
		main_tmp = Image(L"data\\Select\\main.png");
		main = Texture(main_tmp);
		Gaussian = Texture(main_tmp.gaussianBlurred(20, 20));
	}

	startTime = Time::GetMillisec64();
}

// �A���o���I�� �X�V
void Select_Update()
{
	skel = (skel < MAIN_SKEL_LIMIT ? skel + 1 : skel);
	nowTime = Time::GetMillisec64();
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
}