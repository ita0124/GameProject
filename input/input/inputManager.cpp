#include "inputManager.h"
#include "keyInput.h"
#include "mouseInput.h"
#include <DxLib.h>

using namespace std;

vector<int> CInputManager::m_keyButton;
vector<tagPadButton> CInputManager::m_pad;

constexpr int KEY_INPUT_BUTTON[CInputManager::INPUT_NUM] =	//キーボードの入力処理
{
	KEY_INPUT_LEFT,
	KEY_INPUT_RIGHT,
	KEY_INPUT_UP,
	KEY_INPUT_DOWN,
	KEY_INPUT_SPACE,
	KEY_INPUT_LSHIFT,
	KEY_INPUT_F,
};

constexpr tagPadButton PAD_INPUT[CInputManager::INPUT_NUM] =	//コントローラーの入力処理
{
	tagPadButton::BUTTON_LEFT,
	tagPadButton::BUTTON_RIGHT,
	tagPadButton::BUTTON_UP,
	tagPadButton::BUTTON_DOWN,
	tagPadButton::BUTTON_A,
	tagPadButton::BUTTON_X,
	tagPadButton::BUTTON_X,

};

//初期化
void CInputManager::Init()
{
	CKeyInput::Init();
	CPadInput::Init();
	CMouseInput::Init();
	
	for (int input_i = 0; input_i < INPUT_NUM; input_i++)
	{
		m_keyButton.push_back(KEY_INPUT_BUTTON[input_i]);
		m_pad.push_back(PAD_INPUT[input_i]);
	}
}

//更新処理
void CInputManager::Update()
{
	CKeyInput::Update();
	CPadInput::Update();
}


//キー入力情報取得(トリガー判定)
bool CInputManager::IsPushKeyRep(int _key)
{
	if (CKeyInput::IsRep(m_keyButton[_key]))
		return true;

	if (CPadInput::IsRep(m_pad[_key]))
		return true;

	return false;

}

//キー入力情報取得(トリガー判定)
bool CInputManager::IsPushKeyTrg(int _key)
{
	if (CKeyInput::IsTrg(m_keyButton[_key]))
		return true;

	if (CPadInput::IsTrg(m_pad[_key]))
		return true;

	return false;

}

//移動縦入力取得
float CInputManager::GetMoveX()
{
	float x = 0.0f;

	x = CPadInput::GetLX();
	
	if (CKeyInput::IsRep(KEY_INPUT_LEFT) == true)
	{
		x = -1.0f;
	}
	
	if (CKeyInput::IsRep(KEY_INPUT_RIGHT) == true)
	{
		x = 1.0f;
	}

	return x;
}

//移動横入力取得
float CInputManager::GetMoveY()
{
	float y = 0.0f;

	y = CPadInput::GetLY();

	if (CKeyInput::IsRep(KEY_INPUT_UP) == true)
	{
		y = -1.0f;
	}

	if (CKeyInput::IsRep(KEY_INPUT_DOWN) == true)
	{
		y = 1.0f;
	}

	return y;

}

