#include "DropCoin.h"
#include "Game/System/Coin/Coin.h"

namespace {
	constexpr float ROTATION_SPEED = 2.5f;					//回転速度
	constexpr float ORBIT_RADIUS_DECREASE_SPEED = 0.8f;		//旋回半径減少速度
	constexpr float ORBIT_HEIGHT_OFFSET = 18.0f;			//旋回時の高さ補正
	constexpr float ORBIT_RADIUS_MIN = 3.0f;				//吸収開始半径
	constexpr float SCALE_DECREASE_RATE = 0.97f;			//縮小率
	constexpr float ORBIT_RADIUS = 25.0f;					//旋回半径
	constexpr float ORBIT_SPEED = 15.0f * RADIAN_CALC;		//旋回速度
	constexpr int	COIN_COUNT = 1;							//取得コイン量
}

//コンストラクタ
DropCoin::DropCoin() {
	Init();
}
//デストラクタ
DropCoin::~DropCoin() {
	Exit();
}
//初期化処理
void DropCoin::Init() {
	DropItemBase::Init();

	m_DropItemKinds = COIN;		//ドロップアイテム種類
}
//毎フレーム呼び出す処理
void DropCoin::Step() {
	if (!m_IsActive)return;
	DropItemBase::Step();
	switch (m_State) {
	case IDEL:
		Idel();
		break;
	case ORBIT:
		Orbit();
		break;
	case DEATH:
		Death();
		break;
	}
}
//当たり判定後の処理(当たっている場合)
void DropCoin::HitCalc(ObjectBase* _Owner) {
	if (_Owner->GetKinds() == ObjectBase::TagKinds::PLAYER) {
		if (!m_IsHit) {
			m_IsHit = true;
			m_Owner = _Owner;
			m_State = ORBIT;

			SoundManager::Play(SoundManager::TagID::SE_COIN);
			//旋回半径
			m_OrbitRadius = ORBIT_RADIUS;

			//プレイヤーとの距離から旋回角度を設定
			VECTOR Pos = m_Owner->GetPos();
			VECTOR Direction = VSub(m_Pos, Pos);
			Direction = VNorm(Direction);

			m_OrbitAngle = atan2f(-Direction.z, -Direction.x);
			m_OrbitTotalAngle = 0.0f;
		}
	}
}
//消滅
void DropCoin::Death() {
	m_IsActive = false;
	Coin::CalcCoinCount(COIN_COUNT);
}
