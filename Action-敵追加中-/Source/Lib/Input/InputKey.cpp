#include "InputKey.h"


char InputKey::m_NowKey[KEY_MAX];
char InputKey::m_OldKey[KEY_MAX];

//コンストラクタ
InputKey::InputKey() {
	Init();
}

//初期化
void InputKey::Init() {
	memset(m_NowKey, 0, KEY_MAX);
	memset(m_OldKey, 0, KEY_MAX);
}

//キー入力情報を更新
void InputKey::Update() {
	memcpy(m_OldKey, m_NowKey, KEY_MAX);
	GetHitKeyStateAll(m_NowKey);
}

//キーリピート入力情報取得
bool InputKey::IsPushKeyRep(int _Key) {
	return m_NowKey[_Key] == 1;
}

//キートリガー入力情報取得
bool InputKey::IsPushKeyTrg(int _Key) {
	return (m_NowKey[_Key] == 1 && m_OldKey[_Key] != 1);
}

//キーを離したかの情報取得
bool InputKey::IsPushKeyLet(int _Key) {
	return (m_NowKey[_Key] != 1 && m_OldKey[_Key] == 1);
}
