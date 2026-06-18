#include "Sword.h"
#include "Game/Object/Actor/Character/Player/Player.h"

namespace {
	constexpr float		RAD = 5.0f;												//半径

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
		////指定したフレームのローカル座標からワールド座標に変換する行列を取得
		//MATRIX FrameLocalWorldMat = MV1GetFrameLocalWorldMatrix(m_Owner->GetHndl(), 15);
		//指定したフレームのローカル座標からワールド座標に変換する行列を取得
		MATRIX FrameLocalWorldMat = MV1GetFrameLocalWorldMatrix(m_Owner->GetHndl(), 24);
		//X軸回転行列を取得する
		MATRIX RotXMat = MGetRotY(DX_PI_F);
		//二つの行列を乗算
		MATRIX MultMat = MMult(RotXMat, FrameLocalWorldMat);
		MultMat.m[0][0] = MultMat.m[1][1] = MultMat.m[2][2] = 1.0f;
		//行列をセット
		MV1SetMatrix(m_Hndl, MultMat);
		////行列から座標に当たる部分を抜き取る
		m_Pos = VGet(MultMat.m[3][0], MultMat.m[3][1], MultMat.m[3][2]);

		//MV1SetPosition(m_Hndl, m_Pos);		//座標情報

		DrawSphere3D(m_Pos,5.0f,DIV,RED,RED,TRUE);

		DrawFormatStringToHandle(50, 600, RED, DxLibFont::FONTHNDL_N20, "ソードX軸：%2f", m_Pos.x);
		DrawFormatStringToHandle(50, 630, RED, DxLibFont::FONTHNDL_N20, "ソードY軸：%2f", m_Pos.y);
		DrawFormatStringToHandle(50, 660, RED, DxLibFont::FONTHNDL_N20, "ソードZ軸：%2f", m_Pos.z);
	}
}
//初期化処理
void Sword::Init() {
	ObjectBase::Init();

	m_Rad = RAD;
	m_Kinds = SWORD;		//種類設定
}
