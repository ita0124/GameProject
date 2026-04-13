#include "InputKey.h"


char InputKey::m_NowBuf[KEY_BUF_MAX];
char InputKey::m_OldBuf[KEY_BUF_MAX];

//コンストラクタ
InputKey::InputKey() {
	Init();
}

//初期化
void InputKey::Init() {
	memset(m_NowBuf, 0, KEY_BUF_MAX);
	memset(m_OldBuf, 0, KEY_BUF_MAX);
}

//キー入力情報を更新
void InputKey::Update() {
	memcpy(m_OldBuf, m_NowBuf, KEY_BUF_MAX);
	GetHitKeyStateAll(m_NowBuf);
}

//キーリピート入力情報取得
bool InputKey::IsPushKeyRep(int _Key) {
	return m_NowBuf[_Key] == 1;
}

//キートリガー入力情報取得
bool InputKey::IsPushKeyTrg(int _Key) {
	return (m_NowBuf[_Key] == 1 && m_OldBuf[_Key] != 1);
}

//キーを離したかの情報取得
bool InputKey::IsPushKeyLet(int _Key) {
	return (m_NowBuf[_Key] != 1 && m_OldBuf[_Key] == 1);
}
