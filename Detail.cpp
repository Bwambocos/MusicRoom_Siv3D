#include <Siv3D.hpp>
#include "Detail.h"
#include "Bar.h"

static Texture main;
static RoundRect albumImage(25, 25 + BAR_HEIGHT, 250, 250, 12.5);
static RoundRect albumDetail(325, 25 + BAR_HEIGHT, 393, 250, 12.5);
static RoundRect albumList_Flag(25, 300 + BAR_HEIGHT, 36, 36, 5);
static RoundRect albumList_Name(64, 300 + BAR_HEIGHT, 537, 36, 5);
static RoundRect albumList_Time(604, 300 + BAR_HEIGHT, 100, 36, 5);
static RoundRect albumList_Fav(707, 300 + BAR_HEIGHT, 36, 36, 5);

// �A���o���ڍ� ������
void Detail_Init()
{
	main = Texture(L"data\\Detail\\main.png");
}

// �A���o���ڍ� �X�V
void Detail_Update()
{

}

// �A���o���ڍ� �`��
void Detail_Draw()
{
	// �w�i �`��
	{
		main.draw(0, BAR_HEIGHT);
		albumImage.drawShadow({ 0,15 }, 32, 10);
		albumImage.drawFrame(3);
		albumImage.draw(Color(32, 32, 32, 120));
		albumDetail.drawShadow({ 0,15 }, 32, 10);
		albumDetail.drawFrame(3);
		albumDetail.draw(Color(32, 32, 32, 120));
		for (int32_t i = 0; i < 5; ++i)
		{
			RoundRect(albumList_Flag.x, albumList_Flag.y + i * 39, albumList_Flag.w, albumList_Flag.h, albumList_Flag.r).draw(Color(32, 32, 32, 200));
			RoundRect(albumList_Name.x, albumList_Name.y + i * 39, albumList_Name.w, albumList_Name.h, albumList_Name.r).draw(Color(32, 32, 32, 200));
			RoundRect(albumList_Time.x, albumList_Time.y + i * 39, albumList_Time.w, albumList_Time.h, albumList_Time.r).draw(Color(32, 32, 32, 200));
			RoundRect(albumList_Fav.x, albumList_Fav.y + i * 39, albumList_Fav.w, albumList_Fav.h, albumList_Fav.r).draw(Color(32, 32, 32, 200));
		}
	}
}