#include "DxLib.h"
#include "effekseer.h"
#include <locale.h>

// -----------------------------------------------------------------------
//  ライブラリ自動リンク設定
//  VS2022 (_MSC_VER 1930?1949) と VS2026 (_MSC_VER >= 1950) に対応。
//  最新Effekseerは DirectX9 レンダラーを廃止。DX11 のみリンクする。
//
//  【リンクが必要な .lib 3点セット】
//    Effekseer.lib                 … コア
//    EffekseerRendererDX11.lib     … DX11レンダラー本体
//    EffekseerRendererCommon.lib   … GpuParticleSystem 等の共通レンダラー
//                                    最新版で追加。これがないと LNK2019 が大量に出る
//
//  【VS2026について】
//  VS2026からMSVCのバージョン管理がVSのバージョンと切り離され、
//  _MSC_VER は 1950 からスタートする。
//  ただしMSVCのABIはVS2022と互換性が維持されているため、
//  VS2022でビルドしたEffekseerの.libをVS2026でも流用できる。
//  そのため VS2026 でも VS2022 用の .lib を指定している。
// -----------------------------------------------------------------------
#if _MSC_VER >= 1950
  // VS2026以降 → VS2022互換のlibを使用
  #ifdef _DEBUG
    #ifdef _WIN64
      #pragma comment(lib,"Effekseer_vs2022_x64_d.lib")
      #pragma comment(lib,"EffekseerRendererDX11_vs2022_x64_d.lib")
      #pragma comment(lib,"EffekseerRendererCommon_vs2022_x64_d.lib")
    #else
      #pragma comment(lib,"Effekseer_vs2022_x86_d.lib")
      #pragma comment(lib,"EffekseerRendererDX11_vs2022_x86_d.lib")
      #pragma comment(lib,"EffekseerRendererCommon_vs2022_x86_d.lib")
    #endif
  #else
    #ifdef _WIN64
      #pragma comment(lib,"Effekseer_vs2022_x64.lib")
      #pragma comment(lib,"EffekseerRendererDX11_vs2022_x64.lib")
      #pragma comment(lib,"EffekseerRendererCommon_vs2022_x64.lib")
    #else
      #pragma comment(lib,"Effekseer_vs2022_x86.lib")
      #pragma comment(lib,"EffekseerRendererDX11_vs2022_x86.lib")
      #pragma comment(lib,"EffekseerRendererCommon_vs2022_x86.lib")
    #endif
  #endif
#elif _MSC_VER >= 1930
  // VS2022
  #ifdef _DEBUG
    #ifdef _WIN64
      #pragma comment(lib,"Effekseer_vs2022_x64_d.lib")
      #pragma comment(lib,"EffekseerRendererDX11_vs2022_x64_d.lib")
      #pragma comment(lib,"EffekseerRendererCommon_vs2022_x64_d.lib")
    #else
      #pragma comment(lib,"Effekseer_vs2022_x86_d.lib")
      #pragma comment(lib,"EffekseerRendererDX11_vs2022_x86_d.lib")
      #pragma comment(lib,"EffekseerRendererCommon_vs2022_x86_d.lib")
    #endif
  #else
    #ifdef _WIN64
      #pragma comment(lib,"Effekseer_vs2022_x64.lib")
      #pragma comment(lib,"EffekseerRendererDX11_vs2022_x64.lib")
      #pragma comment(lib,"EffekseerRendererCommon_vs2022_x64.lib")
    #else
      #pragma comment(lib,"Effekseer_vs2022_x86.lib")
      #pragma comment(lib,"EffekseerRendererDX11_vs2022_x86.lib")
      #pragma comment(lib,"EffekseerRendererCommon_vs2022_x86.lib")
    #endif
  #endif
#else
  #error "Visual Studio 2022 以降が必要です。"
#endif


// ============================================================
//		EffekseerSingle
// ============================================================

//---------------------------------
//		コンストラクタ
//---------------------------------
EffekseerSingle::EffekseerSingle()
{
	ZeroMemory(&m_pos, sizeof(m_pos));
	ZeroMemory(&m_rot, sizeof(m_rot));
	m_scale  = VGet(1.0f, 1.0f, 1.0f);
	m_hndl   = -1;
	m_ID     = -1;
	m_isLoop = false;
}

//---------------------------------
//		デストラクタ
//---------------------------------
EffekseerSingle::~EffekseerSingle()
{
}

//---------------------------------
//		リセット
//---------------------------------
void EffekseerSingle::Reset(void)
{
	ZeroMemory(&m_pos, sizeof(m_pos));
	ZeroMemory(&m_rot, sizeof(m_rot));
	m_scale  = VGet(1.0f, 1.0f, 1.0f);
	m_hndl   = -1;
	m_ID     = -1;
	m_isLoop = false;
}


// ============================================================
//		EffekseerManager  静的メンバ定義
// ============================================================
std::vector<EffekseerSingle>			EffekseerManager::m_eff;
std::vector<::Effekseer::EffectRef>		EffekseerManager::m_effect;
::EffekseerRendererDX11::RendererRef	EffekseerManager::m_renderer = nullptr;
::Effekseer::ManagerRef					EffekseerManager::m_manager  = nullptr;
int										EffekseerManager::m_allNum   = 0;
int										EffekseerManager::m_useID    = 0;


// ============================================================
//		EffekseerManager
// ============================================================

//---------------------------------
//		コンストラクタ
//---------------------------------
EffekseerManager::EffekseerManager()
{
}

//---------------------------------
//		デストラクタ
//---------------------------------
EffekseerManager::~EffekseerManager()
{
	Exit();
}

//---------------------------------
//		内部ヘルパー: レンダラー取得
//---------------------------------
::EffekseerRenderer::RendererRef EffekseerManager::GetRenderer()
{
	return (::EffekseerRenderer::RendererRef)m_renderer;
}

//---------------------------------
//		初期化
//	【重要】DxLib を DX_DIRECT3D_11 設定で初期化した後に呼ぶこと。
//	SetUseDirect3DVersion(DX_DIRECT3D_11); の後に DxLib_Init() を実行してください。
//---------------------------------
void EffekseerManager::Init(int effNum, int particleNum)
{
	Exit();

	// DirectX11 デバイス取得
	ID3D11Device*        device  = (ID3D11Device*)GetUseDirect3D11Device();
	ID3D11DeviceContext* context = (ID3D11DeviceContext*)GetUseDirect3D11DeviceContext();

	if (device == nullptr || context == nullptr)
	{
		// DX11 デバイスが取得できない場合は何もしない
		// DxLib 側で SetUseDirect3DVersion(DX_DIRECT3D_11) を呼んでから
		// DxLib_Init() を実行しているか確認してください。
		return;
	}

	// フルスクリーン切り替えでリソースが消えるのを防ぐ
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// レンダラー生成
	m_renderer = ::EffekseerRendererDX11::Renderer::Create(device, context, particleNum);
	if (m_renderer == nullptr) return;

	// マネージャー生成
	m_manager = ::Effekseer::Manager::Create(particleNum);
	if (m_manager == nullptr)
	{
		m_renderer.Reset();
		return;
	}

	// 描画機能をレンダラーから設定
	m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
	m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
	m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
	m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());
	m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());

	// ローダー設定
	m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
	m_manager->SetModelLoader(m_renderer->CreateModelLoader());
	m_manager->SetMaterialLoader(m_renderer->CreateMaterialLoader());
	// CurveLoader は最新版で追加
	m_manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	// 座標系は左手座標系に
	m_manager->SetCoordinateSystem(::Effekseer::CoordinateSystem::LH);

	// 仮の投影・カメラ行列（あとで SetProjectionMtx / SetCameraMtx で上書き）
	m_renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(
			90.0f / 180.0f * 3.14159f, 640.0f / 480.0f, 1.0f, 1000.0f));
	m_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(
			::Effekseer::Vector3D(0.0f, 0.0f, -20.0f),
			::Effekseer::Vector3D(0.0f, 0.0f,   0.0f),
			::Effekseer::Vector3D(0.0f, 1.0f,   0.0f)));

	// 個別エフェクトのメモリ確保
	m_eff.assign(effNum, EffekseerSingle());
	m_allNum = effNum;
	m_effect.clear();
	m_useID = 0;
}


//---------------------------------
//		終了処理
//---------------------------------
void EffekseerManager::Exit(void)
{
	if (m_manager != nullptr)
	{
		m_manager->StopAllEffects();
	}

	m_eff.clear();
	m_eff.shrink_to_fit();
	m_allNum = 0;

	m_effect.clear();

	// RefPtr なので Reset() で解放
	m_manager.Reset();
	m_renderer.Reset();
}


//---------------------------------
//		エフェクトデータロード
//	UTF-8 のパスを UTF-16 に変換して Effekseer に渡す。
//	日本語パスにも対応。
//---------------------------------
int EffekseerManager::LoadData(const char* name)
{
	if (m_manager == nullptr || name == nullptr) return -1;

	// UTF-8 → UTF-16 (wchar_t は Windows で 2byte = UTF-16)
	int wlen = MultiByteToWideChar(CP_UTF8, 0, name, -1, nullptr, 0);
	if (wlen <= 0) return -1;

	std::wstring wname(wlen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, name, -1, &wname[0], wlen);

	// Effekseer の文字列型は EFK_CHAR (= char16_t)
	// Windows の wchar_t は 2byte なので reinterpret_cast で流用可
	auto temp = ::Effekseer::Effect::Create(
		m_manager,
		reinterpret_cast<const EFK_CHAR*>(wname.c_str()));

	if (temp == nullptr) return -1;

	m_effect.push_back(temp);
	return static_cast<int>(m_effect.size()) - 1;
}


//---------------------------------
//		投影行列設定
//---------------------------------
void EffekseerManager::SetProjectionMtx(float pers, float aspect, float nearLength, float farLength)
{
	auto renderer = GetRenderer();
	if (renderer == nullptr) return;

	renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(pers, aspect, nearLength, farLength));
}


//---------------------------------
//		投影行列自動設定
//---------------------------------
void EffekseerManager::SetAutoProjectionMtx()
{
	auto renderer = GetRenderer();
	if (renderer == nullptr) return;

	RECT rect;
	GetWindowCRect(&rect);
	float aspect = static_cast<float>(rect.right  - rect.left) /
	               static_cast<float>(rect.bottom - rect.top);

	renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(
			GetCameraFov(), aspect, GetCameraNear(), GetCameraFar()));
}


//---------------------------------
//		カメラ行列設定
//---------------------------------
void EffekseerManager::SetCameraMtx(VECTOR eyePos, VECTOR focusPos, VECTOR up)
{
	auto renderer = GetRenderer();
	if (renderer == nullptr) return;

	renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(
			::Effekseer::Vector3D(eyePos.x,   eyePos.y,   eyePos.z),
			::Effekseer::Vector3D(focusPos.x, focusPos.y, focusPos.z),
			::Effekseer::Vector3D(up.x,       up.y,       up.z)));
}


//---------------------------------
//		カメラ行列自動設定
//---------------------------------
void EffekseerManager::SetAutoCameraMtx()
{
	auto renderer = GetRenderer();
	if (renderer == nullptr) return;

	VECTOR pos    = GetCameraPosition();
	VECTOR target = GetCameraTarget();
	VECTOR up     = GetCameraUpVector();

	renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(
			::Effekseer::Vector3D(pos.x,    pos.y,    pos.z),
			::Effekseer::Vector3D(target.x, target.y, target.z),
			::Effekseer::Vector3D(up.x,     up.y,     up.z)));
}


//---------------------------------
//		カメラ行列設定（回転角度指定版）
//	(0,0,1) を XYZ 軸順に回転させて注視点を求める。
//---------------------------------
void EffekseerManager::SetCameraRotMtx(VECTOR eyePos, VECTOR rot, VECTOR up)
{
	auto renderer = GetRenderer();
	if (renderer == nullptr) return;

	VECTOR focusPos, temp;

	// X軸回転
	temp.x = 0.0f;
	temp.y = -sinf(rot.x);
	temp.z =  cosf(rot.x);
	focusPos = temp;

	// Y軸回転
	temp.x = focusPos.x * cosf(rot.y) + focusPos.z * sinf(rot.y);
	temp.y = focusPos.y;
	temp.z = focusPos.z * cosf(rot.y) - focusPos.x * sinf(rot.y);
	focusPos = temp;

	// Z軸回転
	temp.x = focusPos.x * cosf(rot.z) - focusPos.y * sinf(rot.z);
	temp.y = focusPos.x * sinf(rot.z) + focusPos.y * cosf(rot.z);
	temp.z = focusPos.z;
	focusPos = temp;

	// 視点に加算して注視点を確定
	focusPos = VAdd(eyePos, focusPos);

	renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(
			::Effekseer::Vector3D(eyePos.x,   eyePos.y,   eyePos.z),
			::Effekseer::Vector3D(focusPos.x, focusPos.y, focusPos.z),
			::Effekseer::Vector3D(up.x,       up.y,       up.z)));
}


//---------------------------------
//		更新処理
//---------------------------------
void EffekseerManager::Update(void)
{
	if (m_manager == nullptr) return;

	m_manager->Update();

	for (int i = 0; i < m_allNum; i++)
	{
		int hndl = m_eff[i].GetHndl();
		if (hndl == -1 || m_manager->Exists(hndl))
		{
			continue;
		}

		// ループ設定のものは終了後に再発生させる
		if (m_eff[i].IsLoop())
		{
			VECTOR pos   = m_eff[i].GetTrans();
			VECTOR scale = m_eff[i].GetScale();
			VECTOR rot   = m_eff[i].GetRot();

			int newHndl = m_manager->Play(m_effect[m_eff[i].GetID()], pos.x, pos.y, pos.z);
			m_eff[i].SetHndl(newHndl);
			m_manager->SetScale(newHndl, scale.x, scale.y, scale.z);
			m_manager->SetRotation(newHndl, rot.x, rot.y, rot.z);
		}
		else
		{
			// 再利用の準備
			m_eff[i].Reset();
		}
	}
}


//---------------------------------
//		更新処理（カメラ設定も自動）
//---------------------------------
void EffekseerManager::UpdateAutoCamera(void)
{
	SetAutoProjectionMtx();
	SetAutoCameraMtx();
	Update();
}


//---------------------------------
//		描画処理
//---------------------------------
void EffekseerManager::Draw(void)
{
	if (m_renderer == nullptr || m_manager == nullptr) return;

	// DxLib の頂点バッファを先に吐き出す
	RenderVertex();

	m_renderer->BeginRendering();
	m_manager->Draw();
	m_renderer->EndRendering();

	// DxLib の Direct3D 設定を元に戻す
	RefreshDxLibDirect3DSetting();
}


//---------------------------------
//		エフェクト呼び出し
//---------------------------------
int EffekseerManager::Request(int ID, VECTOR pos, bool isLoop)
{
	if (m_manager == nullptr) return -1;
	if (ID < 0 || ID >= static_cast<int>(m_effect.size())) return -1;

	for (int i = 0; i < m_allNum; i++)
	{
		int number = (m_useID + i) % m_allNum;
		if (m_eff[number].GetHndl() == -1)
		{
			int newHndl = m_manager->Play(m_effect[ID], pos.x, pos.y, pos.z);
			m_eff[number].SetHndl(newHndl);
			m_eff[number].SetLoop(isLoop);
			m_eff[number].SetID(ID);
			m_eff[number].SetTrans(pos);
			m_eff[number].SetScale(VGet(1.0f, 1.0f, 1.0f));
			m_eff[number].SetRot(VGet(0.0f, 0.0f, 0.0f));
			m_useID = (number + 1) % m_allNum;
			return number;
		}
	}

	return -1;  // 空きスロットなし
}


//---------------------------------
//		指定エフェクト停止
//---------------------------------
void EffekseerManager::Stop(int hndl)
{
	if (m_manager == nullptr) return;
	if (hndl < 0 || hndl >= m_allNum) return;

	int localHndl = m_eff[hndl].GetHndl();
	if (localHndl != -1)
	{
		m_manager->StopEffect(localHndl);
	}
	m_eff[hndl].Reset();
}


//---------------------------------
//		全エフェクト停止
//---------------------------------
void EffekseerManager::StopAll(void)
{
	if (m_manager == nullptr) return;

	m_manager->StopAllEffects();

	for (int i = 0; i < m_allNum; i++)
	{
		m_eff[i].Reset();
	}
}


//---------------------------------
//		座標変更
//---------------------------------
void EffekseerManager::SetPosition(int hndl, VECTOR pos)
{
	if (m_manager == nullptr) return;
	if (hndl < 0 || hndl >= m_allNum) return;

	int localHndl = m_eff[hndl].GetHndl();
	if (localHndl == -1) return;

	m_manager->SetLocation(localHndl, pos.x, pos.y, pos.z);
	m_eff[hndl].SetTrans(pos);
}


//---------------------------------
//		拡大縮小率変更
//---------------------------------
void EffekseerManager::SetScale(int hndl, VECTOR scale)
{
	if (m_manager == nullptr) return;
	if (hndl < 0 || hndl >= m_allNum) return;

	int localHndl = m_eff[hndl].GetHndl();
	if (localHndl == -1) return;

	m_manager->SetScale(localHndl, scale.x, scale.y, scale.z);
	m_eff[hndl].SetScale(scale);
}


//---------------------------------
//		回転角度変更
//---------------------------------
void EffekseerManager::SetRot(int hndl, VECTOR rot)
{
	if (m_manager == nullptr) return;
	if (hndl < 0 || hndl >= m_allNum) return;

	int localHndl = m_eff[hndl].GetHndl();
	if (localHndl == -1) return;

	m_manager->SetRotation(localHndl, rot.x, rot.y, rot.z);
	m_eff[hndl].SetRot(rot);
}


//---------------------------------
//		エフェクトアクティブ判定
//---------------------------------
bool EffekseerManager::IsActive(int hndl)
{
	if (hndl < 0 || hndl >= m_allNum) return false;

	return m_eff[hndl].GetHndl() != -1;
}
