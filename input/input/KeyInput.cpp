#include "keyInput.h"
#include <DxLib.h>

unsigned int CKeyInput::m_nowKey;
unsigned int CKeyInput::m_beforeKey;

//--------------------------------
//		コンストラクタ
//--------------------------------
CKeyInput::CKeyInput() 
{
	Init();
}

//--------------------------------
//		キー入力初期化
//--------------------------------
void CKeyInput::Init()
{
	m_nowKey = 0;
	m_beforeKey = 0;
}
//--------------------------------
//		キー入力情報更新
//--------------------------------
void CKeyInput::Update()
{
	//最新情報は1フレーム前の情報になる
	m_beforeKey = m_nowKey;
	//いったん最新情報は初期化
	m_nowKey = 0;

	//現在のキーの状態を取得
	m_nowKey = CheckHitKeyAll(DX_CHECKINPUT_KEY);
}
//--------------------------------
//	キー入力判定（通常判定）
//--------------------------------
bool CKeyInput::IsRep(unsigned int _key)
{
	if (m_nowKey == _key && m_nowKey != 0)
	{
		return true;
	}
	return false;
}
//--------------------------------
//	キー入力判定（トリガー判定）
//--------------------------------
bool CKeyInput::IsTrg(unsigned int _key)
{
	if ((m_nowKey == _key) && (m_beforeKey != _key))
		return true;
	else
		return false;
}

