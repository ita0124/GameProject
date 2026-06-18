#include <crtdbg.h>
#include "Comon.h"
#include "Lib/FPS/FPS.h"
#include "Lib/Input/InputKey.h"
#include "Lib/Input/InputPad.h"
#include "Game/Camera/Player/PlayerCamera.h"
#include "Game/Object/Actor/Character/Player/Player.h"
#include "Game/Object/Sword/Sword.h"
#include "Game/Object/Sky/Sky.h"

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
	SetWindowText("ActionGame");

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1) return -1;

	//一番最初に１回だけやる処理
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	PlayerCamera	m_PlayerCamera;
	Player			m_Player;
	Sword			m_Sword;
	Sky				m_Sky;

	//入力
	InputKey::Init();

	//フォントをハンドルとして保存する
	DxLibFont::Init();

	m_PlayerCamera.Init();
	m_Player.Init();
	m_Sword.Init(&m_Player);
	m_Sky.Init();

	m_Player.Load();
	m_Sword.Load();
	m_Sky.Load();

	//その他
	DebugFPS::Init();

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

		m_PlayerCamera.Step(m_Player);
		m_Player.SetCameraRot(m_PlayerCamera.GetCameraRot());

		m_Sky.Step();
		m_Sky.Update();
		m_Sky.Draw();

		m_Player.Step();
		m_Sword.Step();

		m_PlayerCamera.Update();
		m_Player.Update();
		m_Sword.Update();
		
		m_Player.Draw();
		m_Sword.Draw();

		//FPS表示
		DebugFPS::Print();
		// ゲーム本体の命令はこれより上に書く


		ScreenFlip();		// 描画切り替え
	}

	m_Player.Exit();
	m_Sword.Exit();
	m_Sky.Exit();

	DxLib_End();			// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
