#include "MyEffeckseer.h"

MyEffeckseer::EFFECT_DATA MyEffeckseer::m_Effect[EFFECT_NUM];

const char* FilePath[MyEffeckseer::EFFECTID::EFFECT_NUM] = {
	"Data/Effect/Effect/0tktk01Blow7.efk",
	"Data/Effect/Effect/1tktk01Blow3.efk",
	"Data/Effect/Effect/2tktk01Blow9.efk",
	"Data/Effect/Effect/3tktk01Blow12.efk",
	"Data/Effect/Effect/4tktk01Sword1_1.efk",
	"Data/Effect/Effect/5tktk01Sword1_2.efk",
	"Data/Effect/Effect/6tktk01Sword21.efk",
	"Data/Effect/Effect/7AndrewFM01blue_laser.efk",
	"Data/Effect/Effect/8tktk02Blow3.efk",
	"Data/Effect/Effect/9LossOfBlood.efk",
	"Data/Effect/Effect/10tktk02Blow2.efk",
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
