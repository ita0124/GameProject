#include "FallingPlatform.h"

namespace {
	constexpr VECTOR	INIT_POS = { 0.0f,0.0f,0.0f };
	constexpr char		FILE_PATH[] = ("Data/Model/Stage/FallingPlatform/FallingPlatform.mv1");	//モデルファイルパス
	constexpr int		FALL_WAIT_MAX = 60;		//落ち始めるまでの最大待機時間
	constexpr int		FALL_TIME_MAX = 180;	//落ち続ける最大時間
	constexpr int		RESPAWAN_WAIT_MAX = 60;	//復活までの待機時間
	constexpr float		FALLING_POSY = -1.0f;	//1フレームに落ちるY軸ベクトル
}

//コンストラクタ
FallingPlatform::FallingPlatform() {
	Init();
}
//デストラクタ
FallingPlatform::~FallingPlatform() {
	Exit();
}
//初期化処理
void FallingPlatform::Init() {
	ObjectBase::Init();

	m_FallWait = 0;				//落ち始めるまでの待機時間
	m_FallingTime = 0;			//落ち続ける時間
	m_RespawnWait = 0;			//再生成までの時間
	m_IsHit = false;			//乗られたか
	m_PlatformKinds = FALLING;	//足場オブジェクト種類を再設定
	m_State = NORMAL;			//プレイヤー状態変数
}
//データ読み込み処理
void FallingPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
	m_FirstPos = m_Pos;
}
//毎フレーム呼び出す処理
void FallingPlatform::Step() {
	switch (m_State) {
	case NORMAL:			//通常
		if (m_IsHit) {
			if (m_FallWait >= FALL_WAIT_MAX) {
				//状態変更
				m_State = FALL;
				//待機時間をリセット
				m_FallWait = 0;
			}
			else {
				//待機時間を加算
				m_FallWait++;
			}
		}
		break;
	case FALL:				//落下
		if (m_FallingTime >= FALL_TIME_MAX) {
			//状態変更
			m_State = DEATH;
			m_IsActive = false;
			m_FallingTime = 0;
		}
		else {
			m_FallingTime++;
			m_Pos.y += FALLING_POSY;

			if (m_Object != nullptr) {
				VECTOR MoveDir = VGet(0.0f, FALLING_POSY, 0.0f);
				m_Object->SetPlatformVec(MoveDir);
				m_Object = nullptr;
			}
		}
		break;
	case DEATH:				//消滅
		if (m_RespawnWait >= RESPAWAN_WAIT_MAX) {
			//状態変更
			m_State = NORMAL;
			m_IsActive = true;
			m_RespawnWait = 0;
			m_Pos = m_FirstPos;
			m_IsHit = false;
		}
		else {
			m_RespawnWait++;
		}
		break;
	}
}
//当たり判定後の処理(当たっている場合)
void FallingPlatform::HitCalc(ObjectBase* _Object) {
	if (_Object->GetKinds() == ObjectBase::TagKinds::PLAYER) {
		//乗られた
		m_IsHit = true;
		m_Object = _Object;
	}
}
