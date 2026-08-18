#include "Crystal.h"

namespace {
	constexpr int		HIT_POINTS = 3.0f;														//体力

	constexpr float		RAD = 5.0f;																//半径

	constexpr int		DAMAGE_TIME = 20;														//ダメージ状態の継続時間

	constexpr float		FLOAT_AMPLITUDE = 10.0f;												//浮遊モーションの振幅
	constexpr float		FLOAT_SPEED = 5.0f;														//浮遊モーションの速度

	constexpr char		FILE_PATH[] = ("Data/Model/Enemy//Boss/MainBody/Boss.mv1");				//モデルファイルパス
}
//コンストラクタ
Crystal::Crystal() {
	Init();
}
//デストラクタ
Crystal::~Crystal() {
	Exit();
}
//初期化処理
void Crystal::Init() {
	CharacterBase::Init();

	m_IsActive = true;

	m_Rad = RAD;		//半径

	m_State = IDEL;		//状態変数を初期化
	m_DamageTime = 0;	//ダメージ処理の継続時間
	m_FloatCount = 0;	//浮遊モーション用カウント
}
//データ読み込み処理
void Crystal::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void Crystal::Step() {
	if (m_DamageTime <= 0) {
		//当たり判定オン
		m_IsCollision = true;
		//ダメージ処理の継続時間をリセット
		m_DamageTime = 0;
	}
	else {
		m_DamageTime--;
	}
	switch (m_State) {
	case IDEL:
		Idel();
		break;
	case ROTATION:
		Rotarion();
		break;
	}
}
//当たり判定後の処理(当たっている場合)
void Crystal::HitCalc(ObjectBase* _Object) {
	if (_Object->GetKinds() == TagKinds::PLAYER) {
		m_HitPoints--;
		//ダメージ処理の継続時間セット
		m_DamageTime = DAMAGE_TIME;
		//当たり判定オフ
		m_IsCollision = false;
		switch ((int)m_HitPoints) {
		case 1:
			//マテリアルを変更
			MV1SetTextureGraphHandle(m_Hndl, 0, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_GRADATION_RED), FALSE);
			break;
		case 2:
			//マテリアルを変更
			MV1SetTextureGraphHandle(m_Hndl, 0, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_GRADATION_YELLOW), FALSE);
			break;
		}
	}
}
//リクエスト
bool Crystal::Request(const VECTOR& _Pos) {
	if (m_IsActive)return false;

	m_IsActive = true;
	m_Pos = _Pos;
	m_HitPoints = HIT_POINTS;

	return true;
}
//待機
void Crystal::Idel() {
	//回転状態へ
	m_State = ROTATION;
}
//回転
void Crystal::Rotarion() {
	//回転アニメーションループ再生
	RequestLoop(ROTATION);
	//上下に浮遊させる
	m_Pos.y = sinf(m_FloatCount * RADIAN_CALC) * FLOAT_AMPLITUDE;

	//浮遊カウント更新
	m_FloatCount += FLOAT_SPEED;
}
