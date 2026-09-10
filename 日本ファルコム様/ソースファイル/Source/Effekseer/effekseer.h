#pragma once

//			Effekseer使用クラス
// 使用時はプロパティの「C/C++」→「全般」→「追加のインクルードディレクトリ」と
// 「追加のライブラリディレクトリ」に「Effekseer」を追加してください。
// EffekseerManagerクラスはstaticなので使用する時だけプロジェクトに追加してください。
// また、デバッグ版は警告が出るので「リンカー」→「コマンドライン」→「追加のオプション」に
// 「/ignore:4099 」と記述してください。

//			使用時の注意事項
// EffekseerManager::Draw()前に何かしらDxLibの2D描画関数(半透明処理)が呼ばれてないと
// 描画されない可能性があります。
// また、3Dの透明or半透明の物体の後ろにエフェクトを出してしまうとおかしな見た目になることもあります。

// DirectX9は最新Effekseerで廃止されたためDirectX11専用となります。
// DxLib側も必ずDirectX11モードで初期化してください。
// 現在はVisualStudio2022以降のみ対応しています。

#include <DxLib.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <vector>
#include <string>


class EffekseerSingle;


//		エフェクト管理クラス
//		こっち使って
class EffekseerManager {
protected:
	static std::vector<EffekseerSingle>			m_eff;			// エフェクト単品管理クラス
	static std::vector<::Effekseer::EffectRef>	m_effect;		// エフェクトクラス
	static ::Effekseer::ManagerRef				m_manager;		// エフェクトマネージャ
	static ::EffekseerRendererDX11::RendererRef	m_renderer;		// レンダラー(DirectX11)
	static int									m_allNum;		// 使用エフェクト総数
	static int									m_useID;		// 次に使用予定のID

	//	内部ヘルパー: レンダラー取得（nullptrチェック込み）
	static ::EffekseerRenderer::RendererRef		GetRenderer();

public:
	//	コンストラクタ・デストラクタ
	EffekseerManager();
	~EffekseerManager();

	//	初期化
	//	effNum		:	同時再生できるエフェクトの最大数
	//	particleNum	:	パーティクル総数
	//	@memo		:	DxLib の DX_DIRECT3D_11 設定後に呼ぶこと
	static void Init(int effNum, int particleNum);
	//	終了処理
	static void Exit(void);

	//	エフェクトデータロード
	//	name	:	エフェクトファイルパス
	//	@return	:	成功=エフェクトID, 失敗=-1
	static int	LoadData(const char* name);
	//	投影行列設定
	//	pers		:	カメラのパース(視野角 ラジアン)
	//	aspect		:	アスペクト比
	//	nearLength	:	カメラのニアー
	//	farLength	:	カメラのファー
	//	@memo		:	設定が変わるたびに呼んでください
	static void SetProjectionMtx(float pers, float aspect, float nearLength, float farLength);
	//	投影行列自動設定
	//	@memo	:	DxLibの関数で自動設定
	//				画面分割などには対応不可
	static void SetAutoProjectionMtx(void);
	//	カメラ行列設定
	//	eyePos		:	カメラの視点	
	//	focusPos	:	カメラの注視点
	//	up			:	カメラの上方向
	//	@memo		:	設定が変わるたびに呼んでください
	static void SetCameraMtx(VECTOR eyePos, VECTOR focusPos, VECTOR up);
	//	カメラ行列自動設定
	//	@memo	:	DxLibの関数で自動設定
	static void SetAutoCameraMtx();
	//	カメラ行列設定(回転角度指定版)
	//	eyePos		:	カメラの視点	
	//	rot			:	カメラの角度
	//	up			:	カメラの上方向
	//	@memo		:	設定が変わるたびに呼んでください
	static void SetCameraRotMtx(VECTOR eyePos, VECTOR rot, VECTOR up);
	//	更新処理
	static void Update(void);
	//	更新処理(カメラ設定も自動)
	//	@memo	: ProjectionMtx, CameraMtx の設定も不要
	static void UpdateAutoCamera(void);
	//	描画処理
	static void Draw(void);


	//	エフェクト呼び出し
	//	ID		:	エフェクトID (LoadDataの戻り値)
	//	pos		:	初期位置
	//	isLoop	:	エフェクトがループするか
	//	@return	:	エフェクトハンドル(Stop/SetPosition等に使用), 失敗=-1
	static int Request(int ID, VECTOR pos, bool isLoop = false);
	//	指定エフェクト停止
	//	hndl	:	Requestの戻り値
	static void Stop(int hndl);
	//	全エフェクト停止
	static void StopAll(void);

	//	座標変更
	//	hndl	:	Requestの戻り値
	static void SetPosition(int hndl, VECTOR pos);
	//	拡大縮小率変更
	static void SetScale(int hndl, VECTOR scale);
	//	回転角度変更
	static void SetRot(int hndl, VECTOR rot);
	//	エフェクトアクティブ判定
	//	@return	:	再生中=true, 停止中=false
	static bool IsActive(int hndl);

	//	ロード済みエフェクト数取得
	static int GetEffectCount(void) { return static_cast<int>(m_effect.size()); }
	//	同時再生上限数取得
	static int GetMaxEffectNum(void) { return m_allNum; }
};



//		エフェクト単品管理クラス
//		こちらは基本使わない
class EffekseerSingle {
protected:
	VECTOR				m_pos;			// エフェクトの再生位置
	VECTOR				m_scale;		// エフェクトの倍率
	VECTOR				m_rot;			// エフェクトの回転
	int					m_hndl;			// エフェクトハンドル
	int					m_ID;			// 再生中のエフェクトID
	bool				m_isLoop;		// ループフラグ

public:
	//	コンストラクタ・デストラクタ
	EffekseerSingle();
	~EffekseerSingle();

	void Reset(void);

	//	座標設定
	inline void		SetTrans(VECTOR pos) { m_pos = pos; }
	//	座標取得
	inline VECTOR	GetTrans(void) { return m_pos; }
	//	拡大縮小率設定
	inline void		SetScale(VECTOR scale) { m_scale = scale; }
	//	拡大縮小率取得
	inline VECTOR	GetScale(void) { return m_scale; }
	//	角度設定
	inline void		SetRot(VECTOR rot) { m_rot = rot; }
	//	角度取得
	inline VECTOR	GetRot(void) { return m_rot; }
	//	エフェクトハンドル設定
	inline void		SetHndl(int hndl) { m_hndl = hndl; }
	//	エフェクトハンドル取得
	inline int		GetHndl(void) { return m_hndl; }
	//	エフェクトID設定
	inline void		SetID(int ID) { m_ID = ID; }
	//	エフェクトID取得
	inline int		GetID(void) { return m_ID; }
	//	ループ情報設定
	inline void		SetLoop(bool isLoop) { m_isLoop = isLoop; }
	//	ループ情報取得
	inline bool		IsLoop(void) { return m_isLoop; }
};
