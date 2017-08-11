#pragma once

// ‹È ‰Šú‰»
void Music_Init();

// ‹È XV
void Music_Update();

// ‹È •`‰æ
void Music_Draw();

// ‹Èà–¾ •`‰æ
void musicExpl_Draw();

// ‹È–¼•`‰æˆÊ’u XV
void Update_drawMusicDetailStrings();

// ‹Èè“®‘JˆÚ
void changeMusic(int flag);

// ‹È‘€ì
// kind: 0->ˆê’â~, 1->Ä¶, 2->’â~, 3->ŒJ‚è•Ô‚µØ‚è‘Ö‚¦
void changeMusicStats(int kind);