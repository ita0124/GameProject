#include "GoalPlatform.h"

namespace {
	constexpr float		MIN_LEN = 0.0f;
	constexpr VECTOR	INIT_POS = { 25.0f,0.0f,0.0f };
	constexpr char		FILE_PATH[] = ("Data/Model/Stage/GoalPlatform/GoalPlatform.mv1");	//モデルファイルパス
}

//コンストラクタ
GoalPlatform::GoalPlatform() {
	Init();
}
//デストラクタ
GoalPlatform::~GoalPlatform() {
	Exit();
}
//初期化処理
void GoalPlatform::Init() {
	ObjectBase::Init();

	m_Object = nullptr;
}
//データ読み込み処理
void GoalPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void GoalPlatform::Step() {
}
//当たり判定後の処理(当たっている場合)
void GoalPlatform::HitCalc(ObjectBase* _Object) {
	if (_Object->GetKinds() == ObjectBase::TagKinds::PLAYER) {
		m_Object = _Object;
		if (m_Object != nullptr) {
			m_Object->SetIsGoal(true);
			m_Object = nullptr;
		}
	}
}
