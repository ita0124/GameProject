#include "Sky.h"

namespace {
	constexpr VECTOR	SCALE = { 2500.0f,2500.0f,2500.0f };		//初期スケール
	constexpr float		ROTY_SPEED = 0.001f;						//Y軸回転速度
	constexpr char		FILE_PATH[] = ("Data/Model/Sky/Sky.mv1");	//モデルファイルパス
}

//コンストラクタ
Sky::Sky() {
	Init();
}
//デストラクタ
Sky::~Sky() {
	Exit();
}
//初期化処理
void Sky::Init() {
	ObjectBase::Init();

	m_Scale = SCALE;		//スケール再設定
}
//データ読み込み処理
void Sky::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void Sky::Step() {
	m_Rot.y += ROTY_SPEED;
}
