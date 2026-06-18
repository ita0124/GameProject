#include "FallingPlatform.h"

namespace {
	constexpr VECTOR	INIT_POS = { 75.0f,0.0f,0.0f };
	constexpr char		FILE_PATH[] = ("Data/Model/Stage/FallingPlatform/FallingPlatform.mv1");	//モデルファイルパス
	constexpr int		FALL_WAIT_MAX = 15;		//落ち始めるまでの最大待機時間
	constexpr int		FALL_TIME_MAX = 30;		//落ち続ける最大時間
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

	m_FallWait = 0;			//落ち始めるまでの待機時間
	m_FallingTime = 0;		//落ち続ける時間
	m_RespawnWait = 0;		//再生成までの時間
	m_IsFalling = false;	//落ちてよいか判断する
	m_PlatformKinds = FALLING;	//足場オブジェクト種類を再設定
}
//データ読み込み処理
void FallingPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
	m_FirstPos = m_Pos;
}
//毎フレーム呼び出す処理
void FallingPlatform::Step() {
	if (m_RespawnWait >= RESPAWAN_WAIT_MAX) {
		m_IsActive = true;
		m_IsFalling = false;
		m_RespawnWait = 0;
		m_Pos = m_FirstPos;
	}
	else if (!m_IsActive) {
		m_RespawnWait++;
	}
	else if (m_FallingTime >= FALL_TIME_MAX) {
		m_IsActive = false;
		m_FallingTime = 0;
	}
	else if (m_IsFalling) {
		m_FallingTime++;
		m_Pos.y += FALLING_POSY;

		if (m_Object != nullptr) {
			VECTOR MoveDir = VGet(0.0f, FALLING_POSY, 0.0f);
			m_Object->SetPlatformVec(MoveDir);
			m_Object = nullptr;
		}
	}
}
//当たり判定後の処理(当たっている場合)
void FallingPlatform::HitCalc(ObjectBase* _Object) {
	//一定以上乗られていたら
	if (m_FallWait >= FALL_WAIT_MAX) {
		//落ちてよい
		m_IsFalling = true;
		//待機時間をリセット
		m_FallWait = 0;
	}
	else {
		//待機時間を加算
		m_FallWait++;
	}
	m_Object = _Object;
}
//当たり判定後の処理(当たっていない場合)
void FallingPlatform::NotHitCalc(ObjectBase* _Object) {
	//待機時間をリセット
	m_FallWait = 0;
}
