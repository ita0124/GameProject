#pragma once
#include <DxLib.h>

//FPS管理クラス
class DebugFPS {
private:
	static float m_FrameRate;		//今のフレームを保存
	static int m_NowTime;			//現在時間
	static int m_OldTime;			//現在時間よりひとつ前の時間
	static int m_OldDrawTime;		//現在時間よりひとつ前の描画したPC時間
	static int m_Count;				//カウント用

	static int m_FontHndl;
public:
	//コンストラクタ
	DebugFPS();

	//初期化
	static void Init();

	//FPSを表示
	static void Print();

	//FPS計算
	static void Calcu();

	//次のフレームに進めて良いか
	static bool Next();

	//FPS更新
	static void Step();
};
