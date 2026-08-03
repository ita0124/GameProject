#include "BarrierActivator.h"

namespace {
	constexpr char		FILE_PATH[] = ("Data/Model/Stage/BarrierActivator/BarrierActivator.mv1");	//モデルファイルパス
}

//コンストラクタ
BarrierActivator::BarrierActivator() {
	Init();
}
//デストラクタ
BarrierActivator::~BarrierActivator() {
	Exit();
}
//初期化処理
void BarrierActivator::Init() {
	ObjectBase::Init();

	m_GimmickKinds = BARRIERACTIVATOR;			//ギミックオブジェクト種類
	m_IsHit = false;							//乗られたか
}
//データ読み込み処理
void BarrierActivator::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void BarrierActivator::Step(MobEnemyManager _MobEnemyManager, PlatformManager _PlatformManager) {
	if (m_IsHit) {
		for (int PlatformIndex = 0; PlatformIndex < PLATFORM_MAX; PlatformIndex++) {
			//生きているモブ敵をカウントする
			int MobEnemyActiveCount = 0;
			//足場マネージャークラスから一つ取得
			PlatformBase& OnePlatform = _PlatformManager.GetPlatform(PlatformIndex);
			//作用するギミックタイプが違う場合は次のforへ
			if(OnePlatform.GetGimmickType()!= m_GimmickType)continue;
			for (int MobIndex = 0; MobIndex < MOB_ENEMY_MAX; MobIndex++) {
				//モブ敵マネージャークラスから一つ取得
				MobEnemyBase& OneMobEnemy = _MobEnemyManager.GetMobEnemy(MobIndex);
				//作用するギミックタイプが違う場合は次のforへ
				if (OneMobEnemy.GetGimmickType() != m_GimmickType)continue;
				//取得したモブ敵クラスの生存フラグがオンになっていれば
				if (OneMobEnemy.GetIsActive()) {
					//一つ増やす
					MobEnemyActiveCount++;
				}
			}
			//カウントが増えていなければ
			if (MobEnemyActiveCount == 0) {
				OnePlatform.SetIsActive(false);
			}
			else {
				//生存フラグをオンに
				OnePlatform.SetIsActive(true);
			}
		}
	}
}
//当たり判定後の処理(当たっている場合)
void BarrierActivator::HitCalc(ObjectBase* _Object) {
	if (_Object->GetKinds() == ObjectBase::TagKinds::PLAYER) {
		//乗られた
		m_IsHit = true;
	}
}
