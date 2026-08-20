#include "MyEffeckseer.h"

MyEffeckseer::EFFECT_DATA MyEffeckseer::m_Effect[EFFECT_NUM];

constexpr const char* FilePath[MyEffeckseer::EFFECTID::EFFECT_NUM] = {
};

//コンストラクタ
MyEffeckseer::MyEffeckseer() {

}

//デストラクタ
MyEffeckseer::~MyEffeckseer() {
	Exit();
}

//エフェクト読み込み
void MyEffeckseer::Load() {
	for (int i = 0; i < EFFECT_NUM; i++) {
		m_Effect[i].Hndl = CEffekseerCtrl::LoadData(FilePath[i]);
	}
}
