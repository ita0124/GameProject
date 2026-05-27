#include <crtdbg.h>
#include "Comon.h"
#include "Lib/Debug/DebugFPS.h"
#include "Lib/Fade/Fade.h"
#include "Lib/Input/InputKey.h"
#include "Lib/Input/InputPad.h"
#include "Lib/LoadMaterial/LoadMaterial.h"
#include "Game/Manager/Scene/SceneManager.h"
#include "Game/Sound/SoundManager.h"
#include "Effekseer/MyEffeckseer.h"

 //プログラムは WinMain から始まります
int  WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// メモリリーク発見用
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// スクリーン設定
	ChangeWindowMode(TRUE);			// フルスクリーンorウィンドウモード
	SetGraphMode((int)SCREEN_MAX_X, (int)SCREEN_MAX_Y, 32);	// ウィンドウのサイズ

#ifndef _DEBUG
	// リリース版はログを出さない
	SetOutApplicationLogValidFlag(false);
#endif

	//ウィンドウ名設定
	SetWindowText("リファクタリング");

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1) return -1;

	//一番最初に１回だけやる処理
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	//入力
	InputKey::Init();

	//フォントをハンドルとして保存する
	DxLibFont::Init();

	//シーン
	SceneManager::Init();

	//音楽
	SoundManager::Init();
	SoundManager::Load();
	SoundManager::Volume();

	//マテリアル
	LoadMaterial::Init();
	LoadMaterial::Load();

	//エフェクト
	float	pers = 60.0f * DX_PI_F / 180.0f;
	SetupCamera_Perspective(pers);

	const int EFFECTNUM = 10;
	const int PARTICLENUM = 1000;
	//第一:エフェクト総数
	//第二:パーティクル総数
	MyEffeckseer::Init(EFFECTNUM, PARTICLENUM);
	MyEffeckseer::Load();

	//その他
	DebugFPS::Init();
	Fade::Init();

	//ゲームメインループ
	while (ProcessMessage() != -1)
	{
		//エスケープキーが押されたら終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;

		//60.0FPSくらいになるように
		if (!DebugFPS::Next())continue;

		ClearDrawScreen();	// 画面クリア

		//ゲーム本体の命令はこれより下に書く
		//FPS管理
		DebugFPS::Step();
		//インプット情報を更新
		InputKey::Update();
		InputPad::Update();

		//シーン管理
		SceneManager::Loop();
		SceneManager::Draw();

		MyEffeckseer::UpdateAutoCamera();
		MyEffeckseer::Draw();

		//フェード
		Fade::Step();
		Fade::Draw();

		//FPS表示
		DebugFPS::Print();
		// ゲーム本体の命令はこれより上に書く


		ScreenFlip();		// 描画切り替え
	}

	LoadMaterial::Exit();

	SceneManager::Exit();

	SoundManager::AllStop();
	SoundManager::Exit();

	MyEffeckseer::Exit();

	DxLib_End();			// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
