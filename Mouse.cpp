#include <Siv3D.hpp>

// 変数
Point pos;
bool before = false;

// マウスが引数の範囲に入ったか判定（入っていればtrue,入っていなければfalse）
// i=左上（x座標） j=左上（y座標） k=右下（x座標） l=（y座標）
bool CheckMouseIn(int32 i, int32 j, int32 k, int32 l) {
	const Quad quad({ i, j }, { k, j }, { k, l }, { i, l });
	return quad.mouseOver;
}

// マウスが引数の範囲でクリックしたか判定（クリックしていたらtrue,離したらfalse）
// i=左上（x座標） j=左上（y座標） k=右下（x座標） l=（y座標）
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