#include "Sword.h"
#include "Game/Object/Actor/Character/Player/Player.h"

namespace {
	constexpr float		RAD = 10.0f;											//半径

	constexpr char		FILE_PATH[] = ("Data/Model/Player/Sword/Sword.mv1");	//モデルファイルパス
}

//コンストラクタ
Sword::Sword() {
	Init();
}
//デストラクタ
Sword::~Sword() {

}
//初期化処理
void Sword::Init(ObjectBase* _Object) {
	Init();
	//オーナー変数がnullなら
	if (m_Owner == nullptr) {
		m_Owner = _Object;	//オーナーを設定
	}
}
//データ破棄処理
void Sword::Exit() {
	ObjectBase::Exit();

	//オーナー変数がnullじゃないなら
	if (m_Owner != nullptr) {
		//オーナー変数をnullに
		m_Owner = nullptr;
	}
}
//データ読み込み処理
void Sword::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void Sword::Step() {
	//オーナー変数がnullじゃないなら
	if (m_Owner != nullptr) {
		//指定したフレームのローカル座標からワールド座標に変換する行列を取得
		MATRIX FrameLocalWorldMat = MV1GetFrameLocalWorldMatrix(m_Owner->GetHndl(), Player::FrameNumber::RIGHT_HAND2);
		//Y軸回転行列を取得する
		MATRIX RotYMat = MGetRotY(0.0f*RADIAN_CALC);
		//二つの行列を乗算
		MATRIX MultMat = MMult(RotYMat, FrameLocalWorldMat);
		//行列をセット
		MV1SetMatrix(m_Hndl, MultMat);
		////行列から座標に当たる部分を抜き取る
		m_Pos = VGet(MultMat.m[3][0], MultMat.m[3][1], MultMat.m[3][2]);
	}
}
//初期化処理
void Sword::Init() {
	ObjectBase::Init();

	m_Rad = RAD;
	m_Kinds = SWORD;		//種類設定
}
