#pragma once

// シーン構造体
typedef enum
{
	Scene_Select,
	Scene_Detail,
	Scene_Music,
}Scene_S;

// 更新
void SceneMgr_Update();

// 描画
void SceneMgr_Draw();

// シーン変更
void SceneMgr_ChangeScene(Scene_S);

// 前のシーンを取得
Scene_S get_prevScene();

// 今のシーンを取得
Scene_S get_nowScene();