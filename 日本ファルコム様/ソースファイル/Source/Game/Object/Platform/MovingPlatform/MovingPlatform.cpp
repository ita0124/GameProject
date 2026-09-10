#include "MovingPlatform.h"

using namespace std;

namespace {
	constexpr float		MIN_LEN = 0.0f;
	constexpr int		REMOVE_TIME = 30;
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
	PlatformBase::Init();
	m_IsHitCalc = true;			//当たり判定後の計算を行う
	m_IsEndPos = false;			//終端座標にたどり着いたか
	m_MoveDir = VZERO;			//移動方向ベクトル
	m_PrevLen = 0.0f;			//前フレームの座標から終端座標までの距離を保存する
	m_PlatformKinds = MOVING;	//足場オブジェクト種類を再設定
	m_State = STOP;				//状態変数
	m_StopTime = 0;				//到着したら一度止まる継続時間

	m_IsHit = false;
	m_Object.clear();
}
//データ読み込み処理
void MovingPlatform::Load(const int _Hndl) {
	if (m_Hndl != -1)return;
	//モデルロード
	m_Hndl = MV1DuplicateModel(_Hndl);
	//初期の移動方向ベクトルを設定
	m_MoveDir = VSub(m_MovingPlatformRequestData.EndPos, m_MovingPlatformRequestData.FirstPos);
	//ベクトルの長さを取得
	m_PrevLen = VSize(m_MoveDir);
	//ベクトルを正規化
	m_MoveDir = VNorm(m_MoveDir);
}
//毎フレーム呼び出す処理
void MovingPlatform::Step() {
	m_PrevPos = m_Pos;
	switch (m_State) {
	case MOVE:				//移動中
		if (m_IsHit) {
			for (int Index = 0;Index < m_Object.size();Index++) {
				if (m_Object[Index] != nullptr) {
					VECTOR MoveDir = VScale(m_MoveDir, m_MovingPlatformRequestData.MoveSpeed);
					m_Object[Index]->SetPlatformVec(MoveDir);
				}
			}
			m_Object.clear();
			m_IsHit = false;
		}
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
				//状態変更
				m_State = STOP;
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
			else if (m_PrevLen < Len) {
				//状態変更
				m_State = STOP;
				//終端にたどり着いた
				m_IsEndPos = true;
				//移動方向ベクトルを設定
				m_MoveDir = VSub(m_MovingPlatformRequestData.FirstPos, m_MovingPlatformRequestData.EndPos);
				//ベクトルを正規化
				m_MoveDir = VNorm(m_MoveDir);
				//現在の座標から終端座標への方向ベクトルを生成
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
				//状態変更
				m_State = STOP;
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
				//状態変更
				m_State = STOP;
				//終端にたどり着いた
				m_IsEndPos = false;
				//移動方向ベクトルを設定
				m_MoveDir = VSub(m_MovingPlatformRequestData.EndPos, m_MovingPlatformRequestData.FirstPos);
				//ベクトルを正規化
				m_MoveDir = VNorm(m_MoveDir);
				//現在の座標から終端座標への方向ベクトルを生成
				VECTOR CalcDir = VSub(m_MovingPlatformRequestData.EndPos, m_Pos);
				//ベクトルの長さを取得
				m_PrevLen = VSize(CalcDir);
			}
			else {
				m_PrevLen = Len;
			}
		}
		break;
	case STOP:				//停止
		if (m_StopTime >= REMOVE_TIME) {
			//状態変更
			m_State = MOVE;
			//リセット
			m_StopTime = 0;
		}
		else {
			m_StopTime++;
		}
		break;
	}
}
//当たり判定後の処理(当たっている場合)
void MovingPlatform::HitCalc(ObjectBase* _Object) {
	m_IsHit = true;
	m_Object.push_back(_Object);
}
//当たり判定後の処理(当たっていない場合)
void MovingPlatform::NotHitCalc(ObjectBase* _Object) {
	if (m_IsHit)return;
	m_IsHit = false;
}
