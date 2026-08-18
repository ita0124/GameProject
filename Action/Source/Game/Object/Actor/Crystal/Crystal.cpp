#include "EnergyPoint.h"

namespace {
	static const int MAX_HP = 3.0f;
}


//コンストラクタ・デストラクタ
EnergyPoint::EnergyPoint() {
	Init();
	m_State = WAIT;
}

EnergyPoint::~EnergyPoint() {
	Exit();
}

//初期化処理
void EnergyPoint::Init() {
	Character::Init();

	m_Hp = MAX_HP;
	m_Rad = 15.0f;
}

//データ破棄処理
void EnergyPoint::Exit() {
	Character::Exit();
}

//モデル更新処理
void EnergyPoint::Update() {
	Character::Update();
}

//描画処理
void EnergyPoint::Draw() {
	Character::Draw();
}

//データ読み込み処理
void EnergyPoint::Load(int _Hndl) {
	m_Hndl = MV1DuplicateModel(_Hndl);
}

//毎フレーム呼び出す処理
void EnergyPoint::Step() {
	if (!m_IsActive)return;

	switch (m_State) {
	case WAIT:
		m_State = SPIN;
		break;
	case SPIN:
		if (m_Pos.y < FZERO) {
			m_Pos.y += 1.0f;
		}
		else {
			m_Pos.y = FZERO;
		}

		Character::RequestLoop(ANIME_SPAWN, 0.75f);

		if (m_Hp <= 0.0f) {
			m_State = DEATH;
		}


		if (m_Hp <= 1) {
			MV1SetTextureGraphHandle(m_Hndl, 0, LoadMaterial::MATERIAL_GRADATION_RED, FALSE);
		}
		else if (m_Hp <= 2) {
			MV1SetTextureGraphHandle(m_Hndl, 0, LoadMaterial::MATERIAL_GRADATION_YELLOW, FALSE);
		}

		/*m_Hp--;*/
		break;
	case DEATH:
		Character::RequestEndLoop(ANIME_DEATH);

		if (m_AnimeFlg) {
			m_IsActive = false;
			m_State = WAIT;
		}
		break;
	}
}

//当たり判定の処理
void EnergyPoint::HitCalc(float _Power) {
	if (_Power >= 50.0f) {
		m_Hp += -_Power;
	}
	else {
		m_Hp += -1;
	}
}

//リクエスト
bool EnergyPoint::Request(const VECTOR& _Pos) {
	if (m_IsActive)return false;

	m_IsActive = true;
	m_Pos = _Pos;
	m_Hp = MAX_HP;
	MV1SetTextureGraphHandle(m_Hndl, 0, LoadMaterial::MATERIAL_GRADATION_BLUE, FALSE);

	return true;
}
