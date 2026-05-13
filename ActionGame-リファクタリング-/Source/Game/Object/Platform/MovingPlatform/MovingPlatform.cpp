#include "MovingPlatform.h"

namespace {
	const float		MIN_LEN = 0.0f;
	const VECTOR	INIT_POS = { 25.0f,0.0f,0.0f };
	const char		FILE_PATH[] = ("Data/Model/Stage/MovingPlatform/MovingPlatform.mv1");	//モデルファイルパス
}

//コンストラクタ
MovingPlatform::MovingPlatform() {
	Init();
}
//デストラクタ
MovingPlatform::~MovingPlatform() {
	Exit();
}
//初期化処理
void MovingPlatform::Init() {
	ObjectBase::Init();
	m_IsEndPos = false;			//終端座標にたどり着いたか
	m_MoveDir = VZERO;			//移動方向ベクトル
	m_PrevLen = FZERO;			//前フレームの座標から終端座標までの距離を保存する
	m_PlatformKinds = MOVING;	//足場オブジェクト種類を再設定
}
//データ読み込み処理
void MovingPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
	//初期の移動方向ベクトルを設定
	m_MoveDir = VSub(m_MovingPlatformRequestData.EndPos, m_MovingPlatformRequestData.FirstPos);
	//ベクトルの長さを取得
	m_PrevLen = VSize(m_MoveDir);
	//ベクトルを正規化
	m_MoveDir = VNorm(m_MoveDir);
}
//毎フレーム呼び出す処理
void MovingPlatform::Step() {
	//終端座標にたどり着いてなければ
	if (!m_IsEndPos) {
		//正規化された方向ベクトルを取得
		VECTOR Dir = m_MoveDir;
		//正規化したベクトルに移動速度を乗算
		Dir = VScale(Dir, m_MovingPlatformRequestData.MoveSpeed);
		//座標に加算
		m_Pos = VAdd(m_Pos, Dir);
		//切り替え計算
		//現在の座標から終端是表への方向ベクトルを生成
		VECTOR CalcDir = VSub(m_MovingPlatformRequestData.EndPos, m_Pos);
		//ベクトルの長さを取得
		float Len = VSize(CalcDir);
		//長さが一定以下なら
		if (Len <= MIN_LEN) {
			//終端にたどり着いた
			m_IsEndPos = true;
			//移動方向ベクトルを設定
			m_MoveDir = VSub(m_MovingPlatformRequestData.FirstPos, m_MovingPlatformRequestData.EndPos);
			//ベクトルの長さを取得
			m_PrevLen = VSize(m_MoveDir);
			//ベクトルを正規化
			m_MoveDir = VNorm(m_MoveDir);
		}
		//終端座標を通り過ぎている
		else if (m_PrevLen< Len) {
			//終端にたどり着いた
			m_IsEndPos = true;
			//移動方向ベクトルを設定
			m_MoveDir = VSub(m_MovingPlatformRequestData.FirstPos, m_MovingPlatformRequestData.EndPos);
			//ベクトルを正規化
			m_MoveDir = VNorm(m_MoveDir);
			//現在の座標から終端是表への方向ベクトルを生成
			VECTOR CalcDir = VSub(m_MovingPlatformRequestData.FirstPos, m_Pos);
			//ベクトルの長さを取得
			m_PrevLen = VSize(CalcDir);
		}
		else {
			m_PrevLen = Len;
		}
	}
	else {
		//正規化された方向ベクトルを取得
		VECTOR Dir = m_MoveDir;
		//正規化したベクトルに移動速度を乗算
		Dir = VScale(Dir, m_MovingPlatformRequestData.MoveSpeed);
		//座標に加算
		m_Pos = VAdd(m_Pos, Dir);
		//切り替え計算
		//現在の座標から終端是表への方向ベクトルを生成
		VECTOR CalcDir = VSub(m_MovingPlatformRequestData.FirstPos, m_Pos);
		//ベクトルの長さを取得
		float Len = VSize(CalcDir);
		//長さが一定以下なら
		if (Len <= MIN_LEN) {
			//終端にたどり着いた
			m_IsEndPos = false;
			//移動方向ベクトルを設定
			m_MoveDir = VSub(m_MovingPlatformRequestData.EndPos, m_MovingPlatformRequestData.FirstPos);
			//ベクトルの長さを取得
			m_PrevLen = VSize(m_MoveDir);
			//ベクトルを正規化
			m_MoveDir = VNorm(m_MoveDir);
		}
		//終端座標を通り過ぎている
		else if (m_PrevLen < Len) {
			//終端にたどり着いた
			m_IsEndPos = false;
			//移動方向ベクトルを設定
			m_MoveDir = VSub(m_MovingPlatformRequestData.EndPos, m_MovingPlatformRequestData.FirstPos);
			//ベクトルを正規化
			m_MoveDir = VNorm(m_MoveDir);
			//現在の座標から終端是表への方向ベクトルを生成
			VECTOR CalcDir = VSub(m_MovingPlatformRequestData.EndPos, m_Pos);
			//ベクトルの長さを取得
			m_PrevLen = VSize(CalcDir);
		}
		else {
			m_PrevLen = Len;
		}
	}
}
