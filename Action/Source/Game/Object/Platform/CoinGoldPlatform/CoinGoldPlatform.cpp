#include "CoinGoldPlatform.h"
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
CoinGoldPlatform::CoinGoldPlatform() {
	Init();
}
//デストラクタ
CoinGoldPlatform::~CoinGoldPlatform() {
	Exit();
}
//初期化処理
void CoinGoldPlatform::Init() {
	PlatformBase::Init();

	m_Owner = nullptr;
	m_EnemyPush = false;		//敵と当たり判定を行わない
	m_IsItem = true;			//アイテム系である

	m_IsHit = false;
	m_State = IDEL;

	m_OrbitAngle = 0.0f;		//現在の旋回角度
	m_OrbitTotalAngle = 0.0f;	//累積旋回角度
	m_OrbitCount = 0.0f;		//現在の周回数
}
//毎フレーム呼び出す処理
void CoinGoldPlatform::Step() {
	if (!m_IsActive)return;
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

	DrawFormatStringToHandle(10, 200, RED, DxLibFont::FONTHNDL_N20, "プレイヤー座標X:%.1f", m_Pos.x);
	DrawFormatStringToHandle(10, 220, RED, DxLibFont::FONTHNDL_N20, "プレイヤー座標Y:%.1f", m_Pos.y);
	DrawFormatStringToHandle(10, 240, RED, DxLibFont::FONTHNDL_N20, "プレイヤー座標Z:%.1f", m_Pos.z);
}
//当たり判定後の処理(当たっている場合)
void CoinGoldPlatform::HitCalc(ObjectBase* _Owner) {
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
//待機
void CoinGoldPlatform::Idel() {
	//回転
	m_Rot.y += ROTATION_SPEED * RADIAN_CALC;
}
//旋回
void CoinGoldPlatform::Orbit() {
	//回転
	m_Rot.y += ROTATION_SPEED * RADIAN_CALC;

	//旋回角度を加算
	m_OrbitAngle += ORBIT_SPEED;
	m_OrbitTotalAngle += ORBIT_SPEED;

	//旋回半径を徐々に小さくする
	m_OrbitRadius += -ORBIT_RADIUS_DECREASE_SPEED;

	//プレイヤー座標を取得
	VECTOR Pos = m_Owner->GetPos();

	//プレイヤーを中心に旋回
	m_Pos.x = Pos.x + cosf(m_OrbitAngle) * m_OrbitRadius;
	m_Pos.y = Pos.y + ORBIT_HEIGHT_OFFSET;
	m_Pos.z = Pos.z + sinf(m_OrbitAngle) * m_OrbitRadius;

	//徐々に縮小
	m_Scale = VScale(m_Scale, SCALE_DECREASE_RATE);

	//規定半径まで縮小したら吸収
	if (m_OrbitRadius <= ORBIT_RADIUS_MIN) {
		m_State = DEATH;
	}
}
//消滅
void CoinGoldPlatform::Death() {
	m_IsActive = false;
	Coin::CalcCoinCount(COIN_COUNT);
}
