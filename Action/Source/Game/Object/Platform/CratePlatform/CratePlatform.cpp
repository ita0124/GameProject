#include "CratePlatform.h"
#include "Game/Object/Actor/Character/Player/Player.h"

namespace {
	constexpr VECTOR	EFFECT_SCALE = { 5.0f,5.0f,5.0f };												//エフェクトの拡大倍率
}

//コンストラクタ
CratePlatform::CratePlatform() {
	Init();
}
//デストラクタ
CratePlatform::~CratePlatform() {
	Exit();
}
//初期化処理
void CratePlatform::Init() {
	PlatformBase::Init();

	m_PlatformKinds = CRATE;	//足場オブジェクト種類を再設定
	m_IsBreak = true;			//壊れるか
}
//毎フレーム呼び出す処理
void CratePlatform::Step() {
}
//当たり判定後の処理(当たっている場合)
void HitCalc(ObjectBase* _Object) {

}
//当たり判定後の処理(当たっている場合)
void CratePlatform::HitCalc(ObjectBase* _Object) {
	//プレイヤークラスデータを保存する変数
	Player* PointerPlayer = nullptr;
	//プレイヤークラスをダウンキャスト
	PointerPlayer = dynamic_cast<Player*>(_Object);
	if (PointerPlayer != nullptr) {
		m_IsActive = false;
		//指定ボーンの座標取得
		VECTOR Pos = m_Pos;
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::EFFEKSEER01_ENEMY_DEATH, Pos, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
		//エフェクトの拡大率を設定
		MyEffeckseer::SetScale(m_EffectHndl, EFFECT_SCALE);
	}
}
