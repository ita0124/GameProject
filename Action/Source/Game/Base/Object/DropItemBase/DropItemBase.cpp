#include "DropItemBase.h"

namespace {
	constexpr float		RAD = 20.0f;
	constexpr VECTOR	DROP_ITEM_SIZE = { RAD ,RAD ,RAD };

	constexpr float		ROTATION_SPEED = 2.5f;					//回転速度
	constexpr float		ORBIT_RADIUS_DECREASE_SPEED = 0.8f;		//旋回半径減少速度
	constexpr float		ORBIT_HEIGHT_OFFSET = 18.0f;			//旋回時の高さ補正
	constexpr float		ORBIT_RADIUS_MIN = 3.0f;				//吸収開始半径
	constexpr float		SCALE_DECREASE_RATE = 0.97f;			//縮小率
	constexpr float		ORBIT_RADIUS = 25.0f;					//旋回半径
	constexpr float		ORBIT_SPEED = 15.0f * RADIAN_CALC;		//旋回速度
	constexpr int		COIN_COUNT = 1;							//取得コイン量

	constexpr float		FIRST_JUMP_POWER = 3.0f;			//初回ジャンプ力
	constexpr float		JUMP_POWER_MAX = -7.5f;				//ジャンプ速度の下限
	constexpr float		GRAVITY = -0.025f;					//重力
	constexpr float		GRAVITY_MAX = -0.25f;				//最大重力
}

//コンストラクタ
DropItemBase::DropItemBase() {
	Init();
}
//デストラクタ
DropItemBase::~DropItemBase() {
	Exit();
}
//初期化処理
void DropItemBase::Init() {
	ObjectBase::Init();

	m_Rad = RAD;				//半径
	m_Size = DROP_ITEM_SIZE;	//ボックス当たり判定

	m_IsActive = false;
	m_Owner = nullptr;

	m_IsHit = false;
	m_State = IDEL;
	m_DropItemKinds = COIN;		//ドロップアイテム種類

	m_OrbitAngle = 0.0f;		//現在の旋回角度
	m_OrbitTotalAngle = 0.0f;	//累積旋回角度
	m_OrbitRadius = 0.0f;		//旋回半径
	m_JumpPower = 0.0f;			//ジャンプ力計算
}
// データ読み込み処理
void DropItemBase::Load(const int _Hndl) {
	if (m_Hndl != -1)return;
	//モデルロード
	m_Hndl = MV1DuplicateModel(_Hndl);
}
//毎フレーム呼び出す処理
void DropItemBase::Step() {
	if (!m_IsActive)return;
	m_PrevPos = m_Pos;
	if (m_PlatformVec.x != 0.0f || m_PlatformVec.y != 0.0f || m_PlatformVec.z != 0.0f) {
		m_Pos = VAdd(m_Pos, m_PlatformVec);
	}
	m_PlatformVec = VZERO;
	//重力処理
	GravityManager();
}
//待機
void DropItemBase::Idel() {
	//回転
	m_Rot.y += ROTATION_SPEED * RADIAN_CALC;
}
//旋回
void DropItemBase::Orbit() {
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
//重力処理
void DropItemBase::GravityManager() {
	if (m_IsGravity) {
		//現在のY座標にジャンプ力を加算
		m_Pos.y += m_JumpPower;
		//重力方向に加算
		m_Gravity += GRAVITY;
		//重力速度を制限
		if (m_Gravity <= GRAVITY_MAX) {
			m_Gravity = GRAVITY_MAX;
		}
		if (m_JumpPower <= JUMP_POWER_MAX) {
			m_JumpPower = JUMP_POWER_MAX;
		}
		//ジャンプ力減衰
		m_JumpPower += m_Gravity;
		//
		m_Pos = VAdd(m_Pos, m_MoveVec);

		if (m_JumpPower > 0.0f) {
			m_Scale = VScale(m_Scale, 1.025f);
		}
		else {
			m_Scale = VScale(m_Scale, 0.97f);
		}
	}
	else {
		//ジャンプ力リセット
		m_JumpPower = 0.0f;

		m_Scale = VONE;
	}
}
//リクエスト
bool DropItemBase::Request(const VECTOR& _Pos) {
	if (m_IsActive)return false;

	m_IsHit = false;
	m_IsActive = true;
	m_IsGravity = true;
	m_Pos = _Pos;
	m_JumpPower = FIRST_JUMP_POWER;
	VECTOR MoveVec = VGet((float)GetRand(100), 0.0f, (float)GetRand(100));
	m_MoveVec = VNorm(MoveVec);
	m_Scale = VONE;

	m_State = IDEL;

	SoundManager::Play(SoundManager::SE_DROP_ITEM);

	return true;
}
