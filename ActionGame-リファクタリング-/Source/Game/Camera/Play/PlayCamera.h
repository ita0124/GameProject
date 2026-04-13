#pragma once
#include "Comon.h"
#include "Lib/Input/InputKey.h"
#include "Lib/Input/InputPad.h"

//ゲームプレイ中に使うカメラクラス
class PlayCamera {
private:
	VECTOR m_CameraPos;		//カメラ位置
	VECTOR m_TargetPos;		//注視点
	VECTOR m_UpVec;			//カメラの上方向
	VECTOR m_CameraRot;		//カメラ回転角度
	VECTOR m_CalcRot;

	VECTOR m_CameraPoint;
	VECTOR m_TargetPoint;

	bool m_IsFree1;
	bool m_IsFree2;

	bool m_IsTarget1;
	bool m_IsTarget2;

public:
	//コンストラクタ
	PlayCamera();

	//初期化
	void Init();

	//毎フレーム呼び出す処理(ノーマル)
	void Step(VECTOR _TargetPos);

	//毎フレーム呼び出す処理(ロックオン)
	void Step(VECTOR _TargetPos,VECTOR _PlayerPos,bool _IsSPAtk);

	//更新処理
	void Update();

	//デバック用
	void Draw();

	//カメラの回転率
	VECTOR GetCameraRot() { return m_CameraRot; }

	VECTOR GetCameraPos() { return m_CameraPos; }

	VECTOR GetTarget() { return m_TargetPos; }

	bool GetIsTarget1() { return m_IsTarget1; }
	bool GetIsTarget2() { return m_IsTarget2; }
};
