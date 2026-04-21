#include "ObjectBase.h"

namespace {
	const float	GRAVITY = -0.05f;			//重力
	const float	GRAVITY_MAX = -2.5f;		//最大重力
}

//コンストラクタ
ObjectBase::ObjectBase() {
	m_Hndl = -1;		//画像ハンドル

	Init();
}
//デストラクタ
ObjectBase::~ObjectBase() {
	Exit();
}
//初期化処理
void ObjectBase::Init() {
	m_Pos = VZERO;			//座標
	m_Rot = VZERO;			//回転率
	m_Scale = VONE;			//拡縮
	m_Size = VZERO;			//サイズ
	m_Rad = FZERO;			//半径
	m_IsActive = true;		//生存フラグオン
	m_IsCollision = true;	//当たり判定を実行する
	m_IsPush = false;		//押し出し判定を行わない
	m_Gravity = 0.0f;		//重力
	m_IsGravity = false;	//重力処理をしない
	m_Owner = nullptr;		//オーナーオブジェクト

	m_EffectHndl = -1;	//エフェクトハンドル
	m_IsEffect = false;	//エフェクト出現判定

	m_Kinds = Object;	//種類設定
}
//データ破棄処理
void ObjectBase::Exit() {
	if (m_Hndl == -1)return;
	//ハンドル情報を破棄
	DeleteGraph(m_Hndl);
	//変数を初期化
	m_Hndl = -1;
}
//データ読み込み処理
void ObjectBase::Load(const char* FilePath) {
	if (m_Hndl != -1)return;
	//モデルロード
	m_Hndl = MV1LoadModel(FilePath);

	//初期設定を行う
	MV1SetPosition(m_Hndl, m_Pos);		//座標情報
	MV1SetRotationXYZ(m_Hndl, m_Rot);	//回転角度情報
	MV1SetScale(m_Hndl, m_Scale);		//スケール情報
}
//モデル更新処理
void ObjectBase::Update() {
	MV1SetPosition(m_Hndl, m_Pos);		//座標情報
	MV1SetRotationXYZ(m_Hndl, m_Rot);	//回転角度情報
	MV1SetScale(m_Hndl, m_Scale);		//スケール情報
}
//描画処理
void ObjectBase::Draw() {
	if (!m_IsActive)return;
	MV1DrawModel(m_Hndl);				//モデル描画
}
//重力処理
void ObjectBase::GravityManager() {
	if (!m_IsGravity)return;
	//重力方向に加算
	m_Gravity += GRAVITY;
	//重力速度を制限
	if (m_Gravity <= GRAVITY_MAX) {
		m_Gravity = GRAVITY_MAX;
	}
}
//モデルの中心を取る
VECTOR ObjectBase::GetCenter() {
	VECTOR Res = m_Pos;

	//Y軸を半径分上げる
	Res.y += m_Rad;

	return Res;
}
//重力リセット
void ObjectBase::GravityReset() {
	m_Gravity = 0.0f;
	m_IsGravity = false;
}
