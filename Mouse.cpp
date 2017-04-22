#include <Siv3D.hpp>

// �ϐ�
Point pos;
bool before = false;

// �}�E�X�������͈̔͂ɓ�����������i�����Ă����true,�����Ă��Ȃ����false�j
// i=����ix���W�j j=����iy���W�j k=�E���ix���W�j l=�iy���W�j
bool CheckMouseIn(int32 i, int32 j, int32 k, int32 l) {
	const Quad quad({ i, j }, { k, j }, { k, l }, { i, l });
	return quad.mouseOver;
}

// �}�E�X�������͈̔͂ŃN���b�N����������i�N���b�N���Ă�����true,��������false�j
// i=����ix���W�j j=����iy���W�j k=�E���ix���W�j l=�iy���W�j
bool CheckMouseClick(int32 i, int32 j, int32 k, int32 l) {
	if (CheckMouseIn(i, j, k, l))
	{
		if (Input::MouseL.clicked)
		{
			if (before == false) { before = true; return true; }
			else { return false; }
		}
	}
	else { before = false; return false; }
}