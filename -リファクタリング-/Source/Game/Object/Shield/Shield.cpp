#include "Shield.h"
#include "Game/Object/Actor/Character/Player/Player.h"

namespace {
	const float		RAD = 10.0f;											//半径

	const char		FILE_PATH[] = ("Data/Model/Player/Shield/Shield.mv1");	//モデルファイルパス
}

//コンストラクタ
Shield::Shield() {
	Init();
}
//デストラクタ
Shield::~Shield() {

}
//初期化処理
void Shield::Init(ObjectBase* _Object) {
	Init();
	//オーナー変数がnullなら
	if (m_Owner == nullptr) {
		m_Owner = _Object;	//オーナーを設定
	}
}
//データ破棄処理
void Shield::Exit() {
	ObjectBase::Exit();

	//オーナー変数がnullじゃないなら
	if (m_Owner != nullptr) {
		//オーナー変数をnullに
		m_Owner = nullptr;
	}
}
//データ読み込み処理
void Shield::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void Shield::Step() {
	//オーナー変数がnullじゃないなら
	if (m_Owner != nullptr) {
		//指定したフレームのローカル座標からワールド座標に変換する行列を取得
		MATRIX FrameLocalWorldMat = MV1GetFrameLocalWorldMatrix(m_Owner->GetHndl(), Player::FrameNamber::HANDEND_LEFT);
		//X軸回転行列を取得する
		MATRIX RotXMat = MGetRotX(DX_PI_F);
		//二つの行列を乗算
		MATRIX MultMat = MMult(RotXMat, FrameLocalWorldMat);
		//行列をセット
		MV1SetMatrix(m_Hndl, MultMat);
		//行列から座標に当たる部分を抜き取る
		m_Pos = VGet(MultMat.m[3][0], MultMat.m[3][1], MultMat.m[3][2]);
	}
}
//初期化処理
void Shield::Init() {
	ObjectBase::Init();

	m_Rad = RAD;
	m_Kinds = SHIELD;		//種類設定
}
