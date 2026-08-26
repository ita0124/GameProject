#include "GoalPlatform.h"

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
	PlatformBase::Init();

	m_Object = nullptr;
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
