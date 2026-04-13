#include "Result.h"

namespace {
	static const int CHANGE_TIME = 180;
	static const int BLENDPARM_MAX = 255;
}

//コンストラクタ
Result::Result() {
	Init();
}

//デストラクタ
Result::~Result() {
	Exit();
}

//初期化処理
void Result::Init() {
	for (int i = 0; i < RESULTHANDL_MAX; i++) {
		m_Hndl[i] = -1;
	}

	m_Time = 0;
	m_BlendParam = 0;
	m_IsUpBlendParam = true;
	m_BlendParamTime = 0;
}

// データ読み込み処理
void Result::Load() {
	for (int i = 0; i < RESULTHANDL_MAX; i++) {
		if (m_Hndl[i] == -1) {
			m_Hndl[i] = LoadGraph(FilePath1[i]);
		}
	}
}

//データ破棄処理
void Result::Exit() {
	for (int i = 0; i < RESULTHANDL_MAX; i++) {
		if (m_Hndl[i] != -1) {
			DeleteGraph(m_Hndl[i]);
			m_Hndl[i] = -1;
		}
	}
}

//描画処理
void Result::Draw() {
	//背景
	DrawRotaGraph((int)SCREEN_HALF_X, (int)SCREEN_HALF_Y, 1.0f, 0.0f, m_Hndl[4], TRUE);
	//敗北
	if(ResultNum::GetNum() == 1){
		DrawRotaGraph((int)SCREEN_HALF_X, (int)SCREEN_UPPER, 1.5f, 0.0f, m_Hndl[2], TRUE);
	}
	//勝利
	else if (ResultNum::GetNum() == 2) {
		DrawRotaGraph((int)SCREEN_HALF_X, (int)SCREEN_UPPER, 1.5f, 0.0f, m_Hndl[3], TRUE);
	}

	int i = m_Time / CHANGE_TIME;

	/*まずはここでアルファ値をセット*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_BlendParam);
	DrawRotaGraph((int)SCREEN_HALF_X, (int)SCREEN_LOWER, 1.5f, 0.0f, m_Hndl[i], TRUE);
	//鉾の画像に影響を出さないよう、初期化設定に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

//毎フレーム行う処理
void Result::Step() {
	m_Time++;
	m_BlendParamTime++;

	if (m_Time >= CHANGE_TIME * 2) {
		m_Time = 0;
	}

	if (m_BlendParamTime >= CHANGE_TIME) {
		m_BlendParamTime = 0;
	}

	if (m_BlendParamTime <= CHANGE_TIME / 2) {
		m_IsUpBlendParam = true;
	}
	else {
		m_IsUpBlendParam = false;
	}

	if (m_IsUpBlendParam) {
		m_BlendParam += BLENDPARM_MAX / (CHANGE_TIME / 2);
	}
	else if (!m_IsUpBlendParam) {
		m_BlendParam -= BLENDPARM_MAX / (CHANGE_TIME / 2);
	}

	if (m_BlendParam >= BLENDPARM_MAX) {
		m_BlendParam = BLENDPARM_MAX;
	}
	else if (m_BlendParam <= 0) {
		m_BlendParam = 0;
	}
}
