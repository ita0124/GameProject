#include"3DObject.h"
#include "../../game/common.h"

constexpr float GRAVITIY = 0.1f;						//重力
constexpr float GRAVITIY_MAX = 5.0f;					//最大重力

//---------------------
//	コンストラクタ
//---------------------
CObject::CObject()
{
	Init();
}

//---------------------
//	デストラクタ
//---------------------
CObject::~CObject()
{
	Exit();
}

//---------------------
//		初期化
//---------------------
void CObject::Init()
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_speed = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_scale = { 1.0f,1.0f,1.0f };
	m_size = { 0.0f,0.0f,0.0f };
	m_hndl = -1;
	m_rad = 0.0f;
	m_gravity = 0.0f;
	m_isActive = true;
	m_isGravity = false;
	m_owner = nullptr;
	m_isPushed = true;
	m_objectType = OBJECT_TYPE_SPHERE;
}

//---------------------
//		終了処理
//---------------------
void CObject::Exit()
{
	DeleteModel();
}

//---------------------
//	毎フレームする処理
//---------------------
void CObject::Step()
{
	if (m_isGravity == true)
	{
		//重力処理
		Gravity();
	}

	//重力を速度に加算
	m_speed.y += m_gravity;

	//速度を制限
	if (m_speed.y <= -GRAVITIY_MAX)
	{
		m_speed.y = -GRAVITIY_MAX;
	}

	//現在の座標にスピードを加算
	m_pos = VAdd(m_pos, m_speed);

	//ｙの速度を初期化
	m_speed.y = 0.0f;
}

//---------------------
//		数値の更新
//---------------------
void CObject::Update()
{

	MV1SetPosition(m_hndl, m_pos);
	MV1SetRotationXYZ(m_hndl, m_rot);
	MV1SetScale(m_hndl, m_scale);
}

//---------------------
//	オブジェクトの描写
//---------------------
void CObject::Draw()
{
	//Activeがfalseなら描画をしない
	if (m_isActive == false)return;

#ifdef DEBUG
	//当たり判定を表示
	switch (m_objectType)
	{
	case OBJECT_TYPE_SPHERE:
		DrawSphere3D(GetCenter(), m_rad, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
		break;
	case OBJECT_TYPE_BOX:
		VECTOR half = VScale(m_size, 0.5f);
		VECTOR boxMin = VSub(GetCenter(), half);
		VECTOR boxMax = VAdd(GetCenter(), half);

		VECTOR v[8];

		v[0] = VGet(boxMin.x, boxMin.y, boxMin.z);
		v[1] = VGet(boxMax.x, boxMin.y, boxMin.z);
		v[2] = VGet(boxMax.x, boxMax.y, boxMin.z);
		v[3] = VGet(boxMin.x, boxMax.y, boxMin.z);

		v[4] = VGet(boxMin.x, boxMin.y, boxMax.z);
		v[5] = VGet(boxMax.x, boxMin.y, boxMax.z);
		v[6] = VGet(boxMax.x, boxMax.y, boxMax.z);
		v[7] = VGet(boxMin.x, boxMax.y, boxMax.z);

		DrawLine3D(v[0], v[1], GetColor(255, 0, 0));
		DrawLine3D(v[1], v[2], GetColor(255, 0, 0));
		DrawLine3D(v[2], v[3], GetColor(255, 0, 0));
		DrawLine3D(v[3], v[0], GetColor(255, 0, 0));

		// 上の面
		DrawLine3D(v[4], v[5], GetColor(255, 0, 0));
		DrawLine3D(v[5], v[6], GetColor(255, 0, 0));
		DrawLine3D(v[6], v[7], GetColor(255, 0, 0));
		DrawLine3D(v[7], v[4], GetColor(255, 0, 0));

		// 側面
		for (int i = 0; i < 4; i++)
		{
			DrawLine3D(v[i], v[i + 4], GetColor(255, 0, 0));
		}
		break;
	}


#endif // DEBUG

	MV1DrawModel(m_hndl);
}

//---------------------
//オブジェクトのロード
//---------------------
void CObject::Load()
{

}

//---------------------
//	モデルデータロード
//---------------------
bool CObject::LoadModel(const char* _filePath)
{
	if (m_hndl == -1)
	{
		m_hndl = MV1LoadModel(_filePath);
		return true;
	}
	return false;
}

//---------------------
//	モデルコピーロード
//---------------------
bool CObject::DuplicateModel(int _originHndl)
{
	if (m_hndl == -1)
	{
		m_hndl = MV1DuplicateModel(_originHndl);
		return true;
	}
	return false;
}

//---------------------
//	モデルデータ削除
//---------------------
void CObject::DeleteModel()
{
	if (m_hndl != -1)
	{
		MV1DeleteModel(m_hndl);
		m_hndl = -1;
	}
}

//------------------------------
//		当たり判定後の処理
//------------------------------
void CObject::HitCalc(CObject* _hitObject)
{
	
}

//------------------------------
//	    マップに触れたとき
//------------------------------
void CObject::HitMapCalc()
{

}

//------------------------------
//			重力リセット
//------------------------------
void CObject::GravityReset()
{
	m_gravity = 0.0f;
	m_isFlying = false;
}

//------------------------------
//		中心座標を取得
//------------------------------
VECTOR CObject::GetCenter()
{
	VECTOR center = m_pos;

	switch (m_objectType)
	{
	case OBJECT_TYPE_BOX:
		center.y += m_size.y * 0.5f;
		break;
	case OBJECT_TYPE_SPHERE:
		center.y += m_rad;
		break;
	}

	return center;
}

//-----------------------------
//		  速度のリセット
//------------------------------
void CObject::ResetSpeed()
{
	m_speed.x *= 0.9f;
	m_speed.z *= 0.9f;
}

//-----------------------------
//			重力処理
//-----------------------------
void CObject::Gravity()
{
	m_gravity -= GRAVITIY;
}
