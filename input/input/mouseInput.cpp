#include "mouseInput.h"
#include <DxLib.h>

unsigned int CMouseInput::m_nowKey;
unsigned int CMouseInput::m_beforeKey;

//--------------------------------
//		コンストラクタ
//--------------------------------
CMouseInput::CMouseInput()
{
	Init();
}

//--------------------------------
//		キー入力初期化
//--------------------------------
void CMouseInput::Init()
{
	m_nowKey = 0;
	m_beforeKey = 0;
}
//--------------------------------
//		キー入力情報更新
//--------------------------------
void CMouseInput::Update()
{
	//最新情報は1フレーム前の情報になる
	m_beforeKey = m_nowKey;
	//いったん最新情報は初期化
	m_nowKey = 0;
	
	//現在のマウスの状態を取得
	m_nowKey = GetMouseInput();

}
//--------------------------------
//	キー入力判定（通常判定）
//--------------------------------
bool CMouseInput::IsRep(unsigned int _key)
{
	if ((m_nowKey & _key) != 0) return true;
	else return false;
}
//--------------------------------
//	キー入力判定（トリガー判定）
//--------------------------------
bool CMouseInput::IsTrg(unsigned int _key)
{
	if ((m_nowKey & _key) && !(m_beforeKey & _key)) return true;
	else return false;
}

