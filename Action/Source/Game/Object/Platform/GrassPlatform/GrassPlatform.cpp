#include "GrassPlatform.h"

namespace {
	constexpr VECTOR	INIT_POS = { 0.0f,0.0f,0.0f };
	constexpr char		FILE_PATH[] = ("Data/Model/Stage/GrassPlatform/GrassPlatform.mv1");	//モデルファイルパス
}

//コンストラクタ
GrassPlatform::GrassPlatform() {
	Init();
}
//デストラクタ
GrassPlatform::~GrassPlatform() {
	Exit();
}
//初期化処理
void GrassPlatform::Init() {
	PlatformBase::Init();

	m_PlatformKinds = GRASS;	//足場オブジェクト種類を再設定
	m_ObjectPush = false;		//オブジェクトを押し戻さない
}
//データ読み込み処理
void GrassPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void GrassPlatform::Step() {
}
