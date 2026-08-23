#include "MyEffeckseer.h"

MyEffeckseer::EFFECT_DATA MyEffeckseer::m_Effect[EFFECT_NUM];

constexpr const char* FilePath[MyEffeckseer::EFFECTID::EFFECT_NUM] = {
	"Data/Effect/Efk/Effekseer01/Simple_GeneratingPosition1.efk",	//被弾
	"Data/Effect/Efk/Effekseer01/Simple_Sprite_BillBoard.efk",		//雑魚敵死亡
	"Data/Effect/Efk/tktk01/Blow5.efk",								//衝撃波
	"Data/Effect/Efk/tktk01/Blow12Down.efk",						//振り下ろし
	"Data/Effect/Efk/tktk01/Blow12Up.efk",							//振り上げ
	"Data/Effect/Efk/tktk01/Sword17.efk",							//剣必殺
	"Data/Effect/Efk/tktk02/Blow2.efk",								//敵必殺
	"Data/Effect/Efk/tktk02/Claw1.efk",								//オオカミ噛みつき
	"Data/Effect/Efk/tktk02/Gun3.efk",								//突進
	"Data/Effect/Efk/tktk02/Sword4Horizontal.efk",					//斬撃横
	"Data/Effect/Efk/tktk02/Sword4Vertical.efk",					//斬撃縦
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
