#pragma once
#include <DxLib.h>

static const int KEY_BUF_MAX = 256;

//キーボード入力クラス
class InputKey {
protected:
	static char m_NowBuf[KEY_BUF_MAX];	//今フレームのキー情報
	static char m_OldBuf[KEY_BUF_MAX];	//前フレームのキー情報
public:
	//コンストラクタ
	InputKey();

	//初期化
	static void Init();

	//キー入力情報を更新
	static void Update();

	//キーリピート入力情報取得
	//_Key		:チェックするキー
	//@return	:キーを入力したか
	static bool IsPushKeyRep(int _Key);

	//キートリガー入力情報取得
	//_Key		:チェックするキー
	//@return	:キーを一度だけ入力したか
	static bool	IsPushKeyTrg(int _Key);

	//キーを離したかの情報取得
	//_Key		:チェックするキー
	//@return	:キーを離したか
	static bool IsPushKeyLet(int _Key);
};

/*
キー一覧

KEY_INPUT_BACK			// バックスペースキー
KEY_INPUT_TAB			// タブキー
KEY_INPUT_RETURN		// エンターキー


KEY_INPUT_LSHIFT		// 左シフトキー
KEY_INPUT_RSHIFT		// 右シフトキー
KEY_INPUT_LCONTROL		// 左コントロールキー
KEY_INPUT_RCONTROL		// 右コントロールキー
KEY_INPUT_ESCAPE		// エスケープキー
KEY_INPUT_SPACE			// スペースキー
KEY_INPUT_PGUP			// ＰａｇｅＵＰキー
KEY_INPUT_PGDN			// ＰａｇｅＤｏｗｎキー
KEY_INPUT_END			// エンドキー
KEY_INPUT_HOME			// ホームキー
KEY_INPUT_LEFT			// 左キー
KEY_INPUT_UP			// 上キー
KEY_INPUT_RIGHT			// 右キー
KEY_INPUT_DOWN			// 下キー
KEY_INPUT_INSERT		// インサートキー
KEY_INPUT_DELETE		// デリートキー


KEY_INPUT_MINUS			// －キー
KEY_INPUT_YEN			// ￥キー
KEY_INPUT_PREVTRACK		// ＾キー
KEY_INPUT_PERIOD		// ．キー
KEY_INPUT_SLASH			// ／キー
KEY_INPUT_LALT			// 左ＡＬＴキー
KEY_INPUT_RALT			// 右ＡＬＴキー
KEY_INPUT_SCROLL		// ScrollLockキー
KEY_INPUT_SEMICOLON		// ；キー
KEY_INPUT_COLON			// ：キー
KEY_INPUT_LBRACKET		// ［キー
KEY_INPUT_RBRACKET		// ］キー
KEY_INPUT_AT			// ＠キー
KEY_INPUT_BACKSLASH		// ＼キー
KEY_INPUT_COMMA			// ，キー
KEY_INPUT_CAPSLOCK		// CaspLockキー
KEY_INPUT_PAUSE			// PauseBreakキー


KEY_INPUT_NUMPAD0		// テンキー０
KEY_INPUT_NUMPAD1		// テンキー１
KEY_INPUT_NUMPAD2		// テンキー２
KEY_INPUT_NUMPAD3		// テンキー３
KEY_INPUT_NUMPAD4		// テンキー４
KEY_INPUT_NUMPAD5		// テンキー５
KEY_INPUT_NUMPAD6		// テンキー６
KEY_INPUT_NUMPAD7		// テンキー７
KEY_INPUT_NUMPAD8		// テンキー８
KEY_INPUT_NUMPAD9		// テンキー９
KEY_INPUT_MULTIPLY		// テンキー＊キー
KEY_INPUT_ADD			// テンキー＋キー
KEY_INPUT_SUBTRACT		// テンキー－キー
KEY_INPUT_DECIMAL		// テンキー．キー
KEY_INPUT_DIVIDE		// テンキー／キー
KEY_INPUT_NUMPADENTER	// テンキーのエンターキー


KEY_INPUT_F1			// Ｆ１キー
KEY_INPUT_F2			// Ｆ２キー
KEY_INPUT_F3			// Ｆ３キー
KEY_INPUT_F4			// Ｆ４キー
KEY_INPUT_F5			// Ｆ５キー
KEY_INPUT_F6			// Ｆ６キー
KEY_INPUT_F7			// Ｆ７キー
KEY_INPUT_F8			// Ｆ８キー
KEY_INPUT_F9			// Ｆ９キー
KEY_INPUT_F10			// Ｆ１０キー
KEY_INPUT_F11			// Ｆ１１キー
KEY_INPUT_F12			// Ｆ１２キー


KEY_INPUT_A				// Ａキー
KEY_INPUT_B				// Ｂキー
KEY_INPUT_C				// Ｃキー
KEY_INPUT_D				// Ｄキー
KEY_INPUT_E				// Ｅキー
KEY_INPUT_F				// Ｆキー
KEY_INPUT_G				// Ｇキー
KEY_INPUT_H				// Ｈキー
KEY_INPUT_I				// Ｉキー
KEY_INPUT_J				// Ｊキー
KEY_INPUT_K				// Ｋキー
KEY_INPUT_L				// Ｌキー
KEY_INPUT_M				// Ｍキー
KEY_INPUT_N				// Ｎキー
KEY_INPUT_O				// Ｏキー
KEY_INPUT_P				// Ｐキー
KEY_INPUT_Q				// Ｑキー
KEY_INPUT_R				// Ｒキー
KEY_INPUT_S				// Ｓキー
KEY_INPUT_T				// Ｔキー
KEY_INPUT_U				// Ｕキー
KEY_INPUT_V				// Ｖキー
KEY_INPUT_W				// Ｗキー
KEY_INPUT_X				// Ｘキー
KEY_INPUT_Y				// Ｙキー
KEY_INPUT_Z				// Ｚキー
KEY_INPUT_0				// ０キー
KEY_INPUT_1				// １キー
KEY_INPUT_2				// ２キー
KEY_INPUT_3				// ３キー
KEY_INPUT_4				// ４キー
KEY_INPUT_5				// ５キー
KEY_INPUT_6				// ６キー
KEY_INPUT_7				// ７キー
KEY_INPUT_8				// ８キー
KEY_INPUT_9				// ９キー

*/
